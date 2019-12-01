/* calc_entropy.cpp
The main entry point of the entropy calculator application.

Copyright(c) 2019 David Krikheli

Modification history:
    22/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "calc_entropy_cli.hpp"

using namespace std;
using namespace dk;

static void processData(const ECCLIParser& parser, const std::map<char, size_t>& counter) noexcept {
	// Ascertain the total size of the dataset.
	size_t iTotalSize{ 0 };
	iTotalSize = std::accumulate(counter.begin(), counter.end(), size_t(0),
		[](const size_t previous, const auto& element) { return previous + element.second; });

	// Ascertain the log base.
	size_t iDistinctCount = counter.size();
	double dblLogBase = parser.logBase();
	if (dblLogBase == 0.0)
		dblLogBase = double(iDistinctCount);

	double dblTotalSize{ double(iTotalSize) };
	double dblEntropy{ 0.0 };

	if (dblLogBase != 1.0) {	// Avoid division by zero.
		double p{ 0.0 };
		double logP{ 0.0 };
		for (auto elem : counter) {
			p = double(elem.second) / dblTotalSize;
			logP = log2(p);
			// Convert the log base if the user requested to do so.
			if (dblLogBase != 2.0)
				logP /= log2(dblLogBase);
			// Update the entropy figure.
			dblEntropy -= p * logP;
		}
	}

	cout << "Entropy:        " << dblEntropy << '\n';
	cout << "Logarithm base: " << dblLogBase << '\n';
	cout << "Distinct bytes: " << iDistinctCount << '\n';
	cout << "Total count:    " << iTotalSize << '\n';

	if (parser.frequencyTable()) {
		// Get the max frequency.
		auto best = std::max_element(counter.begin(), counter.end(),
			[](const auto& a, const auto& b)->bool { return a.second < b.second; });
		size_t iMax{ best->second };
		int iMaxNumDigits = int(ceil(log10(iMax)));

		std::cout << '\t' << "---------------" << '\n';
		std::cout << '\t' << "Frequency Table" << '\n';
		std::cout << '\t' << "---------------" << '\n';
		if (parser.binary() || parser.numericFormat())
			for (const auto& [k, v] : counter)
				std::cout << '\t' << setw(3) << int(k) << " - " << setw(iMaxNumDigits) << v << '\n';
		else
			for (const auto& [k, v] : counter)
				std::cout << '\t' << setw(3) << k << " - " << setw(iMaxNumDigits) << v << '\n';
	}
}

static void readDataFromFile(ifstream& inputFile, const ECCLIParser& parser,
							std::map<char, size_t>& counter) {
	// Count the characters in the input file.
	char inputBuffer[8192];
	while (inputFile) {
		inputFile.read(inputBuffer, sizeof(inputBuffer));
		size_t iInputSize = inputFile.gcount();
		char symbol{ 0 };
		for (size_t inx = 0; inx < iInputSize; inx++) {
			symbol = inputBuffer[inx];
			if (parser.binary() || isprint(symbol))
				counter[symbol] ++;
		}
	}
}

int main (int argc, char* argv[]) {
	ECCLIParser parser(argc, argv);
	try {
		parser.parse();
	}
	catch (const CLIParserException & e)
	{
		parser.printErrMsg(string("calc-entropy error: ") + e.what());
		parser.printUsage();
		return 1;
	}

	// Print usage instructions and exit if help has been requested
	// or there is nothing to do.
	if (parser.help()) {
		parser.printUsage();
		return 0;
	}

	if (parser.getFilePath().size()) {
		ifstream inputFile;
		inputFile.open(parser.getFilePath(), ios::binary);
		if (!inputFile) {
			string strErrMsg = "file-entropy error: unable to open the input file \""
				+ parser.getFilePath() + "\"";
			parser.printErrMsg(strErrMsg);
			parser.printUsage();
			return 2;
		}

		std::map<char, size_t> counter;
		readDataFromFile(inputFile, parser, counter);
		processData(parser, counter);
		inputFile.close();
	}
	else {
		while (!cin.eof()) {
			string inputString;
			getline(cin, inputString);
			if (inputString.size() > 0) {
				std::map<char, size_t> counter;
				for (auto symbol : inputString)
					counter[symbol] ++;
				processData(parser, counter);
			}
		}
	}

    return 0;
}
