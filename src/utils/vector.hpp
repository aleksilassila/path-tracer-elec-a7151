#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <iostream>
#include <cmath>

/**
 * Our own implementation of a vector class
 */
class Vector {
public:

    Vector(double x = 0, double y = 0, double z = 0) : x_(x), y_(y), z_(z) {}

    double x() const { return x_; }

    double y() const { return y_; }

    double z() const { return z_; }

    /**
     * Vector addition
     * @param vector
     * @return Vector
     */
    Vector operator+(const Vector &b) {
        return Vector(x_ + b.x_, y_ + b.y_, z_ + b.z_);
    }

    /**
     * Vector subtraction
     * @param vector
     * @return Vector
     */
    Vector operator-(const Vector &b) {
        return Vector(x_ - b.x_, y_ - b.y_, z_ - b.z_);
    }

    /**
     * Vector addition assignment operation
     * @param vector
     * @return Vector
     */
    Vector operator+=(const Vector &b) {
        return Vector(x_ += b.x_, y_ += b.y_, z_ += b.z_);
    }

    /**
     * Vector multiplication by scalar
     * @param scalar
     * @return Vector
     */
    Vector operator*(double k) {
        return Vector(k * x_, k * y_, k * z_);
    }

    /**
     * Calculates length of a vector
     * @return double
     */
    double Len() {
        return std::sqrt(std::pow(x_, 2) + std::pow(y_, 2) + std::pow(z_, 2));
    }

    /**
     * Calculates normal vector
     * @return Vector
     */
    Vector Norm() {
        if (Len() == 0) return {0,0,0};
        return *this * (1 / Len());
    }

    /**
     * Dot Product using operator *
     * @param vector
     * @return double
     */
    double operator*(const Vector &v2) {
        return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
    }


    /**
     * Cross Product using operator %
     * @param vector
     * @return Vector
     */
    Vector operator%(const Vector &v2) {
        return Vector(y_ * v2.z_ - z_ * v2.y_, z_ * v2.x_ - x_ * v2.z_,
                      x_ * v2.y_ - y_ * v2.x_);
    }

    /**
     * Compares if vectors are equal
     * @param vector
     * @return bool
     */
    bool operator==(const Vector &v2) {
        return x_ == v2.x_ && y_ == v2.y_ && z_ == v2.z_;
    }

    /**
     * Compares if vectors are NOT equal
     * @param vector
     * @return bool
     */
    bool operator!=(const Vector &v2) {
        return !(*this == v2);
    }

    /**
     * Dot Product using named function
     * @param vector
     * @return double
     */
    double Dot(const Vector &v2) {
        return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
    }

    /**
     * Cross Product using named function
     * @param vector
     * @return Vector
     */
    Vector CrossProduct(const Vector &v2) {
        return Vector(y_ * v2.z_ - z_ * v2.y_,
                      z_ * v2.x_ - x_ * v2.z_,
                      x_ * v2.y_ - y_ * v2.x_);
    }

    /**
     * Calculates distance between vectors
     * @param vector
     * @return double
     */
    double Distance(const Vector &v2) {
        return std::sqrt(std::pow(x_ - v2.x_, 2) + std::pow(y_ - v2.y_, 2) + std::pow(z_ - v2.z_, 2));
    }

    /**
     * Writes to stream
     * @param stream
     * @param vector
     * @return std::stream
     */
    friend std::ostream &operator<<(std::ostream &os, const Vector &v);


private:

    double x_, y_, z_;


};

inline std::ostream &operator<<(std::ostream &os, const Vector &v) {
    os << '(' << v.x_ << ',' << v.y_ << ',' << v.z_ << ')';
    return os;
}

#endif //! _VECTOR_HPP_
