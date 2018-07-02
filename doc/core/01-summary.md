#Qlib - Core: 
##1.1 Quantum Computer Simulator

Qlib is a standalone C++ library, with no external dependancies, that simulates a n"ideal" Quantum Computer.

A Auantum computer is a computer where instead of operating on bits, it opertates on quantum bits otherwise known as 'qubits'. What is special about quantum bits is that they do not have a specific value. Instead they have some probability to be in any number of states. Classically, a bit can either be 0 or 1 whereas a qubit will have some probability that it is 1 and some probability that it is 0. In other words, a qubit exists in some arbitrary superposition of 0 and 1 with no definite value until it is measured. The section below indicates the possible values of both classical bits and a quantum mechanical bits.
```
Classic Bit:   x  = 0 or 1
Quantum Bit:  |x> = a * |0> + b * |1>
```
In the above description, a quantum bit is in state 0 with a probability of **a*a** and in state 1 with a probability of **b*b** where a and b are both [complex numbers](https://en.wikipedia.org/wiki/Complex_number). By applying unitary operators to this superposition of states, we can manipulate the probabilities to favor certain outcomes. We can determine the actual value of a quantum bit by measuring it. This interaction results in a "collapse" where the qubit is forced to choose if it is 0 or 1 and loses its superposition. Due to the probabilities involved, multiple identical qubits may produce different results when measured.

##Installation
To use Qlib - Core in your own projects, simply download the `src/core` folder or clone the whole repository using `git clone <repo>`. Then simply include a reference to the qlib.h header file in your own project. This header is a single file that wraps up the entirety of the Qlib - Core code. You can then use all aspects of Qlib within your own applications. Example 1.0.0 shows how this file inclusion can be done assuming the repository was cloned into a folder "qlib" at the root directory of the project.

*Example 1.0.0: Including the qlib core header into a custom c++ project*
```cpp
#include "./qlib/src/core/qlib.h"
```

##Usage
A sample calculation is provided below in example 1.0.1 in which we declare a quantum register of 2 qubits, apply 2 gates, and then measure the quantum system to force the register to take on a specific state. See section 1.3 for a list of all the quantum gates that are pre-programmed in Qlib - Core including the gates used in this example.

*Example 1.0.1: Sample calculation on a 2 qubit register*
```cpp
#include <iostream>
#include "./qlib/src/core/qlib.h"

using namespace qlib::quantum;          //Quantum namespace to access qubit, qreg, ensemble
using namespace std;                    //Std namspace to access cout

int main(){
    //Create a quantum register of 2 qubits and print the superposition to terminal
    //All registers / bits are prepared in the |0..0> state
    qreg r(2);
    cout << r.toString() << endl;

    //Apply the Hadamard 'H' gate to qubit 0 of the register
    r.apply(gates::H, {0});
    //Apply the Controlled-Not 'CNOT' or 'CX' to control qubit 0 and target qubit 1
    r.apply(gates::CNOT, {0, 1});

    //Print out new superposition
    cout << r.toString() << endl;

    //Measure both qubits and collapse the state-vector. 
    //Will be either |00> or |11> with approximately 50% chance of either
    r.measure(0);
    r.measure(1);
    cout << r.toString() << endl;

    return 0;
}
```