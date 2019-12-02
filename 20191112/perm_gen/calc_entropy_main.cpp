/* calc_entropy.cpp
The main entry point of the entropy calculator application.

Copyright(c) 2019 David Krikheli

Modification history:
    22/Nov/2019 - David Krikheli created the module.
*/

//#include <chrono>
//#include <map>
#include <vector>
#include <atomic>
#include <thread>

#include <iostream>
#include <fstream>
#include <utility>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "calc_entropy_cli.hpp"

using namespace std;
using namespace dk;

typedef std::vector<size_t> CECounterType;

static void processData(const ECCLIParser& parser, const CECounterType& counter) noexcept {

	// Ascertain the total size of the dataset.
	size_t iTotalSize{ 0 };
	iTotalSize = std::accumulate(counter.begin(), counter.end(), size_t(0),
		[](const size_t previous, const auto& element) { return previous + element; });

	// Ascertain the log base.
	size_t iDistinctCount{ 0 };
	for (auto elem : counter)
		if (elem)
			iDistinctCount++;
	double dblLogBase = parser.logBase();
	if (dblLogBase == 0.0)
		dblLogBase = double(iDistinctCount);

	double dblTotalSize{ double(iTotalSize) };
	double dblEntropy{ 0.0 };

	if (dblLogBase != 1.0) {	// Avoid division by zero.
		double p{ 0.0 };
		double logP{ 0.0 };
		for (auto elem : counter) {
			if (elem == 0)
				continue;
			p = double(elem) / dblTotalSize;

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
			[](const auto& a, const auto& b)->bool { return a < b; });

		size_t iMax{ *best };
		int iMaxNumDigits = int(ceil(log10(iMax)));

		std::cout << '\t' << "---------------" << '\n';
		std::cout << '\t' << "Frequency Table" << '\n';
		std::cout << '\t' << "---------------" << '\n';
		if (parser.binary() || parser.numericFormat()) {
			for (const auto& elem : counter)
				if (elem)
					std::cout << '\t' << setw(3) << int(&elem - &counter[0]) << " - " << setw(iMaxNumDigits) << elem << '\n';
		}
		else {
			for (const auto& elem : counter)
				if (elem)
					std::cout << '\t' << setw(3) << static_cast<unsigned char>(&elem - &counter[0]) << " - " << setw(iMaxNumDigits) << elem << '\n';
		}
	}
}

const size_t iInputDataBufferSize{ 8192 };
struct InputDataBuffer {
	char cBuffer[iInputDataBufferSize];

	size_t iInputSize{ 0 };
	bool bNotEOF{ true };

	std::atomic<bool> abBufferReady{ false };
};
const size_t iNumBuffers{ 2 };
InputDataBuffer buffers[iNumBuffers];

void countBytes(bool bBinary, CECounterType* pCounter) {
	size_t inxBuffer{ 0 };
	bool bNotEOF{ true };

	while (bNotEOF) {
		InputDataBuffer& buffer = buffers[inxBuffer];
		while (!bool(buffer.abBufferReady))
			std::this_thread::yield();

		bNotEOF = buffer.bNotEOF;
		size_t iInputSize = buffer.iInputSize;
		char* cBuffer = buffer.cBuffer;

		unsigned char symbol{ 0 };

		if (bBinary)
			for (size_t inx = 0; inx < iInputSize; inx++) {
				symbol = static_cast<unsigned char>(cBuffer[inx]);
				(*pCounter)[symbol] ++;
			}
		else
			for (size_t inx = 0; inx < iInputSize; inx++) {
				symbol = static_cast<unsigned char>(cBuffer[inx]);
				if (isprint(symbol))
					(*pCounter)[symbol] ++;
			}

		buffer.abBufferReady.store(false);

		inxBuffer++;
		if (inxBuffer == iNumBuffers)
			inxBuffer = 0;
	}
}
static void readDataFromFile(ifstream& inputFile, bool bBinary,
							CECounterType& counter) {

	thread thr(countBytes, bBinary, &counter);

	size_t inxBuffer{ 0 };
	while (inputFile) {
		InputDataBuffer& buffer = buffers[inxBuffer];
		while (bool(buffer.abBufferReady))
			std::this_thread::yield();

		inputFile.read(buffer.cBuffer, iInputDataBufferSize);

		buffer.bNotEOF = bool(inputFile);
		buffer.iInputSize = inputFile.gcount();
		buffer.abBufferReady.store(true);

		inxBuffer++;
		if (inxBuffer == iNumBuffers)
			inxBuffer = 0;
	}

	thr.join();
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

	parser.forceThousandsSeparators(cout);

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

		CECounterType counter(256);
		readDataFromFile(inputFile, parser.binary(), counter);
		processData(parser, counter);
		inputFile.close();
	}
	else {
		while (!cin.eof()) {
			string inputString;
			getline(cin, inputString);
			if (inputString.size() > 0) {
				CECounterType counter(256);
				for (auto symbol : inputString)
					counter[static_cast<unsigned char>(symbol)] ++;
				processData(parser, counter);
			}
		}
	}

    return 0;
}
