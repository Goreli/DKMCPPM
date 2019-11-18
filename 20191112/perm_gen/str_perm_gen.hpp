/* str_perm_gen.cpp
The main entry point of the String Permutation application.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#ifndef str_perm_gen_hpp
#define str_perm_gen_hpp

#include <ostream>
#include <string>
#include <vector>
#include <regex>
#include "perm_gen_base.hpp"

class StringPermutationGenerator : public dk::PermutationGeneratorBase<char> {
public:
	StringPermutationGenerator(size_t iStartNum, size_t iCount, bool bPrintNumbers,
		std::ostream& outStream);
	void assignRegex(const std::string& strRegex, bool bExclusionRegex);

private:
	size_t iStartNum_;
	size_t iCount_;
	bool bPrintNumbers_;
	size_t iCounter_;
	size_t iTop_;
	std::ostream& outStream_;

	// Regex.
	bool bUseCLIRegex_;
	bool bExclusionRegex_;
	std::regex regexObj_;

	inline bool checkWithRegex_(const std::string& strPermutation);
	virtual bool process_(const std::vector<char>& permutation);
};

#endif // str_perm_gen_hpp
