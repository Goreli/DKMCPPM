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
#include "perm_gen_base.hpp"

using namespace std;
using namespace dk;

class StringPermutationGenerator : public PermutationGeneratorBase<char> {
public:
	void resetPermutationCounter() { permutationCounter_ = 0; }
	size_t getPermutationCounter() { return permutationCounter_; }
private:
	virtual bool process_(const std::vector<char>& permutation) {
		permutationCounter_++;
		return true;
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

void printUsage() {
	cout << "String Permutation Generator Benchmarking Application" << endl;
	cout << "Executes the specified number of test cycles and reports the average time." << endl;
	cout << "Copyright (c) 2019 David Krikheli" << endl;
	cout << "Usage: str-perm-gen-b [input_string] [number_of_test_cycles]" << endl;
}

int main (int argc, char* argv[]) {
    if(argc <= 1 || argc > 3) {
		printUsage();
        return 0;
    }

	string inputString{ argv[1] };

	size_t numCycles = 1;
	if (argc == 3)
		numCycles = atoi(argv[2]);

	std::vector<char> vocabulary(inputString.begin(), inputString.end());
	StringPermutationGenerator spg;
	std::chrono::duration<double> totalElapsed(0.0);

	forceThousandsSeparators();

	for (size_t cycleCounter = 0; cycleCounter < numCycles; cycleCounter++) {
		cout << "Test cycle #" << cycleCounter+1 << endl;

		cout << "\tRunning the permutation generator for \"" << inputString << "\"." << endl;
		spg.resetPermutationCounter();
		auto start = std::chrono::high_resolution_clock::now();
		spg.generate(vocabulary);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		cout << "\t" << spg.getPermutationCounter() << " permutations generated in " << elapsed.count() << " seconds." << endl;
		totalElapsed += elapsed;
		cout << endl;
	}

	cout << "Average time per test cycle in seconds: " << totalElapsed.count() / numCycles << endl;

    return 0;
}
