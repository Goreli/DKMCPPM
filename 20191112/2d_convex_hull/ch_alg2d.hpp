/* ch_alg2d.hpp
This module declares facilities required for
executing the 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/


#ifndef ch_alg2d_hpp
#define ch_alg2d_hpp

#include <vector>
#include "ch_data_point2d.hpp"

namespace dk {
    typedef std::vector<DataPoint2D> DataContainer;

    // The calculateCentroid function provides an optional method for
    // calculating the centroid of the dataset as a mean of all points.
    // The algorithm doesn't seem to be very demanding of precision as far as
    // the location of the centroid is concerned. In other words as long as the
    // centroid is located somewhere inside the area occupied by the dataset
    // the algorithm is going to be happy. If there is an efficient alternative
    // to obtaining a location of a suitable centroid then it is ok to leave
    // this function out and proceed calling the prepareData function.
    //
    // One more way to slightly improve performance is to calculate a mean of
    // only a small subset of points. This is exaclty the purpose of the 
    // subSampleSize parameter.
    // If(subSampleSize == 0) then all points in the dataset are used for
    // calculating the centroid. Otherwise only the specified number of points
    // is used.
    // 
    // It's worth mentioning that this function is not the performance
    // bottleneck. The prepareData function executes a sorting routine that
    // has significantly more severe impact on performance than this function.
    // The impact of calcConvexHull2D is more severe too.
    void calculateCentroid(const DataContainer&, DataPoint2D& centroid, const size_t subSampleSize = 0) noexcept;
    void prepareData(DataContainer& dataStore, DataPoint2D& centroid) noexcept;
    void calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint2D& centroid) noexcept;
};   // namespace dk

#endif // ch2d_alg_hpp
