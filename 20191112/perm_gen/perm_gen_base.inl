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
		: bProhibitDups_{ true }, bDefaultRandomPermAlg_{ false } {
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
	void PermutationGeneratorBase<T>::generate(const std::vector<T>& vocabulary, bool bAllowDups, size_t iRandPermAlgId) {
		permutation_.resize(vocabulary.size());
		vocabulary_ = vocabulary;
		bProhibitDups_ = !bAllowDups;
		switch (iRandPermAlgId) {
		case 0:
			bDefaultRandomPermAlg_ = false;
			generate_(0);
			break;
		case 1:
			bDefaultRandomPermAlg_ = true;
			generate_(0);
			break;
		case 2:
			generate_R2_R3_(0);
			break;
		case 3:
			generate_R2_R3_(1);
			break;
		}
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_(size_t iPos) {
		size_t vocSize = vocabulary_.size();

		std::uniform_int_distribution<size_t> dist(0, vocSize-1);

		size_t inx_{ 0 };
		for (size_t inx = 0; inx < vocabulary_.size(); inx++)
		{
			inx_ = bDefaultRandomPermAlg_ ? dist(_randNumGen) : inx;

			auto it = vocabulary_.begin() + inx_;
			if (bProhibitDups_) {
				auto findIt = std::find(vocabulary_.begin(), it, *it);
				if (findIt != it)
					continue;
			}

			permutation_[iPos] = *it;

			// If the call stack has hit the bottom of recursion tree then
			// process the permutation and move on to the next recursion cycle.
			// Otherwise just keep drilling down.
			if (vocSize == 1)
				process_(permutation_);
			else
			{
				vocabulary_.erase(it);
				generate_(iPos + 1);
				vocabulary_.insert(vocabulary_.begin() + inx_, permutation_[iPos]);

				// The following piece of code ran perfectly ok when compiled with clang++
				// on the Ubuntu subsystem of Windows 10. It crashed when compiled with
				// VS 2019 on Windows 10.
				/*
					it = vocabulary_.erase(it);
					generate_nodups_(iPos + 1);
					vocabulary_.insert(it, permutation_[iPos]);
				*/
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
	template <class T>
	void PermutationGeneratorBase<T>::generate_R2_R3_(size_t iOffset) {
		size_t vocSize = vocabulary_.size();
		permutation_ = vocabulary_;
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
