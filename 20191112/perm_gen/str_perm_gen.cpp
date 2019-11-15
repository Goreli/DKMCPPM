/* str_perm_gen.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <string>
#include "perm_gen_base.hpp"

using namespace std;
using namespace dk;

class StringPermutationGenerator : public PermutationGeneratorBase<char> {
public:
	StringPermutationGenerator(size_t maxNumPerm)
		: maxNumPerm_(maxNumPerm), counter_{ 0 } {
	}
private:
	size_t maxNumPerm_;
	size_t counter_;

    virtual bool process_(const vector<char>& permutation) {
		// Turn the permutation into a string and print it.
        std::string strPermutation(permutation.begin(), permutation.end());
		cout << strPermutation << endl;

		bool bKeepGoing = true;
		// If maxNumPerm_ is greater than zero then it must have been
		// specified on the command line. Apply the counting logic in this
		// case. 
		// Otherwise, if maxNumPerm_ equals zero, it must have been ommited
		/// on the command line. Do not apply the counting logic in this case.
		if(maxNumPerm_ > 0) {
			counter_++;
			if (counter_ == maxNumPerm_)
				bKeepGoing = false;
		}
		return bKeepGoing;
    }
};

void printUsage() {
	cout << "String Permutation Generator" << endl;
	cout << "Prints permutations of a specified string. Optionally limits the size of the printed output." << endl;
	cout << "Copyright (c) 2019 David Krikheli" << endl;
	cout << "Usage: str-perm-gen [input_string] [max_num_perm]"<< endl;
	cout << "\tinput_string -\tthe string to generate permutations of;" << endl;
	cout << "\tmax_num_perm -\tthe maximum number of permutations to generate. This is to help ensure" << endl;
	cout << "\t\t\tthat the process completes normally within a reasonable period of time." << endl;
}

int main (int argc, char* argv[]) {
    if(argc <= 1 || argc > 3) {
		printUsage();
        return 0;
    }

	string inputString{ argv[1] };
	std::vector<char> vocabulary(inputString.begin(), inputString.end());

	size_t maxNumPerm = 0;
	if (argc == 3)
		maxNumPerm = atoi(argv[2]);
	StringPermutationGenerator spg(maxNumPerm);

	spg.generate(vocabulary);

	return 0;
}
