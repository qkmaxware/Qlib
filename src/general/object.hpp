#include "./types.h"
#include "./args.hpp"

#include <iostream>
#include <sstream>
#include <exception>
#include <random>
#include <chrono>

#ifndef _QLIB_GEN_OBJECT_H
#define _QLIB_GEN_OBJECT_H

//Define random number generator
std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());

class xobject {
    public:
        virtual std::string toString(){
            std::stringstream sb;
            sb << this;
            return sb.str();
        }
};

#endif