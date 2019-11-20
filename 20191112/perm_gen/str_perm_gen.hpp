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
#include <exception>
#include <random>

#include "perm_gen_base.hpp"

class PermutationGeneratorStopSignal : public std::exception {
public:
	PermutationGeneratorStopSignal();
};

class StringPermutationGenerator : public dk::PermutationGeneratorBase<char> {
public:
	StringPermutationGenerator(size_t iStartNum, size_t iPrintCount, bool bPrintNumbers,
		std::ostream& outStream);

	void assignRegex(const std::string& strRegex, bool bExclusionRegex);
	void setGroupSize(size_t iGroupSize);

private:
	// These are copies of the constructor arguments.
	size_t iStartNum_;
	size_t iPrintCount_;
	bool bPrintNumbers_;
	std::ostream& outStream_;

	// These are assigned to by assignRegex(....).
	bool bUseCLIRegex_;
	bool bExclusionRegex_;
	std::regex objRegex_;

	// Run-time counting. These are updated/maintained in process_(....).
	size_t iPermutationNumber_;
	size_t iPrintCounter_;

	inline bool checkWithRegex_(const std::string& strPermutation);
	virtual void process_(const std::vector<char>& permutation);

	// These are used for randomly selecting a permutation from each
	// consecutive group.
	size_t iGroupSize_;
	size_t iNextInGroup_;
	std::mt19937_64 randGen_;
	std::uniform_int_distribution<size_t> dist_;
};

#endif // str_perm_gen_hpp
