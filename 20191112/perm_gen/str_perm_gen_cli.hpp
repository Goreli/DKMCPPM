/* str_perm_gen_cli.hpp
Declares the StrPermGenCLI command line parser class of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    16/Nov/2019 - David Krikheli created the module.
*/

#ifndef str_perm_gen_cli_hpp
#define str_perm_gen_cli_hpp

#include <string>

class StrPermGenCLI {
public:
	StrPermGenCLI();
	bool parse(int argc, char* argv[]);
	void printUsage();

	std::string& getInputString();
	size_t getStartNumber();
	size_t getCount();
	bool printPermutationNumbers();

private:
	std::string strInput_;
	size_t iStartNum_;
	size_t iCount_;
	bool bPrintNumbers_;
};	// class StrPermGenCLI

#endif //	str_perm_gen_cli_hpp
