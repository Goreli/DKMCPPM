/* ch_data_point2d.hpp
This module declares the ch2d::DataPoint data structure
along with certain overloaded operators required in this
implementation of the 2D Convex Hull algorithm.

Copyright(c) 2019 David Krikheli

Modification history:
    11/Nov/2019 - David Krikheli created the module.
*/

#ifndef ch_data_point2d_hpp
#define ch_data_point2d_hpp

#include <string>

namespace dk {

    struct DataPoint2D{
        double x {0};
        double y {0};
        
        // Angle between the following two vectors:
        //  - A vector with the tip at this point and origin at the centroid;
        //  - The longest vector in the input space of vectors relative to the
        //    centroid.
        double alpha {0};

        // Parses comma separated x and y values and populates x and y
        // respectively. Does not impact any other members.
        void parse(const std::string&) noexcept;

        // Updates x and y in (*this) object.
        // Does not impact any other members.
        DataPoint2D& operator += (const DataPoint2D& point) noexcept;

        // Divides x and y by the scalar. Helps to find a centroid of a cluster
        // of points. Does not impact any other members.
        DataPoint2D& operator /= (size_t integralScalar);

        // Calculates the squared distance between (*this) and point
        // represented by the argument.
        double calcDistSqr(const DataPoint2D&) const noexcept;

        // Returns a result of subtraction of the point represented
        // by the argument from (*this) point. Only calculates x and y.
        // Does not impact any other members.
        DataPoint2D  operator - (const DataPoint2D&) const noexcept;

        // Calculates an angle betweeen the following two vectors:
        //  - ((*this) - origin);
        //  - (tip - origin).
        void calcAlpha(const DataPoint2D& tip, const DataPoint2D& origin) noexcept;

        // Calculates an angle between the first and second vector represented
        // by the function arguments.
        // The angle is in the [0, 2*Pi) domain. The left boundary is inclusive
        // of zero and the right boundary is exclusive of 2*Pi.
        // An angle between a vector and itself equals 0.
        static double calcAngle(const DataPoint2D&, const DataPoint2D&) noexcept;
    };

};   // namespace dk

#endif // ch_data_point2d_hpp
