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

    void MoveTo(const Vector &position) { position_ = position; }

    Vector GetPosition() { return position_; }

    void SetViewPlaneDistance(double viewPlaneDistance) { viewPlaneDistance_ = viewPlaneDistance; }

    [[nodiscard]] double GetViewPlaneDistance() const { return viewPlaneDistance_; }

    void SetYaw(double yaw) {
        yaw_ = std::fmod(yaw, 2 * M_PI);
        updateCamera();
    }

    [[nodiscard]] double GetYaw() const { return yaw_; }

    void SetPitch(double pitch) {
        pitch_ = std::clamp(pitch, -M_PI / 2, M_PI / 2);
        updateCamera();
    }

    [[nodiscard]] double GetPitch() const { return pitch_; }

    void LookAt(Vector target) {
        Vector direction = (target - position_).Norm();
        yaw_ = std::atan2(direction.x(), direction.z());
        pitch_ = std::asin(direction.y());
        direction_ = direction;
    }

    void SetFovDeg(double fov) {
        viewPlaneDistance_ = 1 / std::tan(fov * M_PI / 360);
        updateCamera();
    }

    [[nodiscard]] double GetFovDeg() const {
        return std::atan(1 / viewPlaneDistance_) * 360 / M_PI;
    }

    void IncrementSpeed() {
        speed_ += speed_change_;
        std::cout << "Speed: " << speed_ << std::endl;
    }

    void DecrementSpeed() {
        speed_ -= speed_change_;
        if (speed_ < 0) {
            speed_ = 0;
        }
        std::cout << "Speed: " << speed_ << std::endl;
    }

    void IncrementDAngle() {
        d_angle_ += d_angle_change_;
        std::cout << "Angle Change Speed: " << d_angle_ << std::endl;
    }

    void DecrementDAngle() {
        d_angle_ -= d_angle_change_;
        if (d_angle_ < 0) {
            d_angle_ = 0;
        std::cout << "Angle Change Speed: " << d_angle_ << std::endl;
        }
    }
    /**
     * @param xs x coordinate scaled between -1 and 1
     * @param ys y coordinate scaled between -1 and 1
     */
    Ray GetRay(double xs, double ys) {
        return Ray(position_, direction_ * viewPlaneDistance_ + (yCross_ * xs) + (xCross_ * ys));
    }

    void Rotate(double yawAdd, double pitchAdd) {
        SetYaw(yawAdd + yaw_);
        SetPitch(pitchAdd + pitch_);
        updateCamera();
    }

    void Move(double x, double y, double z) {
        position_ += Vector(x, y, z);
    }

    void MoveForward() {
        position_ += direction_ * speed_;
    }

    void MoveBackward() {
        position_ += direction_ * -1 * speed_;
    }

    void MoveRight() {
        position_ += yCross_ * speed_;
    }

    void MoveLeft() {
        position_ += yCross_ * -1 * speed_;
    }

    void MoveUp() {
        position_ += xCross_ * -1 * speed_;
    }

    void MoveDown() {
        position_ += xCross_ * speed_;
    }

    void MoveUpAlongYaxis() {
        position_ += Vector(0, 1, 0) * speed_;
    }

    void MoveDownAlongYaxis() {
        position_ += Vector(0, -1, 0) * speed_;
    }

    void YawPlus() {
       Rotate(d_angle_, 0);
    }

    void YawMinus() {
        Rotate(-d_angle_, 0);
    }

    void PitchPlus() {
        Rotate(0, d_angle_);
    }

    void PitchMinus() {
        Rotate(0, -d_angle_);
    }

    void updateCamera() {
        direction_ = Vector(std::sin(yaw_) * std::cos(pitch_), std::sin(pitch_),
                            std::cos(yaw_) * std::cos(pitch_));
        yCross_ = (Vector(-std::cos(yaw_), 0, std::sin(yaw_))).Norm();
        xCross_ = yCross_.CrossProduct(direction_).Norm();
    }

private:
    Vector position_;
    Vector direction_;
    double viewPlaneDistance_;
    double yaw_ = 0;
    double pitch_ = 0;
    double d_angle_ = 0.05; // Angle increment
    double speed_ = 1;
    double speed_change_ = 0.1;
    double d_angle_change_ = 0.02;
    Vector yCross_;
    Vector xCross_;
};

#endif //PATHTRACER_CAMERA_H
