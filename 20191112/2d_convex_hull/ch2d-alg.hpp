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

    // The calculateCentroid function provides an optional method to calculate
    // the centroid of the dataset. The algorithm doesn't seem to be very
    // demanding of precision as far as the location of the centroid is
    // is concerned. In other words as long as the centroid is located
    // somewhere inside the area occupied by the dataset the algorithm is
    // going to be happy. If there is an efficient alternative to obtaining
    // a location of a suitable centroid then it is ok to leave this function
    // out and proceed towards calling prepareData.
    void calculateCentroid(const DataContainer&, DataPoint&) noexcept;
    void prepareData(DataContainer& dataStore, DataPoint& centroid) noexcept;
    void calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint& centroid) noexcept;
};   // namespace ch2d

#endif // ch2d_alg_hpp
