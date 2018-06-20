#include <iostream>
#include <vector>

#include "qlib.h"

using namespace qlib::quantum;
using namespace qlib::math;
using namespace std;

int main(){
    /*
    qubit q;
    cout << q.toString() << endl;
    q.apply(gates::H, {0});
    cout << q.toString() << endl;
    */
    qreg r(2);
    cout << r.toString() << endl;
    r.apply(gates::H, {0});
    cout << r.toString() << endl;
    r.apply(gates::CNOT, {0, 1});
    cout << r.toString() << endl;
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
