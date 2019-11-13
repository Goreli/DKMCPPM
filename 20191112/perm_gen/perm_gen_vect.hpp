/* perm-gen.hpp
This header file declares the StringPermutationGenerator class.

Copyright(c) 2019 David Krikheli

Modification history:
    13/Nov/2019 - David Krikheli created the module.
*/

#ifndef perm_gen_hpp
#define perm_gen_hpp

#include <vector>

namespace dk {
    typedef std::vector<char> CharContainer;

    class StringPermutationGenerator {
        public:
            StringPermutationGenerator();
            virtual ~StringPermutationGenerator();
            void generate(const CharContainer&);

            // Should return true if the derived class wants the permutation
            // engine to continue generating new permutations.
            // Otherwise, if the derived class wants to stop generating new
            // permutations, this function should return false.
            // Example scenarios of the derived class wanting to stop
            // generating permutations are:
            //  - Only need to find a permutation that satisfies a particular
            //    condition and then stop;
            //  - Need to generate only a small number of permutations.
            virtual bool process(const CharContainer&) = 0;

        private:
            bool generate_();

            size_t permCount_;
            CharContainer permutation_;
            CharContainer vocabulary_;
    };
};  // namespace dk

#endif  // perm_gen_hpp
