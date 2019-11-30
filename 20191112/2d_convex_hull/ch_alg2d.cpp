/* ch_alg2d.cpp
This module implements the actual 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/

#include <algorithm>
#include "ch_alg2d.hpp"

using namespace std;
using namespace dk;

// This needs to be anything greater than 2*Pi.
const double impossiblyLargeAngle = 7.0;

static DataPoint2D getTipOfLongestVector(const DataContainer& container,
        const DataPoint2D& centroid) noexcept {
    double distanceSqr {0.0};
    double maxDistanceSqr {0.0};
    DataPoint2D tip;
    for(const auto& point: container) {
        distanceSqr = point.calcDistSqr(centroid);
        if(distanceSqr > maxDistanceSqr) {
            maxDistanceSqr = distanceSqr;
            tip = point;
        }
    }
    return tip;
}
static bool compFunc4Sort(const DataPoint2D& p1, const DataPoint2D& p2) {
    if(p1.alpha < p2.alpha)
        return true;
    return false;
}
// Public interface functions.
void dk::calculateCentroid(const DataContainer& dataStore,
        DataPoint2D& centroid, const size_t subSampleSize) noexcept {
    centroid.x = 0.0;
    centroid.y = 0.0;

    size_t effectvSubSampleSize = subSampleSize? 
        min(subSampleSize, dataStore.size()) : dataStore.size();
    size_t inx { 0 };
    
    while(inx < effectvSubSampleSize)
        centroid += dataStore[inx++];
    centroid /= inx;
}
void dk::prepareData(DataContainer& dataStore, DataPoint2D& centroid) noexcept {
    // For each vector calculate the angle between that vector and the longest
    // vector in the dataset.
    DataPoint2D tipOfLongestVector = getTipOfLongestVector(dataStore, centroid);
    for(auto& point: dataStore)
        point.calcAlpha(tipOfLongestVector, centroid);

    sort(dataStore.begin(), dataStore.end(), compFunc4Sort);

    // Put a copy of the first point at the end of the sequence
    // to make sure the algorithm delivers a closed line loop.
    dataStore.push_back(dataStore[0]);
}

/*  void dk::calcConvexHull2D(...)

    Executes multiple passes through the set of input points.
    By virtue of pre-sorting the very first point is good, so the first
    pass saves the first point.

    Each pass examines points following the last good one saved and identifies
    the next good one to save by applying the steps below. Every time it saves
    a good point it also memorises the respective vector relative to the
    centroid because it's required for subsequent calculations. We will call
    this vector the last known good vector.

    The steps are:
        1.1 Save the good point identified in the previous pass. If this is the
            first pass than save the first point;
        1.2 For the good point calculate a vector relative to the centroid.
            Call it the last known good vector.
    If there are any points in the input sequence remaining after the last good
    point saved then for each of them:
        2.1 Calculate a respective vector relative to the centroid;
        2.2 Calculate a delta vector by subtracting the last known good vector
            from the result of step 2.1;
        2.3 Calculate an angle between the delta vector and last known good
            vector;
        2.4 Assess the angle. A smaller angle will indicate a better
            directional alignment of the delta vector with the last known good
            vector. So if the angle is smaller than any of the previous angles
            calculated in this pass then treat the current point as a suitable
            candidate;
        2.5 Move on to the next point until the end of the input dataset has
            been reached. The last suitable candidate identified needs to be
            saved. Save it at the beginning of the next pass.
*/
void dk::calcConvexHull2D(DataContainer& convexHull2D,
        const DataContainer& dataStore, const DataPoint2D& centroid) noexcept {
    // Remember - the last point is the same as first point because we are
    // trying to identify points that form a closed loop of lines.
    // So make sure the algo doesn't compare the last point with the first one
    // because it will show zero angle between them and, therefore, discard any
    // good non-zero angle case that may have been identified.
    // The undesired comparison only happens in the first pass.  We will use
    // the following offset variable to preclude the algo from hitting the last
    // point in the first pass.
    size_t offset = 1;

    auto candidatePointIt = dataStore.begin();
    bool bEnableAnotherPass = true;

    DataPoint2D lastGoodPoint;
    DataPoint2D lastGoodVector;

    while(bEnableAnotherPass) {
        // Save the good point identified in the previous run.
        // Also, calculate its respective vector relative to the centroid.
        lastGoodPoint = *candidatePointIt;
        convexHull2D.push_back(lastGoodPoint);
        lastGoodVector = lastGoodPoint - centroid;

        bEnableAnotherPass = false;
        double minAngle{impossiblyLargeAngle};

        for(auto it = candidatePointIt + 1;
            it != dataStore.end() - offset;
            it++) 
        {
            // Evaluate the delta between this vector and last saved vector.
            DataPoint2D deltaVector = *it - centroid - lastGoodVector;
            double angle = DataPoint2D::calcAngle(deltaVector, lastGoodVector);
            if(angle < minAngle) {
                minAngle = angle;
                candidatePointIt = it;
                // We've got a potential candidate point to save. Facilitate
                // this by enabling another pass.
                bEnableAnotherPass = true;
            }
        }
        offset = 0;
    }
}
