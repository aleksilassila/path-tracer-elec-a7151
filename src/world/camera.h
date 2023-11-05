//
// Created by Aleksi Lassila on 4.11.2023.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "../utils/ray.hpp"

/**
 * Camera class is essentially a ray with some
 * extra properties and functionality such as fov
 * and rendering related stuff
 */
class Camera : public Ray {

public:
    Camera() : Camera(Vector(0, 0, 0), Vector(0, 0, 1)) {}

    Camera(Vector origin, Vector direction) : Ray(origin, direction),
                                              viewPlaneDistance_(1) {}

    /**
     * Calculates the ray for a given pixel
     * TODO: Take camera orientation (direction, d_) into account
     * @param u between -1 and 1
     * @param v between -1 and 1
     * @return
     */
    Ray GetRay(double u, double v) const {
        return {o_, Vector(u, v, 1 * viewPlaneDistance_).Norm()};
    }

private:
    double viewPlaneDistance_;
};

#endif //PATHTRACER_CAMERA_H
