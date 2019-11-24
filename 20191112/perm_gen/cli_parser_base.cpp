/* cli_parser_base.cpp
Defines a base class for command line parsers.

Copyright(c) 2019 David Krikheli

Modification history:
    22/Nov/2019 - David Krikheli created the module.
*/

#ifdef _CONSOLE
// Enable ANSI escape sequences on Windows 10.
#include <windows.h>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN  0x0008
#endif

#include <sstream>
#include <iostream>
#include <locale>
#include "cli_parser_base.hpp"

using namespace std;
using namespace dk;

void CLIParserBase::printErrMsg(const string& strErrMsg) {
#ifdef _CONSOLE
	// If this is Windows 10 then use a singleton pattern to execute
	// the ANSI escape sequence enablement code only once.
	static HANDLE hStdout{ nullptr };
	if (!hStdout) {
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD l_mode;
		GetConsoleMode(hStdout, &l_mode);
		SetConsoleMode(hStdout, l_mode | 
			ENABLE_VIRTUAL_TERMINAL_PROCESSING |
			DISABLE_NEWLINE_AUTO_RETURN);
	}
#endif
	cerr << "\033[41;37m" << strErrMsg << "\033[0m" << '\n';
}
// Make sure integrals are printed with thousands separators included.
struct separate_thousands : std::numpunct<char> {
	char_type do_thousands_sep() const override { return ','; }  // separate with commas
	string_type do_grouping() const override { return "\3"; } // groups of 3 digit
};
void CLIParserBase::forceThousandsSeparators() {
	auto thousands = std::make_unique<separate_thousands>();
	std::cout.imbue(std::locale(std::cout.getloc(), thousands.release()));
}
size_t CLIParserBase::_stringto_size_t(const string& strNum) noexcept {
	std::istringstream iss(strNum);
	size_t iNum{ 0 };
	iss >> iNum;
	return iNum;
}
double CLIParserBase::_stringto_double(const string& strNum) noexcept {
	std::istringstream iss(strNum);
	double dNum{ 0.0 };
	iss >> dNum;
	return dNum;
}


CLIParserException::CLIParserException(const string& strWhat)
: invalid_argument{strWhat} {
}
CLIParserBase::CLIParserBase(int argc, char* argv[])
	: _argc{ argc }, _argv{ argv }, _inxArg{0}
{
}
CLIParserBase::~CLIParserBase()
{
}
void CLIParserBase::_advanceAndCheckMissingValue() {
	char option = _argv[_inxArg][1];
	_inxArg++;
	if (_inxArg == _argc)
		throw CLIParserException(string("Missing value in CLI option -") + option + '.');
}
bool CLIParserBase::_uintOption(const string& strOption, size_t& iValue) {
	if (string(_argv[_inxArg]+1) == strOption) {
		_advanceAndCheckMissingValue();

		if (!isdigit(_argv[_inxArg][0]))
			throw CLIParserException(string("Require numeric argument in CLI option -") + strOption + '.');

		iValue = CLIParserBase::_stringto_size_t(_argv[_inxArg]);
		if (iValue == 0)
			throw CLIParserException(string("Require non-zero argument in CLI option -") + strOption + '.');

		return true;
	}
	return false;
}
bool CLIParserBase::_doubleOption(const string& strOption, double& dValue) {
	if (string(_argv[_inxArg]+1) == strOption) {
		_advanceAndCheckMissingValue();

		if (!isdigit(_argv[_inxArg][0]))
			throw CLIParserException(string("Require numeric argument in CLI option -") + strOption + '.');

		dValue = CLIParserBase::_stringto_double(_argv[_inxArg]);
		if (dValue == 0.0)
			throw CLIParserException(string("Require non-zero argument in CLI option -") + strOption + '.');

		return true;
	}
	return false;
}
bool CLIParserBase::_boolOption(const string& strOption, bool& bValue) noexcept {
	if (string(_argv[_inxArg]+1) == strOption) {
		bValue = true;
		return true;
	}
	return false;
}
bool CLIParserBase::_strOption(const string& strOption, string& strValue) {
	if (string(_argv[_inxArg]+1) == strOption) {
		_advanceAndCheckMissingValue();
		if (strValue.size() > 0)
			throw CLIParserException(string("Redefinition of string value in CLI option -") + strOption + '.');
		strValue = string(_argv[_inxArg]);
		return true;
	}
	return false;
}
bool CLIParserBase::_threeStateOption(const string& strOption, bool& bOption, const string& strVal1, const string& strVal2, bool& bVal) {
	if (_boolOption(strOption, bOption)) {
		_advanceAndCheckMissingValue();
		if (string(_argv[_inxArg]+0) != strVal1 && string(_argv[_inxArg]+0) != strVal2)
			throw CLIParserException(string("Invalid value in CLI option -") + strOption + '.');
		bVal = (string(_argv[_inxArg]+0) == strVal1) ? true : false;
		return true;
	}
	return false;
}
