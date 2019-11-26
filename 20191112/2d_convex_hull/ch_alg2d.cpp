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
void dk::calcConvexHull2D(DataContainer& convexHull2D, const DataContainer& dataStore, const DataPoint2D& centroid) noexcept {
    DataPoint2D lastVertexPoint;
    DataPoint2D lastVertexVector;

    size_t insertionIndex = 0;
    bool bKeepGoing = true;

    // Make sure it doesn't hit the last point during the first pass
    // because it's likely to show zero angle relative to the first point.
    // So it will annihilate all the good non-zero angle cases.
    size_t offset = 1;

    while(bKeepGoing) {
        lastVertexPoint = dataStore[insertionIndex];
        lastVertexVector = lastVertexPoint - centroid;
        convexHull2D.push_back(lastVertexPoint);
        bKeepGoing = false;

        double minAngle{impossiblyLargeAngle};
        for(size_t inx = insertionIndex+1; inx < dataStore.size()-offset; inx++) {
            DataPoint2D vertexPoint = dataStore[inx];
            DataPoint2D vertexVector = vertexPoint - centroid;

            DataPoint2D deltaVector = vertexVector - lastVertexVector;
            double angle = DataPoint2D::calcAngle(deltaVector, lastVertexVector);

            if(angle < minAngle) {
                minAngle = angle;
                insertionIndex = inx;
                bKeepGoing = true;
            }
        }
        offset = 0;
    }
}
