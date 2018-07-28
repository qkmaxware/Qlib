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
    /// <Summary>
    /// Red
    /// </Summary>
    uint r;
    /// <Summary>
    ///Green
    /// </Summary>
    uint g;
    /// <Summary>
    /// Blue
    /// </Summary>
    uint b;
    /// <Summary>
    ///Alpha
    /// </Summary>
    uint a;

    /// <Summary>
    /// Solid Black
    /// </Summary>
    colour() : r(0), g(0), b(0), a(255) {}
    /// <Summary>
    /// Create a solid colour
    /// </Summary>
    colour(uint r, uint g, uint b) : r(r), g(g), b(b), a(255) {}
    /// <Summary>
    /// Create a transparent colour
    /// </Summary>
    colour(uint r, uint g, uint b, uint a) : r(r), g(g), b(b), a(a) {}

    /// <Summary>
    /// Create string in the form #RRGGBB
    /// </Summary>
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

    /// <Summary>
    /// Get opacity as a float in the range 0 to 1
    /// </Summary>
    float getOpacity() {
        return (a % 256) / 255.0;
    }
};

namespace colours {
    /// <Summary>
    /// Solid red
    /// </Summary>
    colour red(255,0,0);
    /// <Summary>
    /// Solid green
    /// </Summary>
    colour green(0,255,0);
    /// <Summary>
    /// Solid blue
    /// </Summary>
    colour blue(0,0,255);
    /// <Summary>
    /// Solid white
    /// </Summary>
    colour white(255,255,255);
    /// <Summary>
    /// Solid black
    /// </Summary>
    colour black(0,0,0);
    /// <Summary>
    /// Fully transparent colour
    /// </Summary>
    colour clear(0,0,0,0);
    /// <Summary>
    /// Solid teal
    /// </Summary>
    colour teal(0,255,255);
    /// <Summary>
    /// Solid purple
    /// </Summary>
    colour purple(255,0,255);
    /// <Summary>
    /// Solid yellow
    /// </Summary>
    colour yellow(255,255,0);
}

}

#endif