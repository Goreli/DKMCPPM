/* calc_entropy_read.hpp
Declares the file reading interface of the calc-entropy application.

Copyright(c) 2019 David Krikheli

Modification history:
	3/Dec/2019 - David Krikheli created the module.
*/

#include <vector>
#include <fstream>


#ifndef calc_entropy_read_hpp
#define calc_entropy_read_hpp

namespace dk {

	typedef std::vector<size_t> CECounterType;
	void readDataFromFile(std::ifstream&, bool, CECounterType&);
};	// namespace dk

#endif	// calc_entropy_read_hpp
