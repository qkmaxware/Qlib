#include <iostream>

#include "types.h"
#include "./math/complex.hpp"
#include "./math/matrix.hpp"
#include "./quantum/qreg.hpp"

int main(){
    qlib::math::matrix a(2,2);
    a(0,0) = 1;
    a(0,1) = 2;
    a(1,0) = 3;
    a(1,1) = 4;

    qlib::math::matrix b(2,2);
    b(0,0) = 1;
    b(0,1) = 2;
    b(1,0) = 3;
    b(1,1) = 4;

    //TENSOR PRODUC
    /*
    (1 2)(1 2) =    (1 2 2 4)
    (3 4)(3 4)      (3 4 6 8)
                    (3 6 4 8)
                    (9 12 12 16)
    */
    qlib::math::matrix t = a << b;

    //std::cout << a.toString() << std::endl;
    std::cout << t.toString() << std::endl;

    return 0;
}
