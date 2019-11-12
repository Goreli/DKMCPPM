/* ch2d-data-point.hpp
This module declares the ch2d::DataPoint data structure
along with certain overloaded operators required in this
implementation of the 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/

#ifndef ch2d_data_point_hpp
#define ch2d_data_point_hpp

#include <string>

namespace ch2d {

    struct DataPoint{
        double x {0};
        double y {0};
        // Angle between the following two vectors:
        //  - A vector with the tip at this point and origin at the centroid;
        //  - The longest vector in the input space of vectors relative to the centroid.
        double alpha {0};
        // Squared length of a vector with the tip at this point and origin at the centroid.
        double lengthSqr {0};

        // Parses a comma separated list of x and y values and populates x and y.
        // Does not impact any other members.
        void parse(const std::string&) noexcept;

        // Updates x and y in (*this) object.
        // Does not impact any other members.
        DataPoint& operator += (const DataPoint& point) noexcept;

        // Divides x and y by the scalar. Helps to find a centroid of a cluster of points.
        // Does not impact any other members.
        DataPoint& operator /= (size_t integralScalar);

        // Populates lengthSqr with distance between (*this)
        // and point represented by the argument squared.
        // Does not impact any other members.
        void calcLenSqr(const DataPoint&) noexcept;

        // Returns a result of subtraction of the point represented
        // by the argument from (*this) point. Only calculates x and y.
        // Does not impact any other members.
        DataPoint  operator - (const DataPoint&) const noexcept;

        // Calculates an angle betweeen the following two vectors:
        //  - ((*this) - origin);
        //  - (tip - origin).
        void calcAlpha(const DataPoint& tip, const DataPoint& origin) noexcept;

        // Calculates an angle between the first and second vector represented
        // by the function arguments.
        static double calcAngle(const DataPoint&, const DataPoint&) noexcept;
    };

};   // namespace ch2d

#endif // ch2d_data_point_hpp
