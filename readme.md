#Qlib: 
##Quantum Computer Simulator

Qlib is a standalone C++ library, with no external dependancies, that simulates a "ideal" Quantum Computer.

A quantum computer is a computer where instead of operating on bits, it opertates on quantum bits 'qubits'. What is special about quantum bits is that they do not have a specific value. Instead they have some probability to be in any number of states. Classically, a bit can either be 0 or 1 whereas a qubit can be in some superposition of 0 and 1
```
Classic Bit:   x  = 0 or 1
Quantum Bit:  |x> = a * |0> + b * |1>
```
In the above description, a quantum bit is in state 0 with a probability of **a*a** and in state 1 with a probability of **b*b** where a and b are both [complex numbers](https://en.wikipedia.org/wiki/Complex_number). By applying unitary operators to this superposition of states, we can manipulate the probabilities to favor certain outcomes. We can determine the actual value of a quantum bit by measuring it. This interaction results in a "collapse" where the qubit is forced to choose if it is 0 or 1 and loses its superposition. Due to the probabilities involved, multiple identical qubits may produce different results when measured.

##Installation
###Linux
```
git clone 
cd qlib
make build
make run
```

##Reference Guide
###Types of systems
In Qlib, we have 3 types of system which can be used to perform quantum simulations.
```
qlib::quantum::qubit        - Single simulated qubit
qlib::quantum::qreg         - Register of multiple simulated qubits
qlib::quantum::ensemble     - Collection of multiple identical quantum systems
```
All of these systems share the same interface
```cpp
//----------------------------------------------------------
// Interface definition
//----------------------------------------------------------
class qsystem: public xobject {
    private:
    public:
        //Apply a quantum gate to this system
        virtual void apply(qlib::quantum::gates::igate& gate,std::vector<ulong> inputBits) = 0;
        //Collapse the state and return the value of the measured qubit
        virtual i32 measure(i32 qubit) = 0;
        //Human readable output
        virtual std::string toString(){
            return xobject::toString();
        };
};
```
###Gates
The following list is all the unitary gates that are included in QLib. 
| Gate | Alias | Parameters | Matrix | Description |
|---|---|---|---|---|
|  Identity | gates::I | x:qubit | [1, 0]<br>[0, 1] | Apply the identity matrix to qubit 'x' |
|  Hadamard | gates::H | x:qubit | [1, 1] * (1/sqrt(2))<br>[1, -1] | Creates an equal superposition of qubit 'x' |
|  Pauli X | gates::X | x:qubit | [0, 1]<br>[1, 0] | Apply quantum NOT gate to qubit 'x', also a rotation around the X-axis of the Bloch sphere |
|  Pauli Y | gates::Y | x:qubit | [0, -i]<br>[i,  0] | Apply rotation around the Y-axis of the Bloch sphere to qubit 'x' |
|  Pauli Z | gates::Z | x:qubit | [1,  0]<br>[0, -1] | Apply rotation around the Z-axis of the Bloch sphere to qubit 'x' |
|  Phase | gates::phase(y) | x:qubit, y:float | [1, 0               ]<br>[0, cos(y) + isin(y)] | Apply a change in phase with a factor of 'y' to qubit 'x'. This gate is a function that returns gate instances. |
|  T | gates::T |  | phase(pi/8) | Apply a change in phase with factor pi/8 |
|  Controlled NOT | gates::CNOT, gates::CX | c:qubit, t:qubit | [1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 0, 1]<br>[0, 0, 1, 0] | If qubit 'c' is set, apply quantum NOT gate to qubit 't' |
|  Controlled Y | gates::CY | c:qubit, t:qubit | [1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 0, -i]<br>[0, 0, i, 0] | If qubit 'c' is set, apply Pauli Y gate to qubit 't' |
|  Controlled Z | gates::CZ | c:qubit, t:qubit | [1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 1, 0]<br>[0, 0, 0, -1] | If qubit 'c' is set, apply Pauli Z gate to qubit 't' |
|  Toffoli | gates::CCNOT, gates::TOFFOLI | c1:qubit, c2:qubit t:qubit | [1, 0, 0, 0, 0, 0, 0, 0]<br>[0, 1, 0, 0, 0, 0, 0, 0]<br>[0, 0, 1, 0, 0, 0, 0, 0]<br>[0, 0, 0, 1, 0, 0, 0, 0]<br>[0, 0, 0, 0, 1, 0, 0, 0]<br>[0, 0, 0, 0, 0, 1, 0, 0]<br>[0, 0, 0, 0, 0, 0, 0, 1]<br>[0, 0, 0, 0, 0, 0, 1, 0] | If qubit 'c1' and 'c2' are set, apply quantum NOT gate to qubit 't' |
You can create your own quantum gates by extending the qlib::quantum::gates::igate interface. 
```cpp
//----------------------------------------------------------
// Interface definition
//----------------------------------------------------------
class igate : public xobject {
    private:
    public:
        //Name of the gate
        virtual std::string name() = 0;
        //Number of qubits it operates on
        virtual size_t inputs() = 0;
        //Using the initial state vector 'in' update the state vector 'out' using the qubit numbers provided in 'inputQubits' list
        virtual void operate(matrix& in, matrix& out, std::vector<ulong> inputQubits) = 0;
};
```

##References
[qHiPSTER](https://arxiv.org/abs/1601.07195)  - Mikhail Smelyanskiy, Nicolas P.D Sawaya, Alan Aspuru-Guzik - Jan 2016 