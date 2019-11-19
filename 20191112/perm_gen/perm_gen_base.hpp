/* perm_gen_base.hpp
This header file declares the PermutationGeneratorBase template class.

Copyright(c) 2019 David Krikheli

Modification history:
    12/Nov/2019 - David Krikheli created the module.
*/

#ifndef perm_gen_base_hpp
#define perm_gen_base_hpp

#include <vector>

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

		// Executes the default permutation engine.
		// Populate std::vector<T> with the input sequence and pass it into
		// the generate() function to start generating permutations of the
		// sequence. This will result in the process_() function triggering
		// multiple times - once per permutation with the permutation passed
		// into the process_() function as an argument. A derived class is
		// required that would override the process_() virtual function to
		// provide the application specific treatment of permutations.
		//
		// The overriden process_() virtual function can call the stop()
		// function to stop the permutation generator.
		// 
		// If the permutation generator was stopped due to the overriden
		// process_() function calling the stop() function then the generate()
		// function will return 'true' to the calling application. Otherwise,
		// if the permutation generator stopped by itself because it exhausted
		// all the possibilities then the generate() function will return 'false'
		// to the calling application.
		bool generate(const std::vector<T>&);

		// Executes the lexicographic permutation engine.
		// This function is similar to generate(....). The difference is it
		// picks each subsequent permutation (or previous permutation depending
		// on the second parameter) arranged in a strictly lexicographic order.
		// Therefore, if the elements of the input vector are not sorted the function
		// will not print all the possible permutations. It will only print
		// permutations remaining before the end of the lexicographic sequence of
		// permutations.
		// The function drops duplicate permutations.
		// 
		// Set the bForward parameter to 'true' to move towards the end of
		// the lexicographic sequence. 
		// Set the bForward parameter to 'false' to move towards the start. 
		bool generate_l(const std::vector<T>&, bool bForward);

		void stop() noexcept;

	private:
		void generate_(size_t);

		// The permutation engine delivers permutations through this function.
		// Every time a new permutation is generated it is passed into this
		// function as a parameter. Derived classes are expected to override
		// this function to provide application specific treatment of the 
		// permutations.
		//
		// If the derived class wants to stop generating permutations then
		// the overriden process_() function should call the stop() function.
		// Example scenarios of the derived class wanting to stop generating
		// permutations are:
		//  - Only need to find a permutation that satisfies a particular
		//    condition and then stop;
		//  - Need to generate only a small number of permutations.
		virtual void process_(const std::vector<T>&) = 0;

		// Holds the current permutation.
		std::vector<char> permutation_;

		// Holds the sequence of input symbols to generate permutations of.
		std::vector<char> vocabulary_;

		// Normally 'false'. If the derived class calls the stop function then
		// this member is set to 'true'.
		bool bStop_;
	};
};  // namespace dk

#include "perm_gen_base.inl"

#endif  // perm_gen_base_hpp
