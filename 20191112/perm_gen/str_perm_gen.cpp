/* str_perm_gen.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    12/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <string>
#include <utility>
#include "perm_gen_base.hpp"

using namespace std;
using namespace dk;

class StringPermutationGenerator : public PermutationGeneratorBase {
    virtual bool process_(const PermutationContainer& permutation) {
        std::string strPermutation(permutation.begin(), permutation.end());
        cout << strPermutation << endl;
        return true;
    }
};

int main (int argc, char* argv[]) {
    if(argc <= 1) {
        cout << "String permutation generator" << endl;
        cout << "Copyright (c) 2019 David Krikheli" << endl;
        cout << "Usage: str-perm-gen [input string]" << endl;
        return 0;
    }

    string strInput {argv[1]};
    VocabularyContainer vocabulary(strInput.begin(), strInput.end());

	StringPermutationGenerator spg;
    spg.generate(vocabulary);

    return 0;
}
