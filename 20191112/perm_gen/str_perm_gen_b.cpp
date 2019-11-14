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
#include "perm_gen_base.hpp"

using namespace std;
using namespace dk;

class StringPermutationGenerator : public PermutationGeneratorBase {
public:
	void resetPermutationCounter() { permutationCounter_ = 0; }
	size_t getPermutationCounter() { return permutationCounter_; }
private:
	virtual bool process_(const PermutationContainer& permutation) {
		permutationCounter_++;
		return true;
	}
	size_t permutationCounter_{ 0 };
};

void printUsage() {
	cout << "String permutation generator benchmarking application." << endl;
	cout << "Calculates and prints average time spent by two alternative generators." << endl;
	cout << "Copyright (c) 2019 David Krikheli" << endl;
	cout << "Usage: str-perm-gen-b [input string] [number of test cycles]" << endl;
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

	VocabularyContainer voc(inputString.begin(), inputString.end());
	StringPermutationGenerator spg;
	std::chrono::duration<double> totalElapsed(0.0);

	for (size_t cycleCounter = 0; cycleCounter < numCycles; cycleCounter++) {
		cout << "Test cycle #" << cycleCounter+1 << endl;

		cout << "\tRunning the permutation generator..." << endl;
		spg.resetPermutationCounter();
		auto start = std::chrono::high_resolution_clock::now();
		spg.generate(voc);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		cout << "\t" << spg.getPermutationCounter() << " permutations generated in " << elapsed.count() << " seconds." << endl;
		totalElapsed += elapsed;
		cout << endl;
	}

	cout << "Average time per test cycle in seconds: " << totalElapsed.count() / numCycles << endl;

    return 0;
}
