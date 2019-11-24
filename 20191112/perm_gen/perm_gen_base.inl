/* perm_gen_base.inl
This header file defines the PermutationGeneratorBase template class.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <algorithm>

namespace dk {

	template <class T>
	PermutationGeneratorBase<T>::PermutationGeneratorBase()
		: bExcludeDups_{ false }, bRandom_{ false } {
		try {
			auto iSeed = std::random_device{}();
			_randNumGen.seed(iSeed);
		}
		catch (...) {
			auto iSeed = std::chrono::system_clock::now().time_since_epoch().count();
			_randNumGen.seed(iSeed);
		}
	}
	template <class T>
	PermutationGeneratorBase<T>::~PermutationGeneratorBase() {
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate(const std::vector<T>& symbolPool, bool bExcludeDups, size_t iRandPermAlgId) {
		permutation_.resize(symbolPool.size());
		symbolPool_ = symbolPool;
		switch (iRandPermAlgId) {
		case 0:
			bExcludeDups_ = bExcludeDups;
			bRandom_ = false;
			generate_(0);
			break;
		case 1:
			bExcludeDups_ = bExcludeDups;
			bRandom_ = true;
			generate_(0);
			break;
		case 2:
			// bExcludeDups_ and bRandom_ have no effect on generate_R2_R3_(....).
			generate_R2_R3_(0);
			break;
		case 3:
			// bExcludeDups_ and bRandom_ have no effect on generate_R2_R3_(....).
			generate_R2_R3_(1);
			break;
		}
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_(size_t iPos) {
		size_t vocSize = symbolPool_.size();

		std::uniform_int_distribution<size_t> dist(0, vocSize-1);

		size_t inx{ 0 };
		for (size_t _inx_ = 0; _inx_ < symbolPool_.size(); _inx_++)
		{
			if(bRandom_)
				inx = dist(_randNumGen);
			else {
				inx = _inx_;
				if (bExcludeDups_) {
					auto it = symbolPool_.begin() + inx;
					auto findIt = std::find(symbolPool_.begin(), it, *it);
					if (findIt != it)
						continue;
				}
			}

			permutation_[iPos] = symbolPool_[inx];

			// If the call stack has hit the bottom of recursion tree then
			// process the permutation and move on to the next recursion cycle.
			// Otherwise just keep drilling down.
			if (vocSize == 1)
				process_(permutation_);
			else
			{
				symbolPool_.erase(symbolPool_.begin() + inx);
				generate_(iPos + 1);
				symbolPool_.insert(symbolPool_.begin() + inx, permutation_[iPos]);

				// The following piece of code ran perfectly ok when compiled with clang++
				// on the Ubuntu subsystem of Windows 10. It crashed when compiled with
				// VS 2019 on Windows 10.
				/*
					it = symbolPool_.erase(it);
					generate_nodups_(iPos + 1);
					symbolPool_.insert(it, permutation_[iPos]);
				*/
			}
		}
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_l(const std::vector<T>& symbolPool, bool bAscending) {
		permutation_ = symbolPool;

		if (bAscending)
			do process_(permutation_);
			while (std::next_permutation(permutation_.begin(), permutation_.end()));
		else
			do process_(permutation_);
			while (std::prev_permutation(permutation_.begin(), permutation_.end()));
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_R2_R3_(size_t iOffset) {
		size_t vocSize = symbolPool_.size();
		permutation_ = symbolPool_;
		size_t j{ 0 };
		while (true) {
			for (size_t i = 0; i < vocSize - 1; i++) {
				std::uniform_int_distribution<size_t> dist(i + iOffset, vocSize - 1);
				j = dist(_randNumGen);
				iter_swap(permutation_.begin() + i, permutation_.begin() + j);
			}
			process_(permutation_);
		}
	}
};  // namespace dk
