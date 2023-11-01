#include <iostream>
#include <cmath>

class Vector{
public:

    Vector(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }

    Vector operator+(Vector& b) {
	    return Vector(x_+b.x_, y_+b.y_, z_+b.z_);
    }
	    
    Vector operator-(Vector& b) {
	    return Vector(x_-b.x_, y_-b.y_, z_-b.z_);
    }

    Vector operator*(double k) {
	    return Vector(k*x_, k*y_, k*z_);
    }

    double Len() { 
	    return std::sqrt(std::pow(x_, 2) + std::pow(y_, 2) + std::pow(z_, 2));
    } 

    Vector Norm() {
	    return *this * (1/Len());
    } 

    double operator*(Vector& v2) {
	    return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
    }

    Vector operator%(Vector& v2) {
	    return Vector(y_*v2.z_ - z_*v2.y_,  z_*v2.x_ - x_*v2.z_,
			    x_*v2.y_ - y_*v2.x_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& v); 
    

private:

    double x_, y_, z_;

    
};

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << '(' << v.x_ << ',' << v.y_ << ',' << v.z_  << ')';
    return os;
}
