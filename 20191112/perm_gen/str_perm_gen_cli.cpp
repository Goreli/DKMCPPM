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
	: strInput_{ "" }, iStartNum_{ 0 }, iCount_{ 0 }, bPrintNumbers_{ false },
	  outFile_{ "" }
{
}
bool StrPermGenCLI::parse(int argc, char* argv[]) noexcept {
	if (argc < 2 || argc > 8)
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
				iss >> iCount_;
				continue;
			}
			// Output file.
			if (strOption[1] == 'o') {
				if (++inx == argc)
					return false;
				outFile_ = string(argv[inx]);
				if (outFile_[0] == '-') {
					outFile_ = string("");
					return false;
				}
				continue;
			}
			// Print permutation numbers.
			if (strOption[1] == 'n') {
				bPrintNumbers_ = true;
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
	cout << "  -c count  - the count of permutations to print;" << '\n';
	cout << "  -n        - print permutation numbers." << '\n';
}

const string& StrPermGenCLI::getInputString() const noexcept {
	return strInput_;
}
size_t StrPermGenCLI::getStartNumber() const noexcept {
	return iStartNum_;
}
size_t StrPermGenCLI::getCount() const noexcept {
	return iCount_;
}
bool StrPermGenCLI::printPermutationNumbers()  const noexcept {
	return bPrintNumbers_;
}
const string& StrPermGenCLI::getOutFilePath() const noexcept {
	return outFile_;
}
