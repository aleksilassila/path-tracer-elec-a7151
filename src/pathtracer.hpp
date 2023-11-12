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

    /**
     * @brief Get the Pixel Color
     * 
     * @param u 
     * @param v 
     * @param scene 
     * @return sf::Color 
     */
    sf::Color GetPixelColor(double u, double v, const Scene &scene);

    /**
     * @brief Test ray bounce direction on surface, by translating x, y and z componetnts of vector to r, g, and b
     * 
     * @param u 
     * @param v 
     * @param scene 
     * @return sf::Color 
     */
    sf::Color TestBounceDir(double u, double v, const Scene &scene);

    /**
     * @brief Get the nearest hit information struct
     * 
     * @param ray 
     * @param scene 
     * @return HitInfo 
     */
    HitInfo GetNearestHitInfo(const Ray &ray, const Scene &scene);

};







