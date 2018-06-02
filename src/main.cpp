#include <iostream>

#include "types.h"
#include "./math/complex.hpp"
#include "./math/matrix.hpp"

int main(){
    qlib::math::complex c(4,5);
    qlib::math::matrix m(2,2);

    m(0,0) = qlib::math::complex(1);
    m(1,1) = qlib::math::complex(4);

    std::cout << c.toString() << std::endl;
    std::cout << m.toString() << std::endl;

    return 0;
}
