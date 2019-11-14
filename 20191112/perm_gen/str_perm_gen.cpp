/* str_perm_gen.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "perm_gen_base.hpp"

using namespace std;
using namespace dk;

class StringPermutationGenerator : public PermutationGeneratorBase<char> {
public:
	StringPermutationGenerator(ostream& outputStream, size_t maxNumPerm)
		: outputStream_(outputStream), maxNumPerm_(maxNumPerm), counter_{ 0 } {
	}
private:
	ostream& outputStream_;
	size_t maxNumPerm_;
	size_t counter_;

    virtual bool process_(const vector<char>& permutation) {
        std::string strPermutation(permutation.begin(), permutation.end());
		outputStream_ << strPermutation << endl;

		bool bKeepGoing = true;
		if(maxNumPerm_ > 0) {
			counter_++;
			if (counter_ == maxNumPerm_)
				bKeepGoing = false;
		}
		return bKeepGoing;
    }
};

struct CLIParser {
	string inputString;
	size_t maxNumPerm;
	string filePath;

	bool parse(int argc, char* argv[]) {
		if (argc <= 1 || argc > 4)
			return false;

		inputString = string(argv[1]);
		if (argc > 2)
			if (!parseOption(argv[2]))
				return false;
		if (argc > 3)
			if (!parseOption(argv[3]))
				return false;
		return true;
	}
	bool parseOption(string strOption) {
		std::string strMax("--max=");
		if (strOption.find(strMax) == 0) {
			string strNum(strOption.begin() + 6, strOption.end());
			maxNumPerm = stoi(strNum);
			return true;
		}
		std::string strOut("--out=");
		if (strOption.find(strOut) == 0) {
			filePath = string(strOption.begin() + 6, strOption.end());
			return true;
		}
		return false;
	}
};

void printUsage() {
	cout << "String Permutation Generator" << endl;
	cout << "Displays a specified number of permutations of a specified string or, optionally, writes them to a specified file." << endl;
	cout << "Copyright (c) 2019 David Krikheli" << endl;
	cout << "Usage: str-perm-gen [input_string] [--max=max_num_perm] [--out=output_file]" << endl;
	cout << "\tinput_string -\tthe string to generate permutations of;" << endl;
	cout << "\tmax_num_perm -\tthe maximum number of permutations to generate. This is to help ensure" << endl;
	cout << "\t\t\tthat the process completes normally within a reasonable period of time;" << endl;
	cout << "\toutput_file -\ta path to a file to write the generated permutations to. Note that the writing" << endl;
	cout << "\t\t\tto a file works significantly faster than printing on the screen." << endl;
}

int main (int argc, char* argv[]) {
	CLIParser parser;
    if(!parser.parse(argc, argv)) {
		printUsage();
		return 0;
    }

	string strInput {argv[1]};
	std::vector<char> vocabulary(strInput.begin(), strInput.end());

	ofstream outputStream;
	if (parser.filePath.size())
		outputStream.open(parser.filePath);

	StringPermutationGenerator spg(parser.filePath.size()? outputStream : cout, parser.maxNumPerm);
    spg.generate(vocabulary);

	if(parser.filePath.size())
		outputStream.close();

	return 0;
}
