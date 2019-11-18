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

StringPermutationGenerator::StringPermutationGenerator(size_t iStartNum, 
	size_t iPrintCount, bool bPrintNumbers, ostream& outStream)
: iStartNum_(iStartNum), iPrintCount_{ iPrintCount }, bPrintNumbers_{ bPrintNumbers },
outStream_{ outStream }, bUseCLIRegex_{ false }, bExclusionRegex_{ false },
objRegex_(), iPermutationNumber_{ 0 }, iPrintCounter_{ 0 }
{
}
void StringPermutationGenerator::assignRegex(const string& strRegex, bool bExclusionRegex) {
	bUseCLIRegex_ = true;
	objRegex_.assign(strRegex);
	bExclusionRegex_ = bExclusionRegex;
}

inline bool StringPermutationGenerator::checkWithRegex_(const string& strPermutation) {
	bool bMatched = regex_search(strPermutation, objRegex_, regex_constants::match_any);
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
	iPermutationNumber_++;

	// Too early to start printing?
	if (iPermutationNumber_ < iStartNum_)
		// Yes. Get the next permutation.
		return true;

	// Already printed all the required permutations?
	if (0 < iPrintCount_ && iPrintCount_ <= iPrintCounter_)
		// Yes. Ok to stop the permutation generator.
		return false;

	// There are more permutations to print.
	// Convert the permutation format from vector to string.
	std::string strPermutation(permutation.begin(), permutation.end());

	bool bPrint{ true };
	if (bUseCLIRegex_)
		bPrint = checkWithRegex_(strPermutation);

	if (bPrint) {
		if (bPrintNumbers_)
			outStream_ << iPermutationNumber_ << " " << strPermutation << '\n';
		else
			outStream_ << strPermutation << '\n';

		iPrintCounter_++;
	}

	return true;
}
