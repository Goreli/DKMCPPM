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

namespace dk {
    typedef std::vector<DataPoint2D> DataContainer;

    // The calculateCentroid function provides an optional method to calculate
    // the centroid of the dataset as a mean of all points. The algorithm
    //  doesn't seem to be very demanding of precision as far as the location
    // of the centroid is concerned. In other words as long as the centroid is
    // located somewhere inside the area occupied by the dataset the algorithm
    // is going to be happy. If there is an efficient alternative to obtaining
    // a location of a suitable centroid then it is ok to leave this function
    // out and proceed towards calling prepareData.
    //
    // One more way to slightly improve performance is to calculate a mean of
    // only a small subset of points. This is exaclty the purpose of the 
    // subSampleSize parameter.
    // If(subSampleSize == 0) then all points in the dataset are used for
    // calculating the centroid. Otherwise only the specified number of points
    // is used.
    void calculateCentroid(const DataContainer&, DataPoint2D& centroid, const size_t subSampleSize = 0) noexcept;
    void prepareData(DataContainer& dataStore, DataPoint2D& centroid) noexcept;
    void calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint2D& centroid) noexcept;
};   // namespace dk

#endif // ch2d_alg_hpp
