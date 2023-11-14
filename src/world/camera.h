//
// Created by Aleksi Lassila on 4.11.2023.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "../utils/ray.hpp"
#include <cmath>
#include <algorithm>

/**
 * Camera class is essentially a ray with some
 * extra properties and functionality such as fov
 * and rendering related stuff
 */
class Camera {

public:
    Camera(Vector position = Vector(0, 0, 0), double viewPlaneDistance = 1, double yaw = 0, double pitch = 0) :
            position_(position), viewPlaneDistance_(viewPlaneDistance), yaw_(yaw), pitch_(pitch) {
    }

    void SetPosition(const Vector &position) { position_ = position; }

    Vector GetPosition() { return position_; }

    void SetViewPlaneDistance(double viewPlaneDistance) { viewPlaneDistance_ = viewPlaneDistance; }

    [[nodiscard]] double GetViewPlaneDistance() const { return viewPlaneDistance_; }

    void SetYaw(double yaw) {
        yaw_ = std::fmod(yaw, 2 * M_PI);
    }

    [[nodiscard]] double GetYaw() const { return yaw_; }

    void SetPitch(double pitch) {
        pitch_ = std::clamp(pitch, 0.0, M_PI / 2);
    }

    [[nodiscard]] double GetPitch() const { return pitch_; }

    void LookAt(Vector target) {
        Vector direction = (target - position_).Norm();
        yaw_ = std::atan2(direction.x(), direction.z());
        pitch_ = std::asin(direction.y());
    }

    void SetFovDeg(double fov) {
        viewPlaneDistance_ = 1 / std::tan(fov * M_PI / 360);
    }

    [[nodiscard]] double GetFovDeg() const {
        return std::atan(1 / viewPlaneDistance_) * 360 / M_PI;
    }

    /**
     * @param xs x coordinate scaled between -1 and 1
     * @param ys y coordinate scaled between -1 and 1
     */
    Ray GetRay(double xs, double ys) {
        Vector direction = Vector(std::sin(yaw_) * std::cos(pitch_), std::sin(pitch_),
                                  std::cos(yaw_) * std::cos(pitch_));
        Vector yCross = direction.CrossProduct(Vector(0, 1, 0)).Norm();
        Vector xCross = yCross.CrossProduct(direction).Norm();

        return Ray(position_, direction * viewPlaneDistance_ + (yCross * xs) + (xCross * ys));
    }

private:
    Vector position_;
    double viewPlaneDistance_;
    double yaw_ = 0;
    double pitch_ = 0;
};

#endif //PATHTRACER_CAMERA_H
