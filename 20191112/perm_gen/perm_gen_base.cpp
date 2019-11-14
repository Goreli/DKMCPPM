/* perm_gen_base.cpp
This module defines the PermutationGeneratorBase class.

Copyright(c) 2019 David Krikheli

Modification history:
    12/Nov/2019 - David Krikheli created the module.
*/

#include "perm_gen_base.hpp"

using namespace std;
using namespace dk;
   
PermutationGeneratorBase::PermutationGeneratorBase() {
}
PermutationGeneratorBase::~PermutationGeneratorBase() {
}   
bool PermutationGeneratorBase::generate(const VocabularyContainer& vocabulary) {
	permutation_.clear();
	vocabulary_ = vocabulary;
    return generate_();
}
bool PermutationGeneratorBase::generate_() {
    // If the call stack has hit the bottom of the recursion tree then
    // process the permutation and move on to the next recursion cycle.
	if (vocabulary_.size() == 0)
		return process_(permutation_);

    bool bContinue { true };
	char symbol = char(0);
	size_t vocSize = vocabulary_.size();

    for(size_t inx = 0; inx < vocSize && bContinue; inx++) {
		symbol = vocabulary_[inx];
		permutation_.push_back(symbol);
		vocabulary_.erase(vocabulary_.begin() + inx);
        bContinue = generate_();
        vocabulary_.insert(vocabulary_.begin()+inx, symbol);
		permutation_.pop_back();
	}
    return bContinue;
}
