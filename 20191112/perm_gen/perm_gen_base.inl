/* perm_gen_base.inl
This header file defines the PermutationGeneratorBase template class.

Copyright(c) 2019 David Krikheli

Modification history:
    14/Nov/2019 - David Krikheli created the module.
*/

namespace dk {

	template <class T>
	PermutationGeneratorBase<T>::PermutationGeneratorBase() {
	}
	template <class T>
	PermutationGeneratorBase<T>::~PermutationGeneratorBase() {
	}
	template <class T>
	bool PermutationGeneratorBase<T>::generate(const std::vector<T>& vocabulary) {
		permutation_.resize(vocabulary.size());
		bContinue_ = true;
		vocabulary_ = vocabulary;
		generate_(0);
		return bContinue_;
	}
	template <class T>
	void PermutationGeneratorBase<T>::generate_(size_t iPos) {
		size_t vocSize = vocabulary_.size();

		for(size_t inx = 0; inx < vocSize && bContinue_; inx++)
		{
			permutation_[iPos] = vocabulary_[inx];

			// If the call stack has hit the bottom of the recursion tree then
			// process the permutation and move on to the next recursion cycle.
			// Otherwise just keep drilling down.
			if (vocSize == 1)
				bContinue_ = process_(permutation_);
			else
			{
				vocabulary_.erase(vocabulary_.begin() + inx);
				generate_(iPos+1);
				vocabulary_.insert(vocabulary_.begin() + inx, permutation_[iPos]);
			}
		}
	}

};  // namespace dk
