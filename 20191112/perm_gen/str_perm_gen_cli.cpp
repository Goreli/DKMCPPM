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
	: strInput_{ "" }, iStartNum_{ 0 }, iCount_{ 0 }, bPrintNumbers_ { false } {
}
bool StrPermGenCLI::parse(int argc, char* argv[]) {
	if (argc < 2 || argc > 6)
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
				if (inx + 1 == argc)
					return false;
				inx++;
				string strNum(argv[inx]);
				if (!isdigit(strNum[0]))
					return false;
				std::istringstream iss(strNum);
				iss >> iCount_;
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
void StrPermGenCLI::printUsage() {
	cout << "Usage: "<< "str-perm-gen [input_string] [options]" << endl;
	cout << "  +number   - start printing at the specified permutation number (1-based);" << endl;
	cout << "  -c count  - the count of permutations to print;" << endl;
	cout << "  -n        - print permutation numbers." << endl;
}

string& StrPermGenCLI::getInputString() {
	return strInput_;
}
size_t StrPermGenCLI::getStartNumber() {
	return iStartNum_;
}
size_t StrPermGenCLI::getCount() {
	return iCount_;
}
bool StrPermGenCLI::printPermutationNumbers() {
	return bPrintNumbers_;
}
