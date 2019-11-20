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

StrPermGenCLI::StrPermGenCLI()
	: strInput_{ "" }, iStartNum_{ 0 }, iPrintCount_{ 0 }, bPrintNumbers_{ false },
	strOutFile_{ "" }, bExclusionRegex_{ false }, strRegex_{ "" }, 
	bPresort_{ false }, bAscending_{ false }, bLexicographicOrder_{ false },
	bAllowDups_{ false }, iGroupSize_ { 0 }, iTaskRepeatCount_ { 1 }
{
}
bool StrPermGenCLI::parse(int argc, char* argv[]) noexcept {
	if (argc < 2 || argc > 16)
		return false;

	strInput_ = string(argv[1]);

	for (size_t inx = 2; inx < argc; inx++) {
		string strOption = string(argv[inx]);

		// The number of initial permutations to skip.
		if (strOption[0] == '+') {
			string strNum(strOption.begin() + 1, strOption.end());
			if (!isdigit(strNum[0]))
				return false;
			std::istringstream iss(strNum);
			iss >> iStartNum_;
			continue;
		}

		if (strOption[0] == '-') {
			// The number of permutations to print.
			if (strOption[1] == 'c') {
				if (++inx == argc)
					return false;
				string strNum(argv[inx]);
				if (!isdigit(strNum[0]))
					return false;
				std::istringstream iss(strNum);
				iss >> iPrintCount_;
				continue;
			}
			// The time option. The task repeat count.
			if (strOption[1] == 't') {
				if (++inx == argc)
					return false;
				string strNum(argv[inx]);
				if (!isdigit(strNum[0]))
					return false;
				std::istringstream iss(strNum);
				iss >> iTaskRepeatCount_;
				if (iTaskRepeatCount_ == 0)
					return false;
				continue;
			}
			// The size of consecutive groups to randomly pick permutations from.
			if (strOption[1] == 'g') {
				if (++inx == argc)
					return false;
				string strNum(argv[inx]);
				if (!isdigit(strNum[0]))
					return false;
				std::istringstream iss(strNum);
				iss >> iGroupSize_;
				continue;
			}
			// Output file.
			if (strOption[1] == 'o') {
				if (++inx == argc)
					return false;
				strOutFile_ = string(argv[inx]);
				if (strOutFile_[0] == '-') {
					strOutFile_ = string("");
					return false;
				}
				continue;
			}
			// Exclusion regex.
			if (strOption[1] == 'e') {
				if (++inx == argc || strRegex_.size() > 0)
					return false;
				bExclusionRegex_ = true;
				strRegex_ = string(argv[inx]);
				continue;
			}
			// Inclusion regex.
			if (strOption[1] == 'i') {
				if (++inx == argc || strRegex_.size() > 0)
					return false;
				bExclusionRegex_ = false;
				strRegex_ = string(argv[inx]);
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
			// Presort the input string.
			if (strOption[1] == 'p') {
				if (++inx == argc)
					return false;
				if(argv[inx][0] != 'a' && argv[inx][0] != 'd')
					return false;
				bPresort_ = true;
				bAscending_ = (argv[inx][0] == 'a') ? true : false;
				continue;
			}
			// Lexicographic order.
			if (strOption[1] == 'l') {
				if (++inx == argc)
					return false;
				if (argv[inx][0] != 'a' && argv[inx][0] != 'd')
					return false;
				bLexicographicOrder_ = true;
				bAscending_ = (argv[inx][0] == 'a') ? true : false;
				continue;
			}

			return false;
		}
		return false;
	}	// for
	return true;
}
void StrPermGenCLI::printUsage() const noexcept {
	cout << "Usage: "<< "str-perm-gen [input_string] [options]" << '\n';
	cout << "  +number   - start printing at the specified permutation number (1-based);" << '\n';
	cout << "  -a        - allow duplicates (-a and -l don\'t go together);" << '\n';
	cout << "  -c count  - the 'count' number of permutations to print;" << '\n';
	cout << "  -e regex  - exclusion regex (-e and -i don\'t go together);" << '\n';
	cout << "  -g size   - randomly pick one permutation per consecutive group;" << '\n';
	cout << "  -i regex  - inclusion regex (-i and -e don\'t go together);" << '\n';
	cout << "  -l order  - (a)scending or (d)escending lexicographic order;" << '\n';
	cout << "  -n        - print permutation numbers;" << '\n';
	cout << "  -o path   - output file path;" << '\n';
	cout << "  -p order  - pre-sort the input string in (a)scending or (d)escending order;" << '\n';
	cout << "  -t repeat - dry-run 'repeat' times and print average duration." << '\n';
}

const string& StrPermGenCLI::getInputString() const noexcept {
	return strInput_;
}
size_t StrPermGenCLI::getStartNumber() const noexcept {
	return iStartNum_;
}
size_t StrPermGenCLI::getPrintCount() const noexcept {
	return iPrintCount_;
}
bool StrPermGenCLI::printPermutationNumbers() const noexcept {
	return bPrintNumbers_;
}
const string& StrPermGenCLI::getOutFilePathStr() const noexcept {
	return strOutFile_;
}
bool StrPermGenCLI::isExclusionRegex() const noexcept {
	return bExclusionRegex_;
}
const string& StrPermGenCLI::getRegexStr() const noexcept {
	return strRegex_;
}
bool StrPermGenCLI::presort() const noexcept {
	return bPresort_;
}
bool StrPermGenCLI::ascending() const noexcept {
	return bAscending_;
}
bool StrPermGenCLI::lexicographic() const noexcept {
	return bLexicographicOrder_;
}
bool StrPermGenCLI::allowDups() const noexcept {
	return bAllowDups_;
}
size_t StrPermGenCLI::getGroupSize() const noexcept {
	return iGroupSize_;
}
size_t StrPermGenCLI::getTaskRepeatCount() const noexcept {
	return iTaskRepeatCount_;
}
