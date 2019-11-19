/* perm_gen_base.inl
This header file defines the PermutationGeneratorBase template class.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <algorithm>

namespace dk {

	template <class T>
	PermutationGeneratorBase<T>::PermutationGeneratorBase() {
	}
	template <class T>
	PermutationGeneratorBase<T>::~PermutationGeneratorBase() {
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate(const std::vector<T>& vocabulary) {
		permutation_.resize(vocabulary.size());
		vocabulary_ = vocabulary;
		generate_(0);
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_(size_t iPos) {
		size_t vocSize = vocabulary_.size();

		for (auto it = vocabulary_.begin(); it < vocabulary_.end(); it++)
		{
			permutation_[iPos] = *it;

			// If the call stack has hit the bottom of the recursion tree then
			// process the permutation and move on to the next recursion cycle.
			// Otherwise just keep drilling down.
			if (vocSize == 1)
				process_(permutation_);
			else
			{
				it = vocabulary_.erase(it);
				generate_(iPos + 1);
				it = vocabulary_.insert(it, permutation_[iPos]);
			}
		}
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_l(const std::vector<T>& vocabulary, bool bForward) {
		permutation_ = vocabulary;

		if (bForward)
			do process_(permutation_);
			while (std::next_permutation(permutation_.begin(), permutation_.end()));
		else
			do process_(permutation_);
			while (std::prev_permutation(permutation_.begin(), permutation_.end()));
	}
};  // namespace dk
