#Qlib - Core: 
##1.2 Quantum Gates

##Table of contents
<!-- TOC -->

- [Qlib - Core:](#qlib---core)
    - [1.2 Quantum Gates](#12-quantum-gates)
    - [Table of contents](#table-of-contents)
        - [Included Quantum Gates](#included-quantum-gates)
        - [Custom Quantum Gates](#custom-quantum-gates)

<!-- /TOC -->

###Included Quantum Gates
Qlib - Core includes many pre-made gates that are commonly used. Table 1.2.0 lists each of the pre-made gates.

**Table 1.2.0**: List of all gates included in Qlib - Core. Each entry contains a gate name, the c++ namespace alias, the input parameters to use the gate, the matrix representation of the gate, and the description of what kind of operation the gate performs.
<table>
    <tr>
        <th>Gate</th>
        <th>Alias</th>
        <th>Parameters</th>
        <th>Matrix</th>
        <th>Description</th>
    </tr>
    <tr>
        <td>Identity</td>
        <td>gates::I</td>
        <td>x:qubit</td>
        <td>[1, 0]<br>[0, 1]</td>
        <td>Apply the identity matrix to qubit 'x'</td>
    </tr>
    <tr>
        <td>Hadamard</td>
        <td>gates::H</td>
        <td>x:qubit</td>
        <td>[1, 1] * (1/sqrt(2))<br>[1, -1]</td>
        <td>Creates an equal superposition of qubit 'x'</td>
    </tr>
    <tr>
        <td>Pauli X</td>
        <td>gates::X</td>
        <td>x:qubit</td>
        <td>[0, 1]<br>[1, 0]</td>
        <td>Apply quantum NOT gate to qubit 'x', also a rotation around the X-axis of the Bloch sphere</td>
    </tr>
    <tr>
        <td>Pauli Y</td>
        <td>gates::Y</td>
        <td>x:qubit</td>
        <td>[0, -i]<br>[i,  0]</td>
        <td>Apply rotation around the Y-axis of the Bloch sphere to qubit 'x'</td>
    </tr>
    <tr>
        <td>Pauli Z </td>
        <td>gates::Z</td>
        <td>x:qubit</td>
        <td>[1,  0]<br>[0, -1]</td>
        <td>Apply rotation around the Z-axis of the Bloch sphere to qubit 'x'</td>
    </tr>
    <tr>
        <td>Phase</td>
        <td>gates::phase(y)</td>
        <td>x:qubit<br> y:float</td>
        <td> [1, 0]<br>[0, cos(y) + isin(y)]</td>
        <td>Apply a change in phase with a factor of 'y' to qubit 'x'. This gate is a function that returns gate instances.</td>
    </tr>
    <tr>
        <td>T</td>
        <td>gates::T</td>
        <td>x:qubit</td>
        <td>phase(pi/8)</td>
        <td>Apply a change in phase with factor pi/8</td>
    </tr>
    <tr>
        <td>Contdolled NOT</td>
        <td>gates::CNOT<br> gates::CX</td>
        <td>c:qubit<br> t:qubit</td>
        <td>[1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 0, 1]<br>[0, 0, 1, 0]</td>
        <td>If qubit 'c' is set, apply quantum NOT gate to qubit 't'</td>
    </tr>
    <tr>
        <td>Controlled Y </td>
        <td>gates::CY</td>
        <td>c:qubit<br> t:qubit</td>
        <td>[1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 0, -i]<br>[0, 0, i, 0]</td>
        <td>If qubit 'c' is set, apply Pauli Y gate to qubit 't'</td>
    </tr>
    <tr>
        <td>Controlled Z</td>
        <td>gates::CZ</td>
        <td>c:qubit<br> t:qubit</td>
        <td>[1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 1, 0]<br>[0, 0, 0, -1]</td>
        <td>If qubit 'c' is set, apply Pauli Z gate to qubit 't'</td>
    </tr>
    <tr>
        <td>Toffoli</td>
        <td>gates::CCNOT<br> gates::TOFFOLI </td>
        <td>c1:qubit<br> c2:qubit<br> t:qubit</td>
        <td>[1, 0, 0, 0, 0, 0, 0, 0]<br>[0, 1, 0, 0, 0, 0, 0, 0]<br>[0, 0, 1, 0, 0, 0, 0, 0]<br>[0, 0, 0, 1, 0, 0, 0, 0]<br>[0, 0, 0, 0, 1, 0, 0, 0]<br>[0, 0, 0, 0, 0, 1, 0, 0]<br>[0, 0, 0, 0, 0, 0, 0, 1]<br>[0, 0, 0, 0, 0, 0, 1, 0]</td>
        <td>If qubit 'c1' and 'c2' are set, apply quantum NOT gate to qubit 't'</td>
    </tr>
</table>
###Custom Quantum Gates
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

*Example 1.2.0: Creating a custom quantum gate by implementing the igate interface.*
```cpp
class MyGate : public igate {
    public:
        std::string name(){
            return "MyGate";
        }
        size_t inputs(){
            return 1;
        }
        void operate(matrix& in, matrix& out, std::vector<ulong> inputQubits){
            ...
        }
};
```
In this example, the name of the gate is an arbitrary identifier "MyGate", the gate operates on only a single qubit, and the operate method will set the values in the output matrix **out** to the result of **U*****in**. 

To further simplify the creation of custom gates, there are 3 included sub-classes of **igate** which already contain the required functionality. These subclasses just require the matrix elements and the name in order to create a gate with a certain functionality. Each of these subclasses are listed below along with an example of their usage.
1) **onequbitgate**
Class representing a gate that operates on only a single qubit. One qubit gates take in a string identifer and a flattened (list) matrix of elements that represents a 2x2 unitary matrix.
```cpp
onequbitgate Y = onequbitgate(
    "Pauli-Y", 
    {
        complex(0,0), complex(0,-1),
        complex(0,1), complex(0,0)
    });
```
2) **controlledgate**
Class representing a gate that operates on a target qubit if a control qubit is set. Two qubit controlled gates take in a string identifer and a flattened (list) matrix of elements that represents a 2x2 unitary matrix.
```cpp
controlledgate CY = controlledgate(
    "CY", 
    {
        complex(0,0), complex(0,-1),
        complex(0,1), complex(0,0)
    });
```
3) **controlled2gate**
Class representing a gate that operates on a target qubit if both control qubits are set. Three qubit controlled gates take in a string identifer and a flattened (list) matrix of elements that represents a 2x2 unitary matrix.
```cpp
controlled2gate CCY = controlled2gate(
    "CCY", 
    {
        complex(0,0), complex(0,-1),
        complex(0,1), complex(0,0)
    });
```