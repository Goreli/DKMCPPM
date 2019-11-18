/* str_perm_gen.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include "str_perm_gen.hpp"
#include "str_perm_gen_cli.hpp"

using namespace std;
using namespace dk;

StringPermutationGenerator::StringPermutationGenerator(size_t iStartNum, size_t iCount, bool bPrintNumbers,
	ostream& outStream) 
: iStartNum_(iStartNum), iCount_{ iCount }, bPrintNumbers_{ bPrintNumbers },
	iCounter_{ 0 }, iTop_{ 0 }, outStream_{ outStream },
	bUseCLIRegex_{ false }, bExclusionRegex_{ false }, regexObj_()
{
	iTop_ = iStartNum_ + iCount_;
}
void StringPermutationGenerator::assignRegex(const string& strRegex, bool bExclusionRegex) {
	bUseCLIRegex_ = true;
	regexObj_.assign(strRegex);
	bExclusionRegex_ = bExclusionRegex;
}

inline bool StringPermutationGenerator::checkWithRegex_(const string& strPermutation) {
	bool bMatched = regex_search(strPermutation, regexObj_, regex_constants::match_any);
	if (
		// Inclusion regex was requested and there is a matching permutation.
		(!bExclusionRegex_ && bMatched)
		||
		// Exclusion regex was requested and there is a no matching permutation.
		(bExclusionRegex_ && !bMatched)
		)
		return true;
	else
		return false;
}
bool StringPermutationGenerator::process_(const vector<char>& permutation) {
	iCounter_++;

	// Too early to start printing?
	if (iCounter_ < iStartNum_)
		// Yes. Get the next permutation.
		return true;

	// Should consider this permutation for printing?
	if (iCount_ == 0 || iCounter_ < iTop_) {
		// Yes. Consider this permutation for printing.
		// Convert the permutation format from vector to string.
		std::string strPermutation(permutation.begin(), permutation.end());

		bool bPrint{ true };
		if (bUseCLIRegex_)
			bPrint = checkWithRegex_(strPermutation);

		if (bPrint) {
			if (bPrintNumbers_)
				outStream_ << iCounter_ << " ";
			outStream_ << strPermutation << '\n';
		}

		return true;
	}

	// Finished printing all the required permutations. Stop the permutation generator.
	return false;
}
