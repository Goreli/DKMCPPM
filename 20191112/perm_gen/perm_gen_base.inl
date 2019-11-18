/* perm_gen_base.inl
This header file defines the PermutationGeneratorBase template class.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

#include <exception>

namespace dk {

	class PermutationGeneratorStopSignal : public std::exception {};

	template <class T>
	PermutationGeneratorBase<T>::PermutationGeneratorBase() : bStop_{ false } {
	}
	template <class T>
	PermutationGeneratorBase<T>::~PermutationGeneratorBase() {
	}
	template <class T>
	bool PermutationGeneratorBase<T>::generate(const std::vector<T>& vocabulary) {
		permutation_.resize(vocabulary.size());
		bStop_ = false;
		vocabulary_ = vocabulary;
		try {
			generate_(0);
		}
		catch (const PermutationGeneratorStopSignal&) {
			// Derived class requested to stop and made generate_() throw
			// an exception.
		}
		return bStop_;
	}
	template <class T>
	void PermutationGeneratorBase<T>::stop() noexcept {
		bStop_ = true;
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
			{
				process_(permutation_);
				if (bStop_)
					// The derived class has called the stop() function. Stop.
					throw PermutationGeneratorStopSignal();
			}
			else
			{
				it = vocabulary_.erase(it);
				generate_(iPos + 1);
				it = vocabulary_.insert(it, permutation_[iPos]);
			}
		}
	}
};  // namespace dk
