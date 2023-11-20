#include "pathtracer.hpp"

PathTracer::~PathTracer() = default;

//sf::Color PathTracer::TestBounceDir(double u, double v, Scene &scene) {
//
//    sf::Color avgCol = sf::Color::Black;
//
//    Camera camera = scene.GetCamera();
//    // start with ray from camera
//    ray_ = camera.CastRay(u, v);
//    HitInfo lastHit_ = GetNearestHitInfo(ray_, scene);
//    Vector bounceDir = lastHit_.sMaterial.findSpecularBounceDirection(ray_, lastHit_.sNormal, 1).Norm();
//
//    if (lastHit_.hit)
//        avgCol = sf::Color(bounceDir.x() * 255, bounceDir.y() * 255, bounceDir.z() * 255);
//
//    return avgCol;
//}