#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <SFML/Graphics/Color.hpp>
#include <iostream>

/**
 * Simple colour class to hold RGB colour information
 */
class Colour {

private:
    unsigned char red_;
    unsigned char green_;
    unsigned char blue_;

public:
    Colour(unsigned int red, unsigned int green, unsigned int blue): red_(red), green_(green), blue_(blue) { }

    /**
     * Color constructor using sf::Color
     * @param colour
     */
    explicit Colour(sf::Color colour): red_((unsigned char) colour.r), green_((unsigned char) colour.g), blue_((unsigned char) colour.b){}
    ~Colour() = default;
    
    // Colour component getters
    unsigned char GetRed() const {return red_;}
    unsigned char GetGreen() const {return green_;}
    unsigned char GetBlue() const {return blue_;}
};

#endif // COLOUR_HPP
