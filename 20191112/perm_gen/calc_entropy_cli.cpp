/* calc_entropy_cli.cpp
Defines the command line parser of the Entropy Calculator application.

Copyright(c) 2019 David Krikheli

Modification history:
     1/Dec/2019 - David Krikheli created the module.
*/

#include "calc_entropy_cli.hpp"

using namespace std;
using namespace dk;

ECCLIParser::ECCLIParser(int argc, char** argv)
	: CLIParserBase(argc, argv), strFilePath_{ "" }, bBinary_{ false },
	bFrequencyTable_{ false }, bNumericFormat_{ true }, bHelp_{ false },
	dblLogBase_{ 0.0 }
{}

void ECCLIParser::parse() {
	if (_argc > 7)
		throw CLIParserException(string("Too many CLI options: ") + to_string(_argc));

	for (_inxArg = 1; _inxArg < _argc; _inxArg++) {
		string strOption = string(_argv[_inxArg]);

		if (strOption[0] == '-') {
			if (_boolOption("b", bBinary_))
				continue;
			if (_threeStateOption("ft", bFrequencyTable_, "n", "t", bNumericFormat_))
				continue;
			if (_boolOption("h", bHelp_))
				continue;
			if (_boolOption("-help", bHelp_))
				continue;
			if (_strOption("i", strFilePath_)) {
				continue;
			}
			if (_doubleOption("l", dblLogBase_)) {
				if (dblLogBase_ == 1.0)
					throw CLIParserException(string("Unacceptable log base in CLI option -") + strOption[1] + '.');
				continue;
			}
			throw CLIParserException(string("Unknown CLI option ") + strOption + '.');
		}
		string strErrMsg = "CLI options should start with \'-\'. Invalid argument #";
		strErrMsg += to_string(_inxArg + 1) + ": " + strOption + ".";
		throw CLIParserException(strErrMsg);
	}	// for

	if (bBinary_ && !bNumericFormat_)
		throw CLIParserException("Incompatible \"-b\" and \"-f t\" options. Unable to output non-printable binary bytes as text.");
	if (bBinary_ && getFilePath().size() == 0)
		throw CLIParserException("Standard input incompatible with option \"-b\"");
}
void ECCLIParser::printUsage() noexcept {
	cout << "Bytewise Entropy Calculator v1.0" << '\n';
	cout << "Prints entropy of input data. Optionally prints the byte frequency table." << '\n';
	cout << "Copyright (c) 2019 David Krikheli" << '\n';
	cout << "Refer the following link for comprehensive help information:" << '\n';
	cout << " " << "https://github.com/Goreli/DKMCPPM/blob/master/20191112/perm_gen/readme.md" << '\n';
	cout << "Usage: " << "calc-entropy [options]" << '\n';
	cout << " -b         - binary. Include non-printable bytes. For file input only;" << '\n';
	cout << " -ft  param - frequency table; param: (n)umeric or (t)ext format;" << '\n';
	cout << " -h         - print help info. Same as --help. Overrides all other options;" << '\n';
	cout << " -i   path  - file to calculate entropy of. Otherwise read standard input;" << '\n';
	cout << " -l   base  - log base. Default - the number of distinct characters." << '\n';
}
const string& ECCLIParser::getFilePath() const noexcept {
	return strFilePath_; 
}
bool ECCLIParser::binary() const noexcept {
	return bBinary_; 
}
bool ECCLIParser::frequencyTable() const noexcept {
	return bFrequencyTable_; 
}
bool ECCLIParser::numericFormat() const noexcept {
	return bNumericFormat_; 
}
bool ECCLIParser::help() const noexcept {
	return bHelp_; 
}
double ECCLIParser::logBase() const noexcept {
	return dblLogBase_; 
}
