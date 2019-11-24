/* file_entropy.cpp
The main entry point of the file entropy calculation application.

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
#include "cli_parser_base.hpp"

using namespace std;
using namespace dk;

class FECLIParser : public dk::CLIParserBase {
public:
	FECLIParser(int argc, char* argv[])
		: CLIParserBase(argc, argv), strFilePath_{ "" }, bBinary_{ false },
		bFrequencyTable_{ false }, bNumericFormat_{ true }, bHelp_{ false },
		dblLogBase_{ 0.0 }
	{}
	bool parse() {
		if (_argc < 2)
			return false;
		if (_argc > 7)
			throw CLIParserException(string("Too many CLI options: ") + to_string(_argc));

		strFilePath_ = string(_argv[1]);
		for (_inxArg = 2; _inxArg < _argc; _inxArg++) {
			string strOption = string(_argv[_inxArg]);

			if (strOption[0] == '-') {
				if (_boolOption("b", bBinary_))
					continue;
				if (_threeStateOption("f", bFrequencyTable_, "n", "t", bNumericFormat_))
					continue;
				if (_boolOption("h", bHelp_))
					continue;
				if (_doubleOption("l", dblLogBase_)) {
					if(dblLogBase_ == 1.0)
						throw CLIParserException(string("Unacceptable log base in CLI option -") + strOption[1] + '.');
					continue;
				}
				throw CLIParserException(string("Unknown CLI option -") + strOption[1] + '.');
			}
			string strErrMsg = "CLI options should start with \'-\'. Invalid argument #";
			strErrMsg += to_string(_inxArg + 1) + ": " + strOption + ".";
			throw CLIParserException(strErrMsg);
		}	// for

		if(bBinary_ && !bNumericFormat_)
			throw CLIParserException("Unable to output non-printable binary bytes as text.");

		return true;
	}
	static void printUsage() noexcept {
		cout << "Usage: file-entropy [file_path] [options]" << '\n';
		cout << "  -b        - binary. Process non-printable bytes as well;" << '\n';
		cout << "  -f param  - frequency table; param: (n)umeric or (t)ext format;" << '\n';
		cout << "  -h        - print help information. Overrides all other options;" << '\n';
		cout << "  -l base   - log base. Default - the number of distinct characters." << '\n';
	}

	const std::string& getFilePath() const noexcept { return strFilePath_; };
	bool binary() const noexcept { return bBinary_; };
	bool frequencyTable() const noexcept { return bFrequencyTable_; };
	bool numericFormat() const noexcept { return bNumericFormat_; };
	bool help() const noexcept { return bHelp_; };
	double logBase() const noexcept { return dblLogBase_; };
private:
	std::string strFilePath_;
	bool bBinary_;
	bool bFrequencyTable_;
	bool bNumericFormat_;
	bool bHelp_;
	double dblLogBase_;
};

int main (int argc, char* argv[]) {
	FECLIParser parser(argc, argv);
	try {
		// Print usage instructions if there is nothing to do or help has been requested.
		if (!parser.parse() || parser.help()) {
			cout << "File Bytewise Entropy Calculator v1.0" << '\n';
			cout << "Prints entropy of a specified file. Optionally prints the byte frequency table." << '\n';
			cout << "Copyright (c) 2019 David Krikheli" << '\n';
			cout << "Refer the following link for comprehensive help information:" << '\n';
			cout << "  " << "https://github.com/Goreli/DKMCPPM/blob/master/20191112/perm_gen/readme.md" << '\n';
			parser.printUsage();
			return 0;
		}
	}
	catch (const CLIParserException & e)
	{
		parser.printErrMsg(string("file-entropy error: ") + e.what());
		parser.printUsage();
		return 1;
	}

	ifstream inputFile;
	inputFile.open(parser.getFilePath(), ios::binary);
	if (!inputFile	) {
		string strErrMsg = "file-entropy error: unable to open the input file \""
							+ parser.getFilePath() + "\"";
		parser.printErrMsg(strErrMsg);
		parser.printUsage();
		return 2;
	}

	// Count the characters in the input file.
	std::map<char, size_t> counter;
	char inputBuffer[1024];
	while (inputFile) {
		inputFile.read(inputBuffer, sizeof(inputBuffer));
		size_t iBufferSize = inputFile.gcount();
		char symbol{ 0 };
		for (size_t inx = 0; inx < iBufferSize; inx++) {
			symbol = inputBuffer[inx];
			if (parser.binary() || isprint(symbol))
				counter[symbol] ++;
		}
	}
	inputFile.close();

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
		if(parser.binary() || parser.numericFormat())
			for (const auto& [k, v] : counter)
				std::cout << '\t' << setw(3) << int(k) << " - " << setw(iMaxNumDigits) << v << '\n';
		else
			for (const auto& [k, v] : counter)
				std::cout << '\t' << setw(3) << k << " - " << setw(iMaxNumDigits) << v << '\n';
	}

    return 0;
}
