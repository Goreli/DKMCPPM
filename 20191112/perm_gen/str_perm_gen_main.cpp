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

void doExecUserTask(const StrPermGenCLIParser& parser, const string& inputString, ostream& outStream, size_t& iPermCount)
{
	// An input string is available. Let's process it.
	std::vector<char> symbolPool(inputString.begin(), inputString.end());
	if (parser.presort()) {
		if (parser.presortAscending())
			sort(symbolPool.begin(), symbolPool.end());
		else
			sort(symbolPool.rbegin(), symbolPool.rend());
	}

	StringPermutationGenerator spg(
		parser.getStartNumber(),
		parser.getPrintCount(),
		parser.printPermutationNumbers(),
		outStream
	);

	spg.setSilent(parser.dryRun());
	spg.setGroupSize(parser.getGroupSize());
	if (parser.getRegexStr().size())
		spg.assignRegex(parser.getRegexStr(), parser.isExclusionRegex());

	try {
		if (parser.lexicographic())
			spg.generate_l(symbolPool, parser.lexOrderAscending());
		else
			spg.generate(symbolPool, parser.excludeDups(), parser.getRandPermAlgId());
	}
	catch (const PermutationGeneratorStopSignal&) {
		// The user defined override of process_(....) must have requested
		// to stop the permutation generator. All good. Do nothing.
	}
	iPermCount = spg.getPermutationCount();
}


void execUserTask(const StrPermGenCLIParser& parser, const string& inputString, ostream& outStream) {
	size_t iPermCount{ 0 };

	if (parser.dryRun()) {
		std::chrono::duration<double> totalElapsed(0.0);
		for (size_t inx = 0; inx < parser.getTaskRepeatCount(); inx++) {
			auto start = std::chrono::high_resolution_clock::now();
			doExecUserTask(parser, inputString, outStream, iPermCount);
			auto finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = finish - start;
			totalElapsed += elapsed;
			outStream << "Test run #" << inx + 1 << " generated " << iPermCount << " permutations in " << elapsed.count() << " seconds." << '\n';
		}

		outStream << '\t' << "Average duration: " << totalElapsed.count() / parser.getTaskRepeatCount() << '\n' << '\n';
	}
	else
		doExecUserTask(parser, inputString, outStream, iPermCount);
}

int main (int argc, char* argv[]) {
	StrPermGenCLIParser parser(argc, argv);
	try { 
		parser.parse();
	}
	catch(const CLIParserException& e)
	{
		parser.printErrMsg(string("str-perm-gen error: ") + e.what());
		parser.printUsage();
		return 1;
    }

	// Print usage instructions and exit if help has been requested
	// or there is nothing to do.
	if (parser.help()) {
		parser.printUsage();
		return 0;
	}

	ofstream fout;
	bool bUseOutputFile = (parser.getOutFilePathStr().size() > 0);
	if (bUseOutputFile) {
		fout.open(parser.getOutFilePathStr());
		if (!fout) {
			parser.printErrMsg("str-perm-gen error: unable to open the output file " + parser.getOutFilePathStr() + ".");
			parser.printUsage();
			return 2;
		}
	}

	if (parser.dryRun())
		parser.forceThousandsSeparators(bUseOutputFile ? fout : cout);

	if(parser.getInputString().size() > 0)
		execUserTask(parser, parser.getInputString(), bUseOutputFile ? fout : cout);
	else {
		while (!cin.eof()) {
			string inputString;
			getline(cin, inputString);
			if(inputString.size()>0)
				execUserTask(parser, inputString, bUseOutputFile ? fout : cout);
		}
	}

	if (bUseOutputFile)
		fout.close();
	return 0;
}
