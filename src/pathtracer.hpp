#include "SFML/Graphics/Color.hpp"
#include "utils/ray.hpp"
#include "world/scene.h"

struct HitInfo {

    // Did ray hit an object
    bool hit;
    //point hit
    Vector point;
    // Surface normal of point hit
    Vector sNormal;
    // Material of object hit
    Material sMaterial;

    HitInfo() : hit(false), point(), sNormal(), sMaterial() { }
};



class PathTracer {

private:

    HitInfo lastHit_;
    Ray ray_;
    int maxBounces_;

    
public:

    PathTracer();
    ~PathTracer();

    sf::Color GetPixelColor(double u, double v, const Scene &scene);
    sf::Color TestBounceDir(double u, double v, const Scene &scene);
    sf::Color AccumulatePixelColour(double u, double v, const Scene &scene);
    HitInfo GetNearestHitInfo(const Ray &ray, const Scene &scene);
    // sf::Color GetAvgColor(const std::vector<sf::Color> &colors);

};







