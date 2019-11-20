/* str_perm_gen.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include "str_perm_gen.hpp"
#include "str_perm_gen_cli.hpp"

using namespace std;
using namespace dk;

int execUserTask(const StrPermGenCLI& parser, size_t& iCount)
{
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
		bUseOutputFile ? fout : cout
	);

	if (parser.getRegexStr().size())
		spg.assignRegex(parser.getRegexStr(), parser.isExclusionRegex());
	if (parser.getTaskRepeatCount() > 1)
		spg.setSilent();
	spg.setGroupSize(parser.getGroupSize());

	try {
		if (parser.lexicographic())
			spg.generate_l(vocabulary, parser.ascending());
		else
			spg.generate(vocabulary, parser.allowDups());
	}
	catch (const PermutationGeneratorStopSignal&) {
		// The user defined override of process_(....) must have requested
		// to stop the permutation generator. All good. Do nothing.
	}
	iCount = spg.getPermutationCount();

	if (bUseOutputFile)
	fout.close();
	return 0;
}

// Make sure integrals are printed with thousands separators included.
struct separate_thousands : std::numpunct<char> {
	char_type do_thousands_sep() const override { return ','; }  // separate with commas
	string_type do_grouping() const override { return "\3"; } // groups of 3 digit
};
void forceThousandsSeparators() {
	auto thousands = std::make_unique<separate_thousands>();
	std::cout.imbue(std::locale(std::cout.getloc(), thousands.release()));
}

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

	size_t iCount;
	if(parser.getTaskRepeatCount() == 1)
		return execUserTask(parser, iCount);

	int iReturnValue{ 0 };
	std::chrono::duration<double> totalElapsed(0.0);
	forceThousandsSeparators();

	for (size_t inx = 0; inx < parser.getTaskRepeatCount(); inx++) {
		auto start = std::chrono::high_resolution_clock::now();
		iReturnValue = execUserTask(parser, iCount);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		totalElapsed += elapsed;
		cout << "Test run #" << inx+1 << " generated " << iCount << " permutations in " << elapsed.count() << " seconds." << '\n';
	}

	cout << '\t' << "Average duration per test run in seconds: " << totalElapsed.count() / parser.getTaskRepeatCount() << '\n' << '\n';

	return iReturnValue;
}
