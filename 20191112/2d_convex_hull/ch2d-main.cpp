/* ch2d-main.cpp
The main entry point of this implementation
of the 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/

#include <iostream>
#include <fstream>
#include "ch2d-alg.hpp"

using namespace std;
using namespace ch2d;

bool readInpFile(const string& inpFileName, DataContainer& container) noexcept {
    ifstream inpFile (inpFileName);
    if (inpFile.is_open()) {
        string inpLine;
        DataPoint point;
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
void debugOutput(const DataContainer& container, const DataPoint& centroid) {
    for(const auto& point: container)
        cout << point.x << "," << point.y << "," << point.alpha << "," << point.lengthSqr << endl;
    cout << "Centroid: " << centroid.x << " " << centroid.y << endl;
}

int main () {
    string inpFileName = "inp-file.csv";
    DataContainer dataStore;

    if(!readInpFile(inpFileName, dataStore)) {
        cerr << "Unable to read file " << inpFileName;
        return 1;
    }

    DataPoint centroid;
    calculateCentroid(dataStore, centroid);
    prepareData(dataStore, centroid);

    //debugOutput(dataStore, centroid);

    DataContainer convexHull2D;
    if(dataStore.size() <= 4)
        convexHull2D = dataStore;
    else
        calcConvexHull2D(convexHull2D, dataStore, centroid);

    cout << "Original count of data points: " << dataStore.size()-1 << endl;
    cout << "Closed line loop of the convex hull consisting of " << convexHull2D.size() << " points:" << endl;
    for (const auto& point : convexHull2D)
        cout << point.x << "," << point.y << endl;

    return 0;
}
