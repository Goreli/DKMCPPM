/* ch2d-alg.hpp
This module declares facilities required for
executing the 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/


#ifndef ch2d_alg_hpp
#define ch2d_alg_hpp

#include <vector>
#include "ch2d-data-point.hpp"

namespace ch2d {

    typedef std::vector<DataPoint> DataContainer;
    void prepareData(DataContainer& dataStore, DataPoint& centroid) noexcept;
    void calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint& centroid) noexcept;

};   // namespace ch2d

#endif // ch2d_alg_hpp
