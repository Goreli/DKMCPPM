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
	void PermutationGeneratorBase<T>::generate(const std::vector<T>& vocabulary, bool bDuplicatesAllowed) {
		permutation_.resize(vocabulary.size());
		vocabulary_ = vocabulary;
		if(bDuplicatesAllowed)
			generate_(0);
		else
			generate_nodups_(0);
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_(size_t iPos) {
		size_t vocSize = vocabulary_.size();

		for (size_t inx = 0; inx < vocabulary_.size(); inx++)
		{
			permutation_[iPos] = vocabulary_[inx];

			// If the call stack has hit the bottom of recursion tree then
			// process the permutation and move on to the next recursion cycle.
			// Otherwise just keep drilling down.
			if (vocSize == 1)
				process_(permutation_);
			else
			{
				vocabulary_.erase(vocabulary_.begin() + inx);
				generate_(iPos + 1);
				vocabulary_.insert(vocabulary_.begin() + inx, permutation_[iPos]);
			}
		}
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_nodups_(size_t iPos) {
		size_t vocSize = vocabulary_.size();

		for (size_t inx = 0; inx < vocabulary_.size(); inx++)
		{
			auto findIt = std::find(vocabulary_.begin(), vocabulary_.begin()+inx, vocabulary_[inx]);
			if (findIt != vocabulary_.begin() + inx)
				continue;

			permutation_[iPos] = vocabulary_[inx];

			// If the call stack has hit the bottom of recursion tree then
			// process the permutation and move on to the next recursion cycle.
			// Otherwise just keep drilling down.
			if (vocSize == 1)
				process_(permutation_);
			else
			{
				vocabulary_.erase(vocabulary_.begin() + inx);
				generate_nodups_(iPos + 1);
				vocabulary_.insert(vocabulary_.begin() + inx, permutation_[iPos]);
			}
		}
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_l(const std::vector<T>& vocabulary, bool bAscending) {
		permutation_ = vocabulary;

		if (bAscending)
			do process_(permutation_);
			while (std::next_permutation(permutation_.begin(), permutation_.end()));
		else
			do process_(permutation_);
			while (std::prev_permutation(permutation_.begin(), permutation_.end()));
	}
};  // namespace dk
