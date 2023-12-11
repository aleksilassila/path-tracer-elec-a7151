#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "../utils/ray.hpp"
#include <cmath>
#include <algorithm>

#define DEFAULT_LOOK_INCREMENT 0.05
#define DEFAULT_MOVEMENT_INCREMENT 0.1

/**
 * Class that implements a camera
 * It is essentially a ray with extra properties, functionality and operations related to rendering
 */
class Camera {

public:
    Camera(Vector position = Vector(0, 0, 0), double viewPlaneDistance = 1, double yaw = 0, double pitch = 0) :
            position_(position),
            viewPlaneDistance_(viewPlaneDistance),
            yaw_(yaw),
            pitch_(pitch) {}

    Vector GetPosition() { return position_; }

    [[nodiscard]] double GetViewPlaneDistance() const { return viewPlaneDistance_; }

    [[nodiscard]] double GetYaw() const { return yaw_; }

    [[nodiscard]] double GetPitch() const { return pitch_; }

    [[nodiscard]] double GetFovDeg() const { return std::atan(1 / viewPlaneDistance_) * 360 / M_PI; }

    void SetViewPlaneDistance(double viewPlaneDistance) { viewPlaneDistance_ = viewPlaneDistance; }

    void SetYaw(double yaw) {
        yaw_ = std::fmod(yaw, 2 * M_PI);
        UpdateCamera_();
    }

    void SetPitch(double pitch) {
        pitch_ = std::clamp(pitch, -M_PI / 2, M_PI / 2);
        UpdateCamera_();
    }

    void SetFovDeg(double fov = 70) {
        viewPlaneDistance_ = 1 / std::tan(fov * M_PI / 360);
        UpdateCamera_();
    }

    /**
     * Sets position to given parameter
     * @param Vector position
     */
    void MoveTo(const Vector &position) { position_ = position; }

    /**
     * Sets camera to look in a given direction
     * @param Vector target
     */
    void LookAt(Vector target) {
        Vector direction = (target - position_).Norm();
        yaw_ = std::atan2(direction.x(), direction.z());
        pitch_ = std::asin(direction.y());
        direction_ = direction;
    }

    void IncrementMoveSpeed(double amount = DEFAULT_MOVEMENT_INCREMENT) {
        movementSpeed_ += amount;
        std::cout << "Speed: " << movementSpeed_ << std::endl;
    }

    void DecrementMoveSpeed(double amount = DEFAULT_MOVEMENT_INCREMENT) {
        movementSpeed_ -= amount;
        if (movementSpeed_ < 0) {
            movementSpeed_ = 0;
        }
        std::cout << "Speed: " << movementSpeed_ << std::endl;
    }

    void IncrementLookSensitivity(double amount = DEFAULT_LOOK_INCREMENT) {
        lookSpeed += amount;
        std::cout << "Angle Change Speed: " << lookSpeed / M_PI * 180 << std::endl;
    }

    void DecrementLookSensitivity(double amount = DEFAULT_LOOK_INCREMENT) {
        lookSpeed -= amount;
        if (lookSpeed < 0) {
            lookSpeed = 0;
        }
        std::cout << "Angle Change Speed: " << lookSpeed / M_PI * 180 << std::endl;
    }

    /**
     * @param xs x coordinate scaled between -1 and 1
     * @param ys y coordinate scaled between -1 and 1
     * @return Ray
     */
    Ray CastRay(double xs, double ys) {
        return Ray(position_, direction_ * viewPlaneDistance_ + (yCross_ * xs) + (xCross_ * ys));
    }

    /**
     * Rotates the camera by given angles
     * @param yawAdd
     * @param pitchAdd
     */
    void Rotate(double yawAdd, double pitchAdd) {
        SetYaw(yawAdd + yaw_);
        SetPitch(pitchAdd + pitch_);
        UpdateCamera_();
        std::cout.setf(std::ios::fixed);
        std::cout.setf(std::ios::showpoint);
        std::cout.precision(2);
//        std::cout << "Yaw: " << yaw_ / M_PI * 180 << std::endl;
//        std::cout << "Pitch: " << pitch_ / M_PI * 180 << std::endl;
    }

    void Move(double x, double y, double z) { position_ += Vector(x, y, z); }

    void MoveForward() { position_ += direction_ * movementSpeed_; }

    void MoveBackward() { position_ += direction_ * -1 * movementSpeed_; }

    void MoveRight() { position_ += yCross_ * movementSpeed_; }

    void MoveLeft() { position_ += yCross_ * -1 * movementSpeed_; }

    void MoveUp() { position_ += xCross_ * movementSpeed_; }

    void MoveDown() { position_ += xCross_ * -1 * movementSpeed_; }

    void MoveUpAlongYaxis() { position_ += Vector(0, 1, 0) * movementSpeed_; }

    void MoveDownAlongYaxis() { position_ += Vector(0, -1, 0) * movementSpeed_; }

    void LookRight() { Rotate(lookSpeed, 0); }

    void LookLeft() { Rotate(-lookSpeed, 0); }

    void LookUp() { Rotate(0, lookSpeed); }

    void LookDown() { Rotate(0, -lookSpeed); }

private:
    void UpdateCamera_() {
        direction_ = Vector(std::sin(yaw_) * std::cos(pitch_),
                            std::sin(pitch_),
                            std::cos(yaw_) * std::cos(pitch_));
        yCross_ = (Vector(-std::cos(yaw_), 0, std::sin(yaw_))).Norm();
        xCross_ = yCross_.CrossProduct(direction_).Norm();
    }

    Vector position_;
    Vector direction_;
    double viewPlaneDistance_;
    double yaw_ = 0;
    double pitch_ = 0;
    double lookSpeed = 0.05; // Angle increment
    double movementSpeed_ = 1;
    Vector yCross_;
    Vector xCross_;
};

#endif //PATHTRACER_CAMERA_H
