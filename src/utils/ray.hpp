#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "vector.hpp"
#include <SFML/Graphics.hpp>

class Ray {

public:

    Ray(Vector origin, Vector direction, sf::Color color = sf::Color(255,255,255)) : o_(origin), d_(direction.Norm()) {
        color_ = color;
    }

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

    sf::Color GetColor() { return color_; }

    void SetColor(sf::Color color) { color_ = color; }


protected:
    Vector o_, d_;
    sf::Color color_;

};

#endif //! _RAY_HPP_
