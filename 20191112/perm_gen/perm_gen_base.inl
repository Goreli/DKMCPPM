/* perm_gen_base.inl
This header file defines the PermutationGeneratorBase template class.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <algorithm>
#include <chrono>

namespace dk {

	template <class T>
	PermutationGeneratorBase<T>::PermutationGeneratorBase()
		: iCounter_{ 0 }, iNextInGroup_{ 0 }, randGen_(), dist_()
	{
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

		for (auto it = vocabulary_.begin(); it < vocabulary_.end(); it++)
		{
			permutation_[iPos] = *it;

			// If the call stack has hit the bottom of recursion tree then
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
	void PermutationGeneratorBase<T>::generate_r(const std::vector<T>& vocabulary, unsigned iGroupSize) {
		permutation_.resize(vocabulary.size());
		vocabulary_ = vocabulary;

		// iCounter_{ 0 }, iNextInGroup_{ 0 }, std::mt19937 randGen_, std::uniform_int_distribution<unsigned> dist_
		
		try {
			auto iSeed = std::random_device{}();
			randGen_.seed(iSeed);
		}
		catch (...) {
			auto iSeed = std::chrono::system_clock::now().time_since_epoch().count();
			randGen_.seed(iSeed);
		}
		dist_.param(std::uniform_int_distribution<unsigned>::param_type(1, iGroupSize));
		iNextInGroup_ = dist_(randGen_);
		iCounter_ = 0;

		generate_r_(0);
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_r_(size_t iPos) {
		size_t vocSize = vocabulary_.size();

		for (auto it = vocabulary_.begin(); it < vocabulary_.end(); it++)
		{
			permutation_[iPos] = *it;

			// If the call stack has hit the bottom of recursion tree and
			// the random index is right then process the permutation and move
			// on to the next recursion cycle. Otherwise just keep drilling down.
			if (vocSize == 1) {
				iCounter_++;
				if(iCounter_ == iNextInGroup_)
					process_(permutation_);
				if (iCounter_ == dist_.b()) {
					iCounter_ = 0;
					iNextInGroup_ = dist_(randGen_);
				}
			}
			else
			{
				it = vocabulary_.erase(it);
				generate_r_(iPos + 1);
				it = vocabulary_.insert(it, permutation_[iPos]);
			}
		}
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_nodups_(size_t iPos) {
		size_t vocSize = vocabulary_.size();

		for (auto it = vocabulary_.begin(); it < vocabulary_.end(); it++)
		{
			// Ignore this symbol if it's already in the permutation.
			auto findIt = find(vocabulary_.begin(), it, *it);
			if (findIt != it)
				continue;

			permutation_[iPos] = *it;

			// If the call stack has hit the bottom of recursion tree then
			// process the permutation and move on to the next recursion cycle.
			// Otherwise just keep drilling down.
			if (vocSize == 1)
				process_(permutation_);
			else
			{
				it = vocabulary_.erase(it);
				generate_nodups_(iPos + 1);
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
