/* str_perm_gen_cli.cpp
Command line parser of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    16/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
//#include <sstream>
//#include <cctype>
#include "str_perm_gen_cli.hpp"

using namespace std;
using namespace dk;

StrPermGenCLIParser::StrPermGenCLIParser(int argc, char* argv[])
	: CLIParserBase(argc, argv), strInput_{ "" }, iStartNum_{ 0 }, iPrintCount_{ 0 },
	bPrintNumbers_{ false }, strOutFile_{ "" }, bExclusionRegex_{ false },
	strRegex_{ "" }, bPresort_{ false }, bPreOrderAscending_{ false }, 
	bLexicographicOrder_{ false }, bLexOrderAscending_{ false },
	bAllowDups_{ false }, iGroupSize_ { 0 }, iTaskRepeatCount_ { 1 },
	bDryRun_(false), bHelp_{ false }, iRandPermAlgId_{ 0 }
{
}

bool StrPermGenCLIParser::parse() {
	if (_argc < 2)
		return false;

	if (_argc > 16)
		throw CLIParserException(string("Too many CLI options: ") + to_string(_argc));

	strInput_ = string(_argv[1]);

	for (_inxArg = 2; _inxArg < _argc; _inxArg++) {
		string strOption = string(_argv[_inxArg]);

		// The number of initial permutations to skip.
		if (strOption[0] == '+') {
			string strNum(strOption.begin() + 1, strOption.end());
			if (!isdigit(strNum[0]))
				throw CLIParserException("Require numeric value in CLI option +.");
			iStartNum_ = StrPermGenCLIParser::_str2_size_t(strNum);
			continue;
		}

		if (strOption[0] == '-') {
			// Presort the input string.
			if( _fourStateOption('p', bPresort_, 'a', 'd', bPreOrderAscending_) )
				continue;
			// Lexicographic order.
			if (_fourStateOption('l', bLexicographicOrder_, 'a', 'd', bLexOrderAscending_))
				continue;

			// Output file.
			if (_strOption('o', strOutFile_))
				continue;
			// Exclusion regex.
			if (_strOption('e', strRegex_)) {
				bExclusionRegex_ = true;
				continue;
			}
			// Inclusion regex.
			if (_strOption('i', strRegex_)) {
				bExclusionRegex_ = false;
				continue;
			}

			// Print permutation numbers.
			if (_boolOption('n', bPrintNumbers_))
				continue;
			// Allow duplicates.
			if (_boolOption('a', bAllowDups_))
				continue;
			// Help.
			if (_boolOption('h', bHelp_))
				continue;

			// The number of permutations to print.
			if (_uintOption('c', iPrintCount_))
				continue;
			// The task repeat count in the dry-run mode.
			if (_uintOption('t', iTaskRepeatCount_)) {
				bDryRun_ = true;
				continue;
			}
			// The id of the random permutation algorithm.
			if (_uintOption('r', iRandPermAlgId_)) {
				if (iRandPermAlgId_ > 3)
					throw CLIParserException(string("Invalid random permutation algorithm id in CLI option -") + strOption[1] + '.');
				continue;
			}
			// The size of the consecutive groups to randomly pick permutations from.
			if (_uintOption('g', iGroupSize_)) {
				if (iGroupSize_ < 2)
					throw CLIParserException(string("Require value of 2 or more in CLI option -") + strOption[1] + '.');
				continue;
			}

			throw CLIParserException(string("Unknown CLI option -") + strOption[1] + '.');
		}
		string strErrMsg = "CLI options should start with either + or -. Invalid argument #";
		strErrMsg += to_string(_inxArg + 1) + ": " + strOption + ".";
		throw CLIParserException(strErrMsg);
	}	// for

	// Validate compatibility of options.
	if(allowDups() && lexicographic())
		throw CLIParserException("Options -a and -l are incompatible.");
	
	return true;
}
void StrPermGenCLIParser::printUsage() noexcept {
	cout << "Usage: "<< "str-perm-gen [input_string] [options]" << '\n';
	cout << "  +number   - start printing at the specified permutation number (1-based);" << '\n';
	cout << "  -a        - allow duplicates (options -a and -l are incompatible);" << '\n';
	cout << "  -c count  - the 'count' number of permutations to print;" << '\n';
	cout << "  -e regex  - exclusion regex (options -e and -i are incompatible);" << '\n';
	cout << "  -g size   - randomly pick one permutation per consecutive group;" << '\n';
	cout << "  -h        - print help information. Overrides all other options;" << '\n';
	cout << "  -i regex  - inclusion regex (options -i and -e are incompatible);" << '\n';
	cout << "  -l order  - (a)scending or (d)escending lexicographic order;" << '\n';
	cout << "  -n        - print permutation numbers;" << '\n';
	cout << "  -o path   - output file path;" << '\n';
	cout << "  -p order  - pre-sort the input string in (a)scending or (d)escending order;" << '\n';
	cout << "  -r id     - random permutation algorithm id:" << '\n';
	cout << "                1 - default algorithm;" << '\n';
	cout << "                2 - Richard Durstenfeld (modernised Fisher-Yates);" << '\n';
	cout << "                3 - Sandra Sattolo." << '\n';
	cout << "  -t repeat - dry-run 'repeat' times and print average duration." << '\n';
}
const string& StrPermGenCLIParser::getInputString() const noexcept {
	return strInput_;
}
size_t StrPermGenCLIParser::getStartNumber() const noexcept {
	return iStartNum_;
}
size_t StrPermGenCLIParser::getPrintCount() const noexcept {
	return iPrintCount_;
}
bool StrPermGenCLIParser::printPermutationNumbers() const noexcept {
	return bPrintNumbers_;
}
const string& StrPermGenCLIParser::getOutFilePathStr() const noexcept {
	return strOutFile_;
}
bool StrPermGenCLIParser::isExclusionRegex() const noexcept {
	return bExclusionRegex_;
}
const string& StrPermGenCLIParser::getRegexStr() const noexcept {
	return strRegex_;
}
bool StrPermGenCLIParser::presort() const noexcept {
	return bPresort_;
}
bool StrPermGenCLIParser::preOrderAscending() const noexcept {
	return bPreOrderAscending_;
}
bool StrPermGenCLIParser::lexicographic() const noexcept {
	return bLexicographicOrder_;
}
bool StrPermGenCLIParser::lexOrderAscending() const noexcept {
	return bLexOrderAscending_;
}
bool StrPermGenCLIParser::allowDups() const noexcept {
	return bAllowDups_;
}
size_t StrPermGenCLIParser::getGroupSize() const noexcept {
	return iGroupSize_;
}
size_t StrPermGenCLIParser::getTaskRepeatCount() const noexcept {
	return iTaskRepeatCount_;
}
bool StrPermGenCLIParser::dryRun() const noexcept {
	return bDryRun_;
}
bool StrPermGenCLIParser::help() const noexcept {
	return bHelp_;
}
size_t StrPermGenCLIParser::getRandPermAlgId() const noexcept {
	return iRandPermAlgId_;
}
