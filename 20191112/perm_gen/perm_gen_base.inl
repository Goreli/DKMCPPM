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
		permutation_.clear();
		vocabulary_ = vocabulary;
		return generate_();
	}
	template <class T>
	bool PermutationGeneratorBase<T>::generate_() {
		// If the call stack has hit the bottom of the recursion tree then
		// process the permutation and move on to the next recursion cycle.
		if (vocabulary_.size() == 0)
			return process_(permutation_);

		bool bContinue{ true };
		T symbol = T(0);
		size_t vocSize = vocabulary_.size();

		for (size_t inx = 0; inx < vocSize && bContinue; inx++) {
			symbol = vocabulary_[inx];
			permutation_.push_back(symbol);
			vocabulary_.erase(vocabulary_.begin() + inx);
			bContinue = generate_();
			vocabulary_.insert(vocabulary_.begin() + inx, symbol);
			permutation_.pop_back();
		}
		return bContinue;
	}

};  // namespace dk
