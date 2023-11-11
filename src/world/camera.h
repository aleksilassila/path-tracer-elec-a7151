//
// Created by Aleksi Lassila on 4.11.2023.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "../utils/ray.hpp"
#include <cmath>

/**
 * Camera class is essentially a ray with some
 * extra properties and functionality such as fov
 * and rendering related stuff
 */
class Camera2 : public Ray {

public:
    Camera2() : Camera2(Vector(0, 0, 0), Vector(0, 0, 1)) {}

    Camera2(Vector origin, Vector direction) : Ray(origin, direction),
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


/**
 * Camera class is essentially a ray with some
 * extra properties and functionality such as fov
 * and rendering related stuff
 */
// New camera with modifiable position and direction
// Direction given in sphere coordinate angles phi and theta

class Camera {

public:
    Camera(Vector position = Vector(0, 0, 0), double vpd = 1, double phi = 0, double theta = 0.001) :
        position_(position), viewPlaneDistance_(vpd), phi_(phi), theta_(theta) {
        UpdateCamera();
    }

    void SetPosition(const Vector &position) { position_ = position; }

    Vector GetPosition() { return position_ ; }

    void SetViewPlaneDistance(double vpd) { viewPlaneDistance_ = vpd; }

    [[nodiscard]] double GetViewPlaneDistance() const { return viewPlaneDistance_; }

    void SetPhi(double phi) {
        phi_ = phi;
        UpdateCamera();
    }

    [[nodiscard]] double GetPhi() const {return phi_;}

    void SetTheta(double theta) {
        theta_ = theta;
        UpdateCamera();
    }

    [[nodiscard]] double GetTheta() const {return theta_;}

    void SetDirection() {
        double x = sin(theta_) * cos(phi_);
        double y = sin(theta_) * sin(phi_);
        double z = cos(theta_);
        direction_ = Vector(x, y, z);
    }

    [[nodiscard]] Vector GetDirection() const {return direction_;}

    void Set_u_theta() {
        double x = cos(theta_) * cos(phi_);
        double y = cos(theta_) * sin(phi_);
        double z = -sin(theta_);
        u_theta_ = Vector(x, y, z);
    }

    [[nodiscard]] Vector Get_u_theta() const {return u_theta_;}

    void Set_u_phi() {
        double x = -sin(phi_);
        double y = cos(phi_);
        u_phi_ = Vector(x, y, 0);
    }

    [[nodiscard]] Vector Get_u_phi() const {return u_phi_;}

    void UpdateCamera() {
        SetDirection();
        Set_u_phi();
        Set_u_theta();
        //std::cout << "Camera updated" << std::endl;
        //std::cout << "Phi: " << phi_ << std::endl;
        //std::cout << "Theta: " << theta_ << std::endl;
        //std::cout << "u_phi: " << u_phi_<< std::endl;
        //std::cout << "u_theta: " << u_theta_ << std::endl;

    }

    Ray GetRay(double u, double v) {
        Ray ray = Ray(position_, (u_phi_ * u + u_theta_ * v + direction_ * viewPlaneDistance_).Norm());
        //std::cout << u << std::endl;
        //std::cout << "u_phi: " << u_phi_ << std::endl;
        //std::cout << "u_theta: " << u_theta_ << std::endl;
        //std::cout << v << std::endl;
        //std::cout << ray.GetOrigin() << std::endl;
        //std::cout << ray.GetDirection() << std::endl;

        return ray;
    }

private:
    Vector position_;
    double viewPlaneDistance_;
    double phi_ = 0;
    double theta_ = 0.0001; //M_PI / 2;
    Vector direction_; // = Vector(0, 0, 1);
    Vector u_phi_;
    Vector u_theta_;
};

#endif //PATHTRACER_CAMERA_H
