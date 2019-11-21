/* str_perm_gen_cli.cpp
The command line parser of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    16/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <sstream>
#include <cctype>
#include "str_perm_gen_cli.hpp"

using namespace std;

StrPermGenCLIParserException::StrPermGenCLIParserException(const string& strWhat)
: invalid_argument{strWhat} {
}

StrPermGenCLIParser::StrPermGenCLIParser()
	: argc_{ 0 }, argv_{ nullptr }, inxArg_{ 0 }, strInput_{ "" },
	iStartNum_{ 0 }, iPrintCount_{ 0 }, bPrintNumbers_{ false },
	strOutFile_{ "" }, bExclusionRegex_{ false }, strRegex_{ "" },
	bPresort_{ false }, bPreOrderAscending_{ false }, 
	bLexicographicOrder_{ false }, bLexOrderAscending_{ false },
	bAllowDups_{ false }, iGroupSize_ { 0 }, iTaskRepeatCount_ { 1 }, 
	bDryRun_(false), bHelp_{ false }//, iRandPermEngId_{ 0 }
{
}

static size_t str2size_t_(const string& strNum) noexcept {
	std::istringstream iss(strNum);
	size_t iNum{ 0 };
	iss >> iNum;
	return iNum;
}
static size_t str2size_t_(const char* pStrNum) noexcept {
	std::istringstream iss(pStrNum);
	size_t iNum{ 0 };
	iss >> iNum;
	return iNum;
}
bool StrPermGenCLIParser::parseSize_t_(char symbol, size_t& iValue) {
	if (argv_[inxArg_][1] == symbol) {
		inxArg_++;
		if (inxArg_ == argc_)
			throw StrPermGenCLIParserException(string("Missing numeric argument for CLI option -") + symbol + '.');

		if (!isdigit(argv_[inxArg_][0]))
			throw StrPermGenCLIParserException(string("Require numeric argument for CLI option -") + symbol + '.');

		iValue = str2size_t_(argv_[inxArg_]);
		if (iValue == 0)
			throw StrPermGenCLIParserException(string("Require non-zero argument for CLI option -") + symbol + '.');

		return true;
	}
	return false;
}

bool StrPermGenCLIParser::parse(int argc, char* argv[]) {
	if (argc < 2)
		return false;

	if (argc > 16)
		throw StrPermGenCLIParserException(string("Too many CLI options: ") + to_string(argc));

	argc_ = argc;
	argv_ = argv;

	strInput_ = string(argv_[1]);

	for (inxArg_ = 2; inxArg_ < argc_; inxArg_++) {
		string strOption = string(argv_[inxArg_]);

		// The number of initial permutations to skip.
		if (strOption[0] == '+') {

			string strNum(strOption.begin() + 1, strOption.end());
			if (!isdigit(strNum[0]))
				throw StrPermGenCLIParserException("Require numeric value with CLI argument +.");
			iStartNum_ = str2size_t_(strNum);
			continue;
		}

		if (strOption[0] == '-') {
			// Output file.
			if (strOption[1] == 'o') {
				if (++inxArg_ == argc_)
					throw StrPermGenCLIParserException("Missing output file path.");
				strOutFile_ = string(argv_[inxArg_]);
				continue;
			}
			// Exclusion regex.
			if (strOption[1] == 'e') {
				if (++inxArg_ == argc_ || strRegex_.size() > 0)
					throw StrPermGenCLIParserException(string("Redefinition of regex with CLI option -") + strOption[1] + '.');
				bExclusionRegex_ = true;
				strRegex_ = string(argv_[inxArg_]);
				continue;
			}
			// Inclusion regex.
			if (strOption[1] == 'i') {
				if (++inxArg_ == argc_ || strRegex_.size() > 0)
					throw StrPermGenCLIParserException(string("Redefinition of regex with CLI option -") + strOption[1] + '.');
				bExclusionRegex_ = false;
				strRegex_ = string(argv_[inxArg_]);
				continue;
			}
			// Print permutation numbers.
			if (strOption[1] == 'n') {
				bPrintNumbers_ = true;
				continue;
			}
			// Duplicates are allowed.
			if (strOption[1] == 'a') {
				bAllowDups_ = true;
				continue;
			}
			// Duplicates are allowed.
			if (strOption[1] == 'h') {
				bHelp_ = true;
				continue;
			}
			// Presort the input string.
			if (strOption[1] == 'p') {
				if (++inxArg_ == argc_)
					throw StrPermGenCLIParserException(string("Missing sort order for CLI option -") + strOption[1] + '.');
				if(argv_[inxArg_][0] != 'a' && argv_[inxArg_][0] != 'd')
					throw StrPermGenCLIParserException(string("Invalid sort order for CLI option -") + strOption[1] + '.');
				bPresort_ = true;
				bPreOrderAscending_ = (argv_[inxArg_][0] == 'a') ? true : false;
				continue;
			}
			// Lexicographic order.
			if (strOption[1] == 'l') {
				if (++inxArg_ == argc_)
					throw StrPermGenCLIParserException(string("Missing lexicographic order for CLI option -") + strOption[1] + '.');
				if (argv_[inxArg_][0] != 'a' && argv_[inxArg_][0] != 'd')
					throw StrPermGenCLIParserException(string("Invalid lexicographic order for CLI option -") + strOption[1] + '.');
				bLexicographicOrder_ = true;
				bLexOrderAscending_ = (argv_[inxArg_][0] == 'a') ? true : false;
				continue;
			}

			// The number of permutations to print.
			if (parseSize_t_('c', iPrintCount_))
				continue;
			// The task repeat count in the dry-run mode.
			if (parseSize_t_('t', iTaskRepeatCount_)) {
				bDryRun_ = true;
				continue;
			}
			// The id of the random permutation engine.
			//if (parseSize_t_('r', iRandPermEngId_))
			//	continue;
			// The size of the consecutive groups to randomly pick permutations from.
			if (parseSize_t_('g', iGroupSize_)) {
				if (iGroupSize_ == 1)
					throw StrPermGenCLIParserException(string("Require value of 2 or more for CLI option -") + 'g' + '.');
				continue;
			}

			throw StrPermGenCLIParserException(string("Unknown CLI option -") + strOption[1] + '.');
		}
		string strErrMsg = "CLI options should start with either + or -. Invalid option #";
		strErrMsg += to_string(inxArg_ + 1) + ": " + strOption + ".";
		throw StrPermGenCLIParserException(strErrMsg);
	}	// for

	// Validate compatibility of options.
	if(allowDups() && lexicographic())
		throw StrPermGenCLIParserException("Options -a and -l are incompatible.");
	
	return true;
}
void StrPermGenCLIParser::printUsage() const noexcept {
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
//size_t StrPermGenCLIParser::getRandPermEngId() const noexcept {
//	return iRandPermEngId_;
//}
