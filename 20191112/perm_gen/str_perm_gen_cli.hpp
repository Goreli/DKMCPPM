/* str_perm_gen_cli.hpp
Declares the StrPermGenCLI command line parser class of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    16/Nov/2019 - David Krikheli created the module.
*/

#ifndef str_perm_gen_cli_hpp
#define str_perm_gen_cli_hpp

#include <string>
#include <stdexcept>

class StrPermGenCLIParserException : public std::invalid_argument {
public: 
	StrPermGenCLIParserException(const std::string&);
};

class StrPermGenCLIParser {
public:
	StrPermGenCLIParser();
	bool parse(int argc, char* argv[]);
	void printUsage() const noexcept;

	const std::string& getInputString() const noexcept;
	size_t getStartNumber() const noexcept;
	size_t getPrintCount() const noexcept;
	bool printPermutationNumbers() const noexcept;
	const std::string& getOutFilePathStr() const noexcept;

	bool isExclusionRegex() const noexcept;
	const std::string& getRegexStr() const noexcept;

	bool presort() const noexcept;
	bool preOrderAscending() const noexcept;

	bool lexicographic() const noexcept;
	bool lexOrderAscending() const noexcept;

	bool allowDups() const noexcept;

	size_t getGroupSize() const noexcept;

	size_t getTaskRepeatCount() const noexcept;
	bool dryRun() const noexcept;

	bool help() const noexcept;

	//size_t getRandPermEngId() const noexcept;

private:
	int argc_;
	char** argv_;
	int inxArg_;

	std::string strInput_;
	size_t iStartNum_;
	size_t iPrintCount_;
	bool bPrintNumbers_;
	std::string strOutFile_;

	// Command line regex.
	bool bExclusionRegex_;
	std::string strRegex_;

	bool bPresort_;
	bool bPreOrderAscending_;

	bool bLexicographicOrder_;
	bool bLexOrderAscending_;

	bool bAllowDups_;

	size_t iGroupSize_;

	size_t iTaskRepeatCount_;
	bool bDryRun_;

	bool bHelp_;

	// Random permutation engine Id.
	//size_t iRandPermEngId_;

	bool parseSize_t_(char, size_t&);
};	// class StrPermGenCLI

#endif //	str_perm_gen_cli_hpp
