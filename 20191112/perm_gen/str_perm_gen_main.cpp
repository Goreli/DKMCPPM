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

int execUserTask(const StrPermGenCLIParser& parser, size_t& iCount)
{
	const string& inputString = parser.getInputString();
	std::vector<char> symbolPool(inputString.begin(), inputString.end());
	if (parser.presort()) {
		if (parser.preOrderAscending())
			sort(symbolPool.begin(), symbolPool.end());
		else
			sort(symbolPool.rbegin(), symbolPool.rend());
	}

	ofstream fout;
	bool bUseOutputFile = parser.getOutFilePathStr().size() ? true : false;
	if (bUseOutputFile)
	{
		fout.open(parser.getOutFilePathStr());
		if (!fout) {
			cerr << "Error: unable to open the output file. Exiting with error code 1." << '\n';
			return 10;
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
	if (parser.dryRun())
		spg.setSilent();
	spg.setGroupSize(parser.getGroupSize());

	try {
		if (parser.lexicographic())
			spg.generate_l(symbolPool, parser.lexOrderAscending());
		else
			spg.generate(symbolPool, parser.allowDups(), parser.getRandPermAlgId());
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

int main (int argc, char* argv[]) {
	StrPermGenCLIParser parser(argc, argv);
	try { 
		// Print usage instructions if there is nothing to do or help has been requested.
		if (!parser.parse() || parser.help()) {
			cout << "String Permutation Generator v1.0" << '\n';
			cout << "Copyright (c) 2019 David Krikheli" << '\n';
			cout << "Refer the following link for comprehensive help information:" << '\n';
			cout << "  " << "https://github.com/Goreli/DKMCPPM/blob/master/20191112/perm_gen/readme.md" << '\n';
			parser.printUsage();
			return 0;
		}
	}
	catch(const CLIParserException& e)
	{
		parser.printErrMsg(string("str-perm-gen error: ") + e.what());
		parser.printUsage();
		return 1;
    }

	size_t iCount;
	if(!parser.dryRun())
		return execUserTask(parser, iCount);

	int iReturnValue{ 0 };
	std::chrono::duration<double> totalElapsed(0.0);
	parser.forceThousandsSeparators();

	for (size_t inx = 0; inx < parser.getTaskRepeatCount(); inx++) {
		auto start = std::chrono::high_resolution_clock::now();
		iReturnValue = execUserTask(parser, iCount);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		totalElapsed += elapsed;
		cout << "Test run #" << inx+1 << " generated " << iCount << " permutations in " << elapsed.count() << " seconds." << '\n';
	}

	cout << '\t' << "Average duration: " << totalElapsed.count() / parser.getTaskRepeatCount() << '\n' << '\n';

	return iReturnValue;
}
