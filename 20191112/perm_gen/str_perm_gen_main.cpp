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


int main (int argc, char* argv[]) {
	StrPermGenCLI parser;
	if(!parser.parse(argc, argv)) {
		cout << "String Permutation Generator v1.0" << '\n';
		cout << "Copyright (c) 2019 David Krikheli" << '\n';
		cout << "Refer the following link for comprehensive help information:" << '\n';
		cout << "  " << "https://github.com/Goreli/DKMCPPM/blob/master/20191112/perm_gen/readme.md" << '\n';
		parser.printUsage();
        return 0;
    }

	const string& inputString = parser.getInputString();
	std::vector<char> vocabulary(inputString.begin(), inputString.end());
	if (parser.presort()) {
		if (parser.ascending())
			sort(vocabulary.begin(), vocabulary.end());
		else
			sort(vocabulary.rbegin(), vocabulary.rend());
	}

	ofstream fout;
	bool bUseOutputFile = parser.getOutFilePathStr().size() ? true : false;
	if (bUseOutputFile)
	{
		fout.open(parser.getOutFilePathStr());
		if (!fout) {
			cerr << "Error: unable to open the output file. Exiting with error code 1." << '\n';
			return 1;
		}
	}

	StringPermutationGenerator spg(
		parser.getStartNumber(),
		parser.getPrintCount(),
		parser.printPermutationNumbers(),
		bUseOutputFile? fout : cout
	);

	if (parser.getRegexStr().size())
		spg.assignRegex(parser.getRegexStr(), parser.isExclusionRegex());

	if (parser.lexicographic())
		spg.generate_l(vocabulary, parser.forward());
	else
		spg.generate(vocabulary);

	if(bUseOutputFile)
		fout.close();
	return 0;
}
