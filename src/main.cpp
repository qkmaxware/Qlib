#include <iostream>

#include "./general/object.hpp"
#include "./math/complex.hpp"
#include "./math/matrix.hpp"
#include "./quantum/qubit.hpp"
#include "./quantum/qreg.hpp"
#include "./quantum/operator.hpp"

using namespace qlib::quantum;
using namespace qlib::math;

int main(){
    qubit q;

    
    std::cout << H.toString() << std::endl;
    std::cout << q.toString() << std::endl;

    return 0;
}
