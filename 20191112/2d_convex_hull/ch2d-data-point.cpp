/* ch2d-data-point.cpp
This module defines the ch2d::DataPoint data structure
along with its overloaded operators and member functions
required in this implementation of the 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/

#include <algorithm>
#include <sstream>
#include <cmath>
#include "ch2d-data-point.hpp"

using namespace std;
using namespace ch2d;

const double constPi = 3.141592653589793238463;

void ltrim(string &s) noexcept {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}
void rtrim(string &s) noexcept {
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
}
void trim(string &s) noexcept {
    ltrim(s);
    rtrim(s);
}
void DataPoint::parse(const string& str) noexcept {
    stringstream ss(str);
    string token;
    getline(ss, token, ',');
    trim(token);
    x = stod(token);
    getline(ss, token, ',');
    trim(token);
    y = stod(token);
}
DataPoint& DataPoint::operator += (const DataPoint& point) noexcept
{
    x += point.x;
    y += point.y;
    return *this;
}
DataPoint& DataPoint::operator /= (size_t integralScalar)
{
    x /= decltype(x)(integralScalar);
    y /= decltype(y)(integralScalar);
    return *this;
}
void DataPoint::calcLenSqr(const DataPoint& point) noexcept {
    double dX = x - point.x;
    double dY = y - point.y;
    lengthSqr = dX*dX + dY*dY;
}
DataPoint  DataPoint::operator - (const DataPoint& p) const noexcept {
    DataPoint result;
    result.x = this->x - p.x;
    result.y = this->y - p.y;
    return result;
}
void DataPoint::calcAlpha(const DataPoint& tip, const DataPoint& origin) noexcept {
    DataPoint v1 = *this - origin;
    DataPoint v2 = tip - origin;

    alpha = calcAngle(v1, v2);
}
double DataPoint::calcAngle(const DataPoint& v1, const DataPoint& v2) noexcept {
    double dotProduct = v2.x * v1.x + v2.y * v1.y;
    double determinant = v2.x * v1.y - v2.y * v1.x;
    double angle = atan2(determinant, dotProduct);

    // Make the angle vary in the [0, 2*Pi] domain to preclude interruption of continuity
    // at the Pi boundary point past which it would otherwise abruptly become negative.
    // This is because in this implementation we need continuity around Pi, not around
    // 0 (zero).
    if(angle < 0.0)
        angle += 2*constPi;

    return angle;
}

