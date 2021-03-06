/* ch_data_point2d.cpp
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
#include "ch_data_point2d.hpp"

using namespace std;
using namespace dk;

const double constPi = 3.141592653589793238463;

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
	return ltrim(rtrim(str, chars), chars);
}

void DataPoint2D::parse(const string& str) noexcept {
    stringstream ss(str);
    string token;
    getline(ss, token, ',');
    trim(token);
    x = stod(token);
    getline(ss, token, ',');
    trim(token);
    y = stod(token);
}
DataPoint2D& DataPoint2D::operator += (const DataPoint2D& point) noexcept
{
    x += point.x;
    y += point.y;
    return *this;
}
DataPoint2D& DataPoint2D::operator /= (size_t integralScalar)
{
    x /= decltype(x)(integralScalar);
    y /= decltype(y)(integralScalar);
    return *this;
}
double DataPoint2D::calcDistSqr(const DataPoint2D& point) const noexcept {
    double dX = x - point.x;
    double dY = y - point.y;
    return dX*dX + dY*dY;
}
DataPoint2D  DataPoint2D::operator - (const DataPoint2D& p) const noexcept {
    DataPoint2D result;
    result.x = this->x - p.x;
    result.y = this->y - p.y;
    return result;
}
void DataPoint2D::calcAlpha(const DataPoint2D& tip, const DataPoint2D& origin) noexcept {
    DataPoint2D v1 = *this - origin;
    DataPoint2D v2 = tip - origin;

    alpha = calcAngle(v1, v2);
}
double DataPoint2D::calcAngle(const DataPoint2D& v1, const DataPoint2D& v2) noexcept {
    double dotProduct = v2.x * v1.x + v2.y * v1.y;
    double determinant = v2.x * v1.y - v2.y * v1.x;
    double angle = atan2(determinant, dotProduct);

    // Make the angle vary in the [0, 2*Pi) domain to preclude interruption of continuity
    // at the Pi boundary point past which it would otherwise abruptly become negative.
    // This is because in this implementation we need continuity around Pi, not around
    // 0 (zero).
    if(angle < 0.0)
        angle += 2*constPi;

    return angle;
}

