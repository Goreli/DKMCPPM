/* str_perm_gen_b.cpp
The main entry point of the String Permutation benchmarking application.

Copyright(c) 2019 David Krikheli

Modification history:
    13/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <string>
#include <utility>
#include <chrono>
#include <locale>
#include "cli_parser_base.hpp"
#include "perm_gen_base.hpp"

using namespace std;
using namespace dk;

class BCLIParser : public dk::CLIParserBase {
public:
	BCLIParser(int argc, char* argv[]) : CLIParserBase(argc, argv), strInput_{ "" }, iTaskRepeatCount_{ 1 }, bHelp_{ false } {}
	bool parse() {
		if (_argc < 2)
			return false;
		if (_argc > 5)
			throw CLIParserException(string("Too many CLI options: ") + to_string(_argc));

		strInput_ = string(_argv[1]);
		for (_inxArg = 2; _inxArg < _argc; _inxArg++) {
			string strOption = string(_argv[_inxArg]);

			if (strOption[0] == '-') {
				if (_uintOption('t', iTaskRepeatCount_))
					continue;
				if (_boolOption('h', bHelp_))
					continue;
				throw CLIParserException(string("Unknown CLI option -") + strOption[1] + '.');
			}
			string strErrMsg = "CLI options should start with -. Invalid argument #";
			strErrMsg += to_string(_inxArg + 1) + ": " + strOption + ".";
			throw CLIParserException(strErrMsg);
		}	// for
		return true;
	}
	static void printUsage() noexcept {
		cout << "Usage: str-perm-gen-b [input_string] [options]" << '\n';
		cout << "  -h        - print help information. Overrides all other options;" << '\n';
		cout << "  -t repeat - dry-run 'repeat' times and print average duration." << '\n';
	}

	const std::string& getInputString() const noexcept { return strInput_; };
	size_t getTaskRepeatCount() const noexcept { return iTaskRepeatCount_; };
	bool help() const noexcept { return bHelp_; };
private:
	std::string strInput_;
	size_t iTaskRepeatCount_;
	bool bHelp_;
};

class StringPermutationGenerator : public PermutationGeneratorBase<char> {
public:
	void resetPermutationCounter() { permutationCounter_ = 0; }
	size_t getPermutationCounter() { return permutationCounter_; }
private:
	virtual void process_(const std::vector<char>& permutation) {
		permutationCounter_++;
	}
	size_t permutationCounter_{ 0 };
};

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
	BCLIParser parser(argc, argv);
	try {
		// Print usage instructions if there is nothing to do or help has been requested.
		if (!parser.parse() || parser.help()) {
			cout << "String Permutation Generator Benchmarking Application v1.0" << '\n';
			cout << "Dry-runs the permutation engine a specified number of times" << '\n';
			cout << "and reports the average duration." << '\n';
			cout << "Copyright (c) 2019 David Krikheli" << '\n';
			cout << "Refer the following link for comprehensive help information:" << '\n';
			cout << "  " << "https://github.com/Goreli/DKMCPPM/blob/master/20191112/perm_gen/readme.md" << '\n';
			parser.printUsage();
			return 0;
		}
	}
	catch (const CLIParserException & e)
	{
		// Use ANSI escape characters to print the error message in white on red.
		cerr << "\033[3;41;37m" << "str-perm-gen-b error: " << e.what() << "\033[0m" << '\n';
		parser.printUsage();
		return 1;
	}

	std::vector<char> vocabulary(parser.getInputString().begin(), parser.getInputString().end());
	StringPermutationGenerator spg;
	std::chrono::duration<double> totalElapsed(0.0);

	forceThousandsSeparators();

	for (size_t cycleCounter = 0; cycleCounter < parser.getTaskRepeatCount(); cycleCounter++) {
		cout << "Test cycle #" << cycleCounter+1 << endl;

		cout << "\tRunning the permutation generator for \"" << parser.getInputString() << "\"." << '\n';
		spg.resetPermutationCounter();
		auto start = std::chrono::high_resolution_clock::now();
		spg.generate(vocabulary, true, 0);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		cout << "\t" << spg.getPermutationCounter() << " permutations generated in " << elapsed.count() << " seconds." << '\n';
		totalElapsed += elapsed;
		cout << '\n';
	}

	cout << "Average duration in seconds: " << totalElapsed.count() / parser.getTaskRepeatCount() << '\n' << '\n';

    return 0;
}
