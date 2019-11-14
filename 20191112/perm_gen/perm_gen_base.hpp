/* perm_gen_base.hpp
This header file declares the PermutationGeneratorBase class.

Copyright(c) 2019 David Krikheli

Modification history:
    12/Nov/2019 - David Krikheli created the module.
*/

#ifndef perm_gen_base_hpp
#define perm_gen_base_hpp

#include <vector>

namespace dk {
    typedef std::vector<char> PermutationContainer;
    typedef std::vector<char> VocabularyContainer;

    class PermutationGeneratorBase {
        public:
			PermutationGeneratorBase();
            virtual ~PermutationGeneratorBase();

			// If the process was stopped due to a condition triggered by a derived
			// class then the function returns true. Otherwise, if the process was
			// stopped because it reached its natural end and all possible permutations
			// have already been generated, then the function returns false.
            bool generate(const VocabularyContainer&);

        private:
            bool generate_();

			// Should return true if the derived class wants the permutation
			// engine to continue generating new permutations.
			// Otherwise, if the derived class wants to stop generating new
			// permutations, this function should return false.
			// Example scenarios of the derived class wanting to stop
			// generating permutations are:
			//  - Only need to find a permutation that satisfies a particular
			//    condition and then stop;
			//  - Need to generate only a small number of permutations.
			virtual bool process_(const PermutationContainer&) = 0;

            PermutationContainer permutation_;
            VocabularyContainer vocabulary_;
    };
};  // namespace dk

#endif  // perm_gen_base_hpp
