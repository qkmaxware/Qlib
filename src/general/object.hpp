#include <sstream>

#ifndef _QLIB_GEN_OBJECT_H
#define _QLIB_GEN_OBJECT_H

class xobject {
    public:
        virtual std::string toString(){
            std::stringstream sb;
            sb << this;
            return sb.str();
        }
};

#endif