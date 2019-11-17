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

		// Populate std::vector<T> with the input sequence and pass it into
		// the generate() function to start generating permutations of the
		// sequence. This will result in the process_() function triggering
		// multiple times - once per permutation with the permutation passed
		// into the process_() function as an argument. A derived class is
		// required that would override the process_() virtual function to
		// provide the application specific treatment of permutations.
		// 
		// If the permutation generator was stopped due the overriden
		// process_() function returning 'false' then the generate() function
		// in turn will return 'false' to the calling application. Otherwise,
		// if the permutation generator was stopped because it exhausted all
		// the permutation possibilities then the generate() function will
		// return 'true' to the calling application.
		bool generate(const std::vector<T>&);

	private:
		void generate_(size_t iPos);

		// The permutation engine delivers permutations through this function.
		// Every time a new permutation is generated it is passed into this
		// function as a parameter. Derived classes are expected to override
		// this function to provide application specific treatment of the 
		// permutations.
		//
		// The overriding function implemented in the derived class should
		// return 'true' if the derived class wants the permutation engine to
		// continue generating new permutations. Otherwise, if the derived
		// class wants to stop generating new permutations, this function
		// should return 'false'.
		// Example scenarios of the derived class wanting to stop permutations
		// permutations are:
		//  - Only need to find a permutation that satisfies a particular
		//    condition and then stop;
		//  - Need to generate only a small number of permutations.
		virtual bool process_(const std::vector<T>&) = 0;

		// Holds the current permutation.
		std::vector<char> permutation_;

		// Holds the sequence of input symbols to generate permutations of.
		std::vector<char> vocabulary_;

		bool bContinue_;
	};
};  // namespace dk

#include "perm_gen_base.inl"

#endif  // perm_gen_base_hpp
