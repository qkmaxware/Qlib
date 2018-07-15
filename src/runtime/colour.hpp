#ifndef _COLOUR_H
#define _COLOUR_H

#include <iostream>
#include <sstream>
#include <iomanip>

namespace graphics {

/// <Summary>
/// Colour represented in red, green, blue, alpha with values from 0 to 255.
/// </Summary>
struct colour {
    uint r;
    uint g;
    uint b;
    uint a;

    colour() : r(0), g(0), b(0), a(255) {}
    colour(uint r, uint g, uint b, uint a) : r(r), g(g), b(b), a(a) {}

    std::string toHex(){
        stringstream sb;
        sb << "#";
        sb << std::hex;
        if(r <= 16){
            sb << '0';
        }
        sb << (r % 256);

        if(g <= 16){
            sb << '0';
        }
        sb << (g % 256);

        if(b <= 16){
            sb << '0';
        }
        sb << (b % 256);
        return sb.str();
    }

    float getOpacity() {
        return (a % 256) / 255.0;
    }
};

}

#endif