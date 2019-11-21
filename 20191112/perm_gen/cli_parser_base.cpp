/* cli_parser_base.cpp
Defines a base class for command line parsers.

Copyright(c) 2019 David Krikheli

Modification history:
    22/Nov/2019 - David Krikheli created the module.
*/

#include <sstream>
#include "cli_parser_base.hpp"

using namespace std;
using namespace dk;

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
size_t CLIParserBase::_str2_size_t(const string& strNum) noexcept {
	std::istringstream iss(strNum);
	size_t iNum{ 0 };
	iss >> iNum;
	return iNum;
}
void CLIParserBase::_advanceAndCheckMissingValue() {
	char option = _argv[_inxArg][1];
	_inxArg++;
	if (_inxArg == _argc)
		throw CLIParserException(string("Missing value in CLI option -") + option + '.');
}
bool CLIParserBase::_uintOption(char symbol, size_t& iValue) {
	if (_argv[_inxArg][1] == symbol) {
		_advanceAndCheckMissingValue();

		if (!isdigit(_argv[_inxArg][0]))
			throw CLIParserException(string("Require numeric argument in CLI option -") + symbol + '.');

		iValue = CLIParserBase::_str2_size_t(_argv[_inxArg]);
		if (iValue == 0)
			throw CLIParserException(string("Require non-zero argument in CLI option -") + symbol + '.');

		return true;
	}
	return false;
}
bool CLIParserBase::_boolOption(char symbol, bool& bValue) noexcept {
	if (_argv[_inxArg][1] == symbol) {
		bValue = true;
		return true;
	}
	return false;
}
bool CLIParserBase::_strOption(char symbol, string& strValue) {
	if (_argv[_inxArg][1] == symbol) {
		_advanceAndCheckMissingValue();
		if (strValue.size() > 0)
			throw CLIParserException(string("Redefinition of string value in CLI option -") + symbol + '.');
		strValue = string(_argv[_inxArg]);
		return true;
	}
	return false;
}
bool CLIParserBase::_fourStateOption(char sym1, bool& bFirst, char sym2, char sym3, bool& bSecond) {
	if (_boolOption(sym1, bFirst)) {
		_advanceAndCheckMissingValue();
		if (_argv[_inxArg][0] != sym2 && _argv[_inxArg][0] != sym3)
			throw CLIParserException(string("Invalid value in CLI option -") + sym1 + '.');
		bSecond = (_argv[_inxArg][0] == sym2) ? true : false;
		return true;
	}
	return false;
}
