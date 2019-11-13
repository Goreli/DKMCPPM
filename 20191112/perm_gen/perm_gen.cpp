/* perm-str-main.cpp
The main entry point of this implementation
of the String Permutation algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    12/Nov/2019 - David Krikheli created the module.
*/

#include <algorithm>
#include "perm_gen.hpp"

using namespace std;
using namespace dk;
   
StringPermutationGenerator::StringPermutationGenerator()
    : permCount_{0}, vocabularySize_{0} {
}
StringPermutationGenerator::~StringPermutationGenerator() {
}   
void StringPermutationGenerator::generate(const CharContainer& vocabulary) {
    permCount_ = 0;
    permutation_.clear();
    vocabulary_ = vocabulary;
    vocabularySize_ = distance(vocabulary_.begin(), vocabulary_.end());

    generate_();
}
bool StringPermutationGenerator::generate_() {
    // If the call stack has hit the bottom of the recursion tree then
    // process the permutation and move on to the next recursion cycle.
    if(vocabularySize_ == 0) {
        permCount_++;
        return process(permutation_);
    }

    bool bContinue { true };

    char letter = *(vocabulary_.begin());
    permutation_.push_back(letter);
    vocabulary_.erase_after(vocabulary_.before_begin());
    --vocabularySize_;
    bContinue = generate_();
    vocabulary_.insert_after(vocabulary_.before_begin(), letter);
    permutation_.pop_back();
    vocabularySize_++;

    auto it1 = vocabulary_.begin();
    auto it2 = vocabulary_.begin();
    it2++;
    for(size_t inx = 1; inx < vocabularySize_ && bContinue; inx++) {
        letter = *it2;
        permutation_.push_back(letter);
        vocabulary_.erase_after(it1);
        --vocabularySize_;
        bContinue = generate_();
        vocabulary_.insert_after(it1, letter);
        permutation_.pop_back();
        vocabularySize_++;
        it1++;
        it2++;
    }
    return bContinue;
}
