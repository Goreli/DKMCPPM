/* ch2d_main.cpp
The main entry point of this implementation
of the 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <fstream>
#include "ch_alg2d.hpp"

using namespace std;
using namespace dk;

bool readInpFile(const string& inpFileName, DataContainer& container) noexcept {
    ifstream inpFile (inpFileName);
    if (inpFile.is_open()) {
        string inpLine;
        DataPoint2D point;
        while ( getline (inpFile, inpLine) ){
            point.parse(inpLine);
            container.push_back(point);
        }
        inpFile.close();
            return true;
    }
    else
        return false;
}
void debugOutput(const DataContainer& container, const DataPoint2D& centroid) {
    for(const auto& point: container)
        cout << point.x << "," << point.y << "," << point.alpha << "," << point.lengthSqr << '\n';
    cout << "Centroid: " << centroid.x << " " << centroid.y << '\n';
}

int main (int argc, char* argv[]) {
    if(argc <= 1) {
        cout << "2D convex hull calculator" << '\n';
        cout << "Copyright (c) 2019 David Krikheli" << '\n';
        cout << "Usage: ch2d [input file path]" << '\n';
        return 0;
    }

    string inpFilePath {argv[1]};
    DataContainer dataStore;
    if(!readInpFile(inpFilePath, dataStore)) {
        cerr << "Unable to read file " << inpFilePath;
        return 1;
    }

    DataPoint2D centroid;
    calculateCentroid(dataStore, centroid);
    prepareData(dataStore, centroid);

    //debugOutput(dataStore, centroid);

    DataContainer convexHull2D;
    if(dataStore.size() <= 4)
        convexHull2D = dataStore;
    else
        calcConvexHull2D(convexHull2D, dataStore, centroid);

    cout << "Original count of data points: " << dataStore.size()-1 << '\n';
    cout << "Closed line loop of the convex hull consisting of " << convexHull2D.size() << " points:" << '\n';
    for (const auto& point : convexHull2D)
        cout << point.x << "," << point.y << '\n';

    return 0;
}
