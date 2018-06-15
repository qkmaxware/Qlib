#include <iostream>
#include <vector>

#include "./general/object.hpp"
#include "./math/complex.hpp"
#include "./math/matrix.hpp"
#include "./quantum/systems/qubit.hpp"
#include "./quantum/systems/qreg.hpp"
#include "./quantum/standard_gates.hpp"
#include "./quantum/systems/system.hpp"
#include "./quantum/systems/ensemble.hpp"

using namespace qlib::quantum;
using namespace qlib::math;
using namespace std;

int main(){
    qubit q;
    cout << q.toString() << endl;
    q.apply(gates::H, args<ulong>({0}));
    cout << q.toString() << endl;
    /*
    std::cout << "Gates" << std::endl;
    std::cout << H.toString() << std::endl;
    std::cout << X.toString() << std::endl;
    std::cout << Y.toString() << std::endl;
    std::cout << Z.toString() << std::endl;

    std::cout << std::endl << "Building Ensemble" << std::endl;
    std::vector<qsystem*> systems;
    for(int i = 0; i < 10; i++){
        systems.push_back(new qubit());
    }
    ensemble e(systems);
    std::cout << " -- Size: " << e.size() << std::endl;

    std::cout << std::endl << "Operating" << std::endl;
    e.apply(H);

    std::cout << std::endl << "Measurements" << std::endl;
    std::cout << e.measure(0) << std::endl;
    */
    return 0;
}
