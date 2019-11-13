/* str_perm_gen_main.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    12/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <string>
#include <utility>
#include "perm_gen.hpp"

using namespace std;
using namespace dk;

class StringPermutationPrinter : public StringPermutationGenerator {
    virtual bool process(const std::vector<char>& permutation) {
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
    CharContainer vocabulary(strInput.begin(), strInput.end());

    StringPermutationPrinter spp;
    spp.generate(vocabulary);

    return 0;
}
