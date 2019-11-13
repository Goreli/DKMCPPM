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
    : permCount_{0} {
}
StringPermutationGenerator::~StringPermutationGenerator() {
}   
void StringPermutationGenerator::generate(const CharContainer& vocabulary) {
    permCount_ = 0;
    permutation_.clear();
    vocabulary_ = vocabulary;

    generate_();
}
bool StringPermutationGenerator::generate_() {
    // If the call stack has hit the bottom of the recursion tree then
    // process the permutation and move on to the next recursion cycle.
    if(vocabulary_.size() == 0) {
        permCount_++;
        return process(permutation_);
    }

    bool bContinue { true };
    for(size_t inx = 0; inx < vocabulary_.size() && bContinue; inx++) {
        char letter = vocabulary_[inx];
        permutation_.push_back(letter);
        vocabulary_.erase(vocabulary_.begin() + inx);
        bContinue = generate_();
        vocabulary_.insert(vocabulary_.begin()+inx, letter);
        permutation_.pop_back();
    }
    return bContinue;
}
