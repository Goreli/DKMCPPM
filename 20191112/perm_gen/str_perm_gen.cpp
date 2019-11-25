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
#include <chrono>
#include "str_perm_gen.hpp"

using namespace std;
using namespace dk;

PermutationGeneratorStopSignal::PermutationGeneratorStopSignal()
	: exception() {}

StringPermutationGenerator::StringPermutationGenerator(size_t iStartNum, 
	size_t iPrintCount, bool bPrintNumbers, ostream& outStream)
: iStartNum_(iStartNum), iPrintCount_{ iPrintCount }, bPrintNumbers_{ bPrintNumbers },
outStream_{ outStream }, bUseCLIRegex_{ false }, bExclusionRegex_{ false },
objRegex_(), iPermutationNumber_{ 0 }, iPrintCounter_{ 0 },
iGroupSize_{ 0 }, iIntraGroupCounter_{ 0 }, iNextInGroup_{ 0 }, dist_(), bSilent_(false)
{
}
void StringPermutationGenerator::assignRegex(const string& strRegex, bool bExclusionRegex) noexcept {
	bUseCLIRegex_ = true;
	objRegex_.assign(strRegex);
	bExclusionRegex_ = bExclusionRegex;
}
void StringPermutationGenerator::setGroupSize(size_t iGroupSize) noexcept {
	if (iGroupSize < 2) {
		iGroupSize_ = 0;
		return;
	}

	iGroupSize_ = iGroupSize;
	dist_.param(std::uniform_int_distribution<size_t>::param_type(1, iGroupSize));
	iNextInGroup_ = dist_(_randNumGen);
}
void StringPermutationGenerator::setSilent() noexcept {
	bSilent_ = true;
}
inline bool StringPermutationGenerator::checkWithRegex_(const string& strPermutation) noexcept {
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
void StringPermutationGenerator::process_(const vector<char>& permutation) {
	iPermutationNumber_++;

	// Too early to start printing?
	if (iPermutationNumber_ < iStartNum_)
		// Yes. Get the next permutation.
		return;

	// Already printed all the required permutations?
	if (0 < iPrintCount_ && iPrintCount_ <= iPrintCounter_)
		// Yes. Ok to stop the permutation generator.
		// Make sure to catch this exception in the piece of code that started the
		// permutation generator using either generate(....) or generate_l(....).
		throw PermutationGeneratorStopSignal();

	// There are more permutations to print.
	// Convert the permutation from the generic vector format to string.
	std::string strPermutation(permutation.begin(), permutation.end());

	// Apply the regex filter if so requested by the user.
	if (bUseCLIRegex_ && !checkWithRegex_(strPermutation))
		return;

	// Check if the random selection from groups is expected
	// to happen and make it happen if it is.
	if (iGroupSize_) {
		iIntraGroupCounter_++;

		bool bSkip = true;
		if (iIntraGroupCounter_ == iNextInGroup_)
			bSkip = false;

		if (iIntraGroupCounter_ == iGroupSize_) {
			iNextInGroup_ = dist_(_randNumGen);
			iIntraGroupCounter_ = 0;
		}

		if (bSkip)
			return;
	}

	// Print at last and remember to update the counter of printed permutations.
	if (!bSilent_) {
		if (bPrintNumbers_)
			outStream_ << iPermutationNumber_ << " " << strPermutation << '\n';
		else
			outStream_ << strPermutation << '\n';
	}
	iPrintCounter_++;
}
size_t StringPermutationGenerator::getPermutationCount() noexcept {
	return iPrintCounter_;
}