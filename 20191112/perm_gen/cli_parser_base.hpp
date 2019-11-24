/* cli_parser_base.hpp
Declares a base class for command line parsers.

Copyright(c) 2019 David Krikheli

Modification history:
    22/Nov/2019 - David Krikheli created the module.
*/

#ifndef cli_parser_base_hpp
#define cli_parser_base_hpp

#include <string>
#include <stdexcept>

namespace dk {
	class CLIParserException : public std::invalid_argument {
	public:
		CLIParserException(const std::string&);
	};

	class CLIParserBase {
	public:
		CLIParserBase(int argc, char* argv[]);
		virtual ~CLIParserBase();

		// Color printing on Linux.
		static void printErrMsg(const std::string&);

		static void forceThousandsSeparators();

	protected:
		void _advanceAndCheckMissingValue();
		bool _strOption(char, std::string&);
		bool _uintOption(char, size_t&);
		bool _doubleOption(char, double&);
		bool _boolOption(char, bool&) noexcept;
		bool _threeStateOption(char sym1, bool& bFirst, char sym2, char sym3, bool& bSecond);
		static size_t _str2_size_t(const std::string&) noexcept;
		static double _str2_double(const std::string&) noexcept;

		int _argc;
		char** _argv;
		int _inxArg;
	};	// class CLIParserBase
};	// namespace dk
#endif //	cli_parser_base_hpp
