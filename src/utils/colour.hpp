/*
* Simple colour class to hold RGB colour information
*/

#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <SFML/Graphics/Color.hpp>
#include <iostream>

class Colour {

private:

    unsigned char red_;
    unsigned char green_;
    unsigned char blue_;


public:

    /*
    * New colour
    */
    Colour(unsigned int red, unsigned int green, unsigned int blue): red_(red), green_(green), blue_(blue) { }

    /* 
    * Color from sf::Color class
    */
    Colour(sf::Color colour): red_((unsigned char) colour.r), green_((unsigned char) colour.g), blue_((unsigned char) colour.b){}
    ~Colour() = default;
    
    // Colour component getters
    unsigned char red() const {return red_;}
    unsigned char green() const {return green_;}
    unsigned char blue() const {return blue_;}

    friend std::ostream &operator<<(std::ostream &os, Colour &colour); 

};

#endif // COLOUR_HPP
