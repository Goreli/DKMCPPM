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

static DataPoint2D getTipOfLongestVector(const DataContainer& container) noexcept {
    double maxDistanceSqr {0.0};
    DataPoint2D tip = container[0];
    for(const auto& point: container) {
        if(point.lengthSqr > maxDistanceSqr) {
            maxDistanceSqr = point.lengthSqr;
            tip = point;
        }
    }
    return tip;
}
static bool compFunc4Sort(const DataPoint2D& p1, const DataPoint2D& p2) {
    if(p1.alpha < p2.alpha)
        return true;
    if(p1.alpha > p2.alpha)
        return false;
    return (p1.lengthSqr > p2.lengthSqr);
}
static bool compFunc4Unique(const DataPoint2D& p1, const DataPoint2D& p2) {
    return p1.alpha == p2.alpha;
}
static void removeExtraPointsWithEqualAngles(DataContainer& dataStore) {
    auto it = unique(dataStore.begin(), dataStore.end(), compFunc4Unique);
    dataStore.resize(distance(dataStore.begin(), it));
}

// Public interface functions.
void dk::calculateCentroid(const DataContainer& dataStore, DataPoint2D& centroid, const size_t subSampleSize) noexcept {
    centroid.x = 0.0;
    centroid.y = 0.0;

    size_t effectvSubSampleSize = subSampleSize? min(subSampleSize, dataStore.size()) : dataStore.size();
    size_t inx { 0 };
    
    while(inx < effectvSubSampleSize)
        centroid += dataStore[inx++];
    centroid /= inx;
}
void dk::prepareData(DataContainer& dataStore, DataPoint2D& centroid) noexcept {
    // Calculate lengths of the vectors relative to the centroid.
    // We don't really need the actual lengths. Squared lenghts
    // will suffice as that will save us some compute time.
    for(auto& point: dataStore)
        point.calcLenSqr(centroid);

    DataPoint2D tipOfLongestVector = getTipOfLongestVector(dataStore);
    for(auto& point: dataStore)
        point.calcAlpha(tipOfLongestVector, centroid);

    sort(dataStore.begin(), dataStore.end(), compFunc4Sort);
    removeExtraPointsWithEqualAngles(dataStore);

    // Put a copy of the first point at the end of the sequence
    // to make sure the algorithm delivers a closed line loop.
    dataStore.push_back(dataStore[0]);
}

/*
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
        2.2 Calculate a delta vector between the result of step 2.1 and last
            known good vector;
        2.3 Calculate an angle between the delta vector and last known good
            vector;
        2.4 Assess the angle. If it's smaller than any of the previous
            angles calculated in this pass than treat the current point as
            a suitable candidate;
        2.5 Move on to the next point until the end of the input dataset has
            been reached. The last suitable candidate identified is to be
            saved at the beginning of the next pass.
*/
/*
void dk::calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint2D& centroid) noexcept {
    DataPoint2D lastGoodPoint;
    DataPoint2D lastGoodVector;

    size_t insertionIndex = 0;
    bool bKeepGoing = true;

    // Remember - the first point and last point are the same because
    // we are trying to identify points that form a closed loop of lines.
    // So make sure the algo doesn't hit the last point during the first pass
    // because it will show zero angle relative to the first point and,
    // therefore cause to discard any good non-zero angle case identified.
    size_t offset = 1;

    while(bKeepGoing) {
        //lastVertexPoint = dataStore[insertionIndex];

        // Save the good point identified in the previous run.
        // Also, calculate its respective vector relative to the centroid.
        lastGoodPoint = dataStore[insertionIndex];
        convexHull2D.push_back(lastGoodPoint);
        lastGoodVector = lastGoodPoint - centroid;
        bKeepGoing = false;

        double minAngle{impossiblyLargeAngle};
        for(size_t inx = insertionIndex+1; inx < dataStore.size()-offset; inx++) {
            DataPoint2D vertexPoint = dataStore[inx];
            DataPoint2D vertexVector = vertexPoint - centroid;

            // Evaluate the delta between this vector and last saved vector.
            DataPoint2D deltaVector = vertexVector - lastGoodVector;
            double angle = DataPoint2D::calcAngle(deltaVector, lastGoodVector);

            // A smaller angle will indicate a better alignment of the delta
            // vector with the last saved vector. 
            // So let's examine the angle and register it along with the
            // respective point if the value of the angle is smaller than
            // before.
            if(angle < minAngle) {
                minAngle = angle;
                insertionIndex = inx;
                bKeepGoing = true;
            }
        }
        offset = 0;
    }
}
*/
/*
void dk::calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint2D& centroid) noexcept {
    DataPoint2D lastGoodPoint;
    DataPoint2D lastGoodVector;

    size_t insertionIndex = 0;
    bool bKeepGoing = true;

    // Remember - the first point and last point are the same because
    // we are trying to identify points that form a closed loop of lines.
    // So make sure the algo doesn't hit the last point during the first pass
    // because it will show zero angle relative to the first point and,
    // therefore cause to discard any good non-zero angle case identified.
    size_t offset = 1;

    while(bKeepGoing) {
        //lastVertexPoint = dataStore[insertionIndex];

        // Save the good point identified in the previous run.
        // Also, calculate its respective vector relative to the centroid.
        lastGoodPoint = dataStore[insertionIndex];
        convexHull2D.push_back(lastGoodPoint);
        lastGoodVector = lastGoodPoint - centroid;
        bKeepGoing = false;

        double minAngle{impossiblyLargeAngle};
        for_each(
            dataStore.begin() + insertionIndex + 1, 
            dataStore.end()-offset, 
            [&centroid, &lastGoodVector, &minAngle, &dataStore, &insertionIndex, &bKeepGoing]
                (const auto& vertexPoint)
        {
            DataPoint2D vertexVector = vertexPoint - centroid;

            // Evaluate the delta between this vector and last saved vector.
            DataPoint2D deltaVector = vertexVector - lastGoodVector;
            double angle = DataPoint2D::calcAngle(deltaVector, lastGoodVector);

            // A smaller angle will indicate a better alignment of the delta
            // vector with the last saved vector. 
            // So let's examine the angle and register it along with the
            // respective point if the value of the angle is smaller than
            // before.
            if(angle < minAngle) {
                minAngle = angle;
                insertionIndex = &vertexPoint - &dataStore[0];
                bKeepGoing = true;
            }
        });

        offset = 0;
    }
}
*/
void dk::calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint2D& centroid) noexcept {
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
    bool bKeepGoing = true;

    DataPoint2D lastGoodPoint;
    DataPoint2D lastGoodVector;

    while(bKeepGoing) {
        // Save the good point identified in the previous run.
        // Also, calculate its respective vector relative to the centroid.
        lastGoodPoint = *candidatePointIt;
        convexHull2D.push_back(lastGoodPoint);
        lastGoodVector = lastGoodPoint - centroid;
        bKeepGoing = false;

        double minAngle{impossiblyLargeAngle};
        for(auto it = candidatePointIt + 1; it != dataStore.end() - offset; it++) 
        {
            // Evaluate the delta between this vector and last saved vector.
            DataPoint2D deltaVector = *it - centroid - lastGoodVector;
            double angle = DataPoint2D::calcAngle(deltaVector, lastGoodVector);

            // A smaller angle will indicate a better alignment of the delta
            // vector with the last saved vector. 
            // So let's examine the angle and register it along with the
            // respective point if the value of the angle is smaller than
            // before.
            if(angle < minAngle) {
                minAngle = angle;
                candidatePointIt = it;
                bKeepGoing = true;
            }
        }

        offset = 0;
    }
}
