#include "pathtracer.hpp"

PathTracer::PathTracer(): ray_(Vector(), Vector()), maxBounces_(4){ }
PathTracer::~PathTracer() {}


HitInfo PathTracer::GetNearestHitInfo(const Ray &ray, const Scene &scene) {

    HitInfo hit;
    double minDistance = std::numeric_limits<double>::max();

    for (const auto &object: scene.GetObjects()) {

        //Vector intersectionPoint = object->GetIntersectionPoint(ray);
        // Used for finding the closest intersection to ray origin.
        //double intersectionDistance = intersectionPoint.Distance(ray.GetOrigin());
        double intersectionDistance = object->GetIntersectionDistance(ray);

        if (intersectionDistance < minDistance && intersectionDistance > 0) {

            minDistance = intersectionDistance; // Update

            // Update hit info
            hit.hit = true;
            //hit.point = intersectionPoint;
            hit.point = object->GetIntersectionPoint(ray);
            hit.sMaterial = object->GetMaterial();
            //hit.sNormal = object->Normal(intersectionPoint);
            hit.sNormal = object->Normal(hit.point);

        }
    }

    return hit;
}

sf::Color PathTracer::TestBounceDir(double u, double v, const Scene &scene){

    sf::Color avgCol = sf::Color::Black;

    Camera camera = scene.GetCamera();
    // start with ray from camera
    ray_ = camera.GetRay(u, v);
    lastHit_ = GetNearestHitInfo(ray_, scene);
    Vector bounceDir = lastHit_.sMaterial.findBounceDirection(ray_ ,lastHit_.sNormal).Norm();

    if (lastHit_.hit)
        avgCol = sf::Color(bounceDir.x() * 255, bounceDir.y() * 255, bounceDir.z() * 255);
    
    return avgCol;
}


sf::Color PathTracer::GetPixelColor(double u, double v, const Scene &scene) {

    // Initialize color components as white
    double R = 1, G = 1, B = 1;
    double light = 0;

    Camera camera = scene.GetCamera();

    //Start with camera ray.
    ray_ = camera.GetRay(u, v);

    for (int b = 0; b < maxBounces_; b++) {

        lastHit_ = GetNearestHitInfo(ray_, scene);

        if (lastHit_.hit) {

            sf::Color surfaceCol = lastHit_.sMaterial.getColor();
            Vector point = lastHit_.point;
            Vector bounceDir = lastHit_.sMaterial.findBounceDirection(ray_ ,lastHit_.sNormal).Norm();
            light += lastHit_.sMaterial.getEmission();
            if (light > 0) break;
            
            // Update color
            R *= (surfaceCol.r / 255.0);
            G *= (surfaceCol.g / 255.0);
            B *= (surfaceCol.b / 255.0);

            // Update ray
            ray_.SetOrigin(point);
            ray_.SetDirection(bounceDir);


        } else {
            // if ray does not hit anything, break the loop
            break;
        }
    }

    // Apply accumulated light to the color and return
    return sf::Color(R * 255 * light, G * 255 * light, B * 255 * light);
}