/* perm_gen_base.hpp
This header file declares the PermutationGeneratorBase template class.

Copyright(c) 2019 David Krikheli

Modification history:
    12/Nov/2019 - David Krikheli created the module.
*/

#ifndef perm_gen_base_hpp
#define perm_gen_base_hpp

#include <vector>
#include <random>

namespace dk {

	// The class template provides a generic permutation engine that can
	// process sequences of arbitrary trivial types. Populate std::vector<T>
	// with the input sequence and pass it into the generate(....) function to
	// start generating permutations of the sequence. Every time the engine
	// generates a new permutation it passes the permutation into the 
	// process_(....) pure virtual function. Derived classes are responsible
	// for utilising (printing, updating application variables etc) the
	// permutations by overriding the process_(....) function.
	template <class T>
	class PermutationGeneratorBase {
	public:
		PermutationGeneratorBase();
		virtual ~PermutationGeneratorBase();

		// Executes the default permutation generator.
		// Populate std::vector<T> with the input value and pass it into
		// the generate() function to start generating permutations of the
		// value. This will result in the process_() function triggering
		// multiple times - once per permutation with the permutation passed
		// into the process_() function as an argument. A derived class is
		// required that would override the process_() virtual function to
		// provide the application specific treatment of permutations.
		//
		// The overriden process_() virtual function should throw an exception
		// to stop the permutation generator should there be a need for premature
		// completion of the process. Call the generate(....) function from within
		// a try block of the exception handling code to preclude propagation of the
		// exception back into the user space.
		// 
		// The second parameter, if 'true', allows to generate duplicate permutations.
		// With the second parameter set to 'false' the permutation generator does not
		// generate duplicate permutations.
		//
		// The third parameter specifies the id of a random permutation algorithm
		// to use. 0 - no randomisation.
		void generate(const std::vector<T>&, bool, size_t);

		// Executes the lexicographic permutation generator.
		// This function is similar to generate(....). The difference is it
		// picks each subsequent permutation (or previous permutation depending
		// on the second parameter) from a lexicographically arranged sequence.
		// Therefore, if the elements of the input vector are not sorted the function
		// will not print all the possible permutations. It will only print
		// permutations remaining before the end of the lexicographic sequence of
		// permutations.
		// The function drops duplicate permutations.
		// 
		// Set the bForward parameter to 'true' to move forward towards the end of
		// the lexicographic sequence. 
		// Set the bForward parameter to 'false' to move backward towards the start.
		void generate_l(const std::vector<T>&, bool bForward);

	private:
		void generate_(size_t);
		void generate_R2_R3_(size_t);

		// The permutation engine delivers permutations through this function.
		// Every time a new permutation is generated it is passed into this
		// function as a parameter. Derived classes are expected to override
		// this function to provide application specific treatment of the 
		// permutations.
		//
		// If the derived class wants to stop generating permutations then
		// the overriden process_() function should throw an exception. The
		// exception should be caught and processed by the caller of the
		// generate_(....) or generate_l(....) function, whichever was used
		// to start the permutation engine.
		// Example scenarios of the derived class wanting to stop generating
		// permutations are:
		//  - Only need to find a permutation that satisfies a particular
		//    condition and then stop;
		//  - Need to generate only a small number of permutations.
		virtual void process_(const std::vector<T>&) = 0;

		// Holds the current permutation.
		std::vector<char> permutation_;

		// Holds the sequence of input symbols to generate permutations of.
		std::vector<char> symbolPool_;

		bool bExcludeDups_;
		bool bRandom_;

	protected:
		// Random number generator.
		std::mt19937_64 _randNumGen;
	};
};  // namespace dk

#include "perm_gen_base.inl"

#endif  // perm_gen_base_hpp
