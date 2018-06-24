##Reference Guide
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

You can create your own quantum gates by extending the qlib::quantum::gates::igate interface and implementing all the virtual methods below.
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