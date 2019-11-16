/* str_perm_gen.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <fstream>
#include <iostream>
#include <string>
#include "perm_gen_base.hpp"
#include "str_perm_gen_cli.hpp"

using namespace std;
using namespace dk;

class StringPermutationGenerator : public PermutationGeneratorBase<char> {
public:
	StringPermutationGenerator(size_t iStartNum, size_t iCount, bool bPrintNumbers,
		ostream& outStream) 
	: iStartNum_(iStartNum), iCount_{ iCount }, bPrintNumbers_{ bPrintNumbers },
		iCounter_{ 0 }, iTop_{ 0 }, outStream_{ outStream }
	{
		iTop_ = iStartNum_ + iCount_;
	}
private:
	size_t iStartNum_;
	size_t iCount_;
	bool bPrintNumbers_;
	size_t iCounter_;
	size_t iTop_;
	ostream& outStream_;

    virtual bool process_(const vector<char>& permutation) {
		iCounter_++;

		// Too early to start printing?
		if (iCounter_ < iStartNum_)
			// Yes. Get the next permutation.
			return true;

		// Should print this one?
		if (iCount_ == 0 || iCounter_ < iTop_) {
			// Yes. Keep printing.
			if (bPrintNumbers_)
				outStream_ << iCounter_ << " ";

			// Convert the permutation format from the vector into string.
			std::string strPermutation(permutation.begin(), permutation.end());
			outStream_ << strPermutation << '\n';
			return true;
		}

		// Finished printing the expected number of permutations. Stop the permutation generator.
		return false;
    }
};

int main (int argc, char* argv[]) {
	StrPermGenCLI parser;
	if(!parser.parse(argc, argv)) {
		cout << "String Permutation Generator v1.0" << '\n';
		cout << "Prints permutations of a specified string. Optionally limits the size of the printed output." << '\n';
		cout << "Copyright (c) 2019 David Krikheli" << '\n';
		parser.printUsage();
        return 0;
    }

	const string& inputString = parser.getInputString();
	std::vector<char> vocabulary(inputString.begin(), inputString.end());

	ofstream fout;
	bool bUseOutputFile = parser.getOutFilePath().size() ? true : false;
	if (bUseOutputFile)
	{
		fout.open(parser.getOutFilePath());
		if (!fout) {
			cerr << "Error: unable to open the output file. Exiting with error code 1." << '\n';
			return 1;
		}
	}

	StringPermutationGenerator spg(
		parser.getStartNumber(),
		parser.getCount(),
		parser.printPermutationNumbers(),
		bUseOutputFile? fout : cout
	);

	spg.generate(vocabulary);

	if(bUseOutputFile)
		fout.close();
	return 0;
}
