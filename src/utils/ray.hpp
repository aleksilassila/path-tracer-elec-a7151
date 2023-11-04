#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "vector.hpp"

class Ray {

public:

    Ray(Vector origin, Vector direction) : o_(origin), d_(direction) {}

    Vector GetOrigin() const {
        return o_;
    }

    Vector GetDirection() const {
        return d_;
    }

    void SetOrigin(Vector origin) {
        o_ = origin;
    }

    void SetDirection(Vector direction) {
        d_ = direction;
    }


private:
    Vector o_, d_;

};

#endif //! _RAY_HPP_ 
