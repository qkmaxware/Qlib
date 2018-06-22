#include "./types.h"

#include <iostream>
#include <sstream>
#include <exception>
#include <random>
#include <chrono>

#ifndef _QLIB_GEN_OBJECT_H
#define _QLIB_GEN_OBJECT_H

/// <Summary>
///Global random number generator, seed is based on current system time
/// </Summary>
std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());

/// <Summary>
///Base class for all objects
/// </Summary>
class xobject {
    public:
        /// <Summary>
        ///Creates and std::string representation of this object. (default: object address)
        /// </Summary>
        virtual std::string toString(){
            std::stringstream sb;
            sb << this;
            return sb.str();
        }
};

#endif