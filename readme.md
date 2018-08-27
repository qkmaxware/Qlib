# Qlib: Quantum Computer Simulator, Runtime, and IDE
### About Qlib
Qlib is a quantum computer simulator designed as a tool for learning about quantum computers and quantum algorithms. It is designed with a focus on readable code without any external dependancies, using only features provided natively in the C++, Java, JavaScript libraries. Usage of any part of this repository is Licenced under the [MIT](https://github.com/qkmaxware/Qlib/blob/master/LICENSE.txt) license.

### Project Structure
This project is split into several sub-projects, each one serving a very specific purpose. The source code for each of the projects can be found in its own folder under the `/src` directory in the project hierarchy.

| Project | Description |
|---|---|
| Qlib Core | This is the core of the simulator written in C++. It features all the code required to simulate quantum bits, quantum registers, and ensembles of many registers. |
| Qlib Runtime | A command line tool written in C++ used to run scripts written in a form of Quantum Assembly (QAS). |
| Qlib IDE | A Java application with some syntax highlighting and the ability to invoke the runtime from within the application for rapid script testing. |
| Qlib VsCode | A simple Visual Studio Code extension with syntax highlighting for the QAS langauge. |

- [Qlib: Quantum Computer Simulator, Runtime, and IDE](#qlib-quantum-computer-simulator-runtime-and-ide)
    - [About Qlib](#about-qlib)
    - [Project Structure](#project-structure)
    - [Releases](#releases)
    - [Qlib - Core](#qlib---core)
        - [Installation](#installation)
        - [Types of Simulated Systems](#types-of-simulated-systems)
        - [Quantum Gates](#quantum-gates)
            - [Included gates](#included-gates)
            - [Custom Quantum Gates](#custom-quantum-gates)
        - [Usage Examples](#usage-examples)
    - [Qlib - Runtime](#qlib---runtime)
        - [Obtaining a copy](#obtaining-a-copy)
        - [Compiling your own copy](#compiling-your-own-copy)
        - [QAS](#qas)
    - [Qlib - IDE](#qlib---ide)
        - [Obtaining a copy](#obtaining-a-copy-1)
        - [Compiling your own copy](#compiling-your-own-copy-1)
    - [Documentation](#documentation)

### Releases
For compiled executables of the Runtime, IDE, and VsCode extension please see the [/releases](https://github.com/qkmaxware/Qlib/releases) section.

You can also find a version of the Runtime as a Docker container [here](https://hub.docker.com/r/qkmaxware/qlib/).

### Qlib - Core
Qlib - Core: A header only quantum computer simulator written in C++14 (h and hpp files). 

#### Installation
1. Obtain a copy of the Qlib - Core source code
    - Use git to clone the repository to your local machine
    ```git clone https://github.com/qkmaxware/Qlib.git```
    - Download a compressed copy of the repository from the [/releases](https://github.com/qkmaxware/Qlib/releases) or from the top of the GitHub page under "Clone or Download".
2. Start a new C++ project which will use Qlib - Core. The build method and IDE do not matter; however, the Core was compiled using `GCC` and `Make` on Ubuntu. 
3. Copy the `/src/core` code into the section of your project for external libraries. I typically put this code into something like `/lib/qlib` in my own projects.
4. Add the following `include` to the main C++ file. This file will include all the components of Qlib - Core for you.
    ```cpp
    #include "./lib/qlib/qlib.h"
    ```
5. Use any component of Qlib - Core

#### Types of Simulated Systems
| System | Qlib - Core Namespace| Description |
|---|---|---|
| Qubit | qlib::quantum::qubit | Class representing a single quantum bit. |
| Quantum Register | qlib::quantum::qreg | Class representing a register of many quantum bits. The register must have a minimum of '1' qubit. |
| Ensemble | qlib::quantum::ensemble | Class representing a collection of many "identical" quantum registers. Using this class is the same as creating many quantum register instances and applying the same operations to each one. |

#### Quantum Gates
##### Included gates
| Gate | Qlib - Core Namespace | Parameters | Matrix | Description |
|---|---|---|---|---|
| Identity | gates::I | x:qubit | [1, 0]<br>[0, 1] | Apply the identity matrix to qubit 'x'. This doesn't affect the state of the qubit. |
| Hadamard | gates::H | x:qubit | [1, 1] * (1/sqrt(2))<br>[1, -1] | Puts qubit 'x' in an equal superposition of 0 and 1. |
| Pauli X | gates::X | x:qubit | [0, 1]<br>[1, 0] | Apply quantum NOT gate to qubit 'x', also a rotation around the X-axis of the Bloch sphere |
| Pauli Y | gates::Y | x:qubit | [0, -i]<br>[i, 0] | Apply rotation around the Y-axis of the Bloch sphere to qubit 'x' |
| Pauli Z | gates::Z | x:qubit | [1, 0]<br>[0, -1] | Apply rotation around the Z-axis of the Bloch sphere to qubit 'x' |
| Phase | gates::phase(y) | x:qubit, y:float | [1, 0 ]<br>[0, cos(y) + isin(y)] | Apply a change in phase with a factor of 'y' to qubit 'x'. This gate is a function that returns gate instances. |
| T | gates::T | | phase(pi/8) | Apply a change in phase with factor pi/8 |
| Controlled NOT | gates::CNOT, gates::CX | c:qubit, t:qubit | [1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 0, 1]<br>[0, 0, 1, 0] | If qubit 'c' is set, apply quantum NOT gate to qubit 't' |
| Controlled Y | gates::CY | c:qubit, t:qubit | [1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 0, -i]<br>[0, 0, i, 0] | If qubit 'c' is set, apply Pauli Y gate to qubit 't' |
| Controlled Z | gates::CZ | c:qubit, t:qubit | [1, 0, 0, 0]<br>[0, 1, 0, 0]<br>[0, 0, 1, 0]<br>[0, 0, 0, -1] | If qubit 'c' is set, apply Pauli Z gate to qubit 't' |
| Toffoli | gates::CCNOT, gates::TOFFOLI | c1:qubit, c2:qubit t:qubit | [1, 0, 0, 0, 0, 0, 0, 0]<br>[0, 1, 0, 0, 0, 0, 0, 0]<br>[0, 0, 1, 0, 0, 0, 0, 0]<br>[0, 0, 0, 1, 0, 0, 0, 0]<br>[0, 0, 0, 0, 1, 0, 0, 0]<br>[0, 0, 0, 0, 0, 1, 0, 0]<br>[0, 0, 0, 0, 0, 0, 0, 1]<br>[0, 0, 0, 0, 0, 0, 1, 0] | If qubit 'c1' and 'c2' are set, apply quantum NOT gate to qubit 't' |

##### Custom Quantum Gates
You can create your own quantum gate in one of two ways. 
1. Use the onequbitgate, controlledgate, and controlled2gate classes to create a gate quickly providing, only a 2x2 matrix, and a name to the constructor. 
```cpp
//Definition of Pauli-Z using onequbitgate as the base class
onequbitgate Z = onequbitgate(
    "Pauli-Z", 
    {complex(1), complex(0),
    complex(0), complex(-1)}
);
```
```cpp
//Definition of the Controlled Pauli-Z using controlledgate as the base class and a reference to the Pauli-Z gate
controlledgate CZ = controlledgate(
    "CZ", 
    Z
    );
);
```
```cpp
//Definition of the two qubit controlled "Toffoli" gate using controlled2gate as the base class and a reference to the Pauli-X gate
controlled2gate CCNOT = controlled2gate(
    "CCNOT", 
    X
    );
```
2. Extend the [igate](https://github.com/qkmaxware/Qlib/blob/master/src/core/quantum/gates/igate.hpp) interface. Every quantum gate in Qlib - Core extends from this interface. You can use it to create your own quantum gates from scratch.

#### Usage Examples
1. Creating a quantum bit and operating on it with the Hadamard gate.
```cpp
#include <iostream>
#include "qlib.h"

using namespace qlib::quantum;
using namespace std;

int main(){
    //Create qubit
    qubit q;
    //Apply the built in Hadamard Gate
    q.apply(gates::H, {0});
}
```
2. Creating a quantum register and entangling the quantum bits.
```cpp
#include <iostream>
#include "qlib.h"

using namespace qlib::quantum;
using namespace std;

int main(){
    //Create a 2 qubit register
    qreg q(2);
    //Apply the built in Hadamard Gate to qubit 0
    q.apply(gates::H, {0});
    //apply the CNOT gate to qubit 1 using 0 as a control
    q.apply(gates::CNOT, {0,1});
}
```
3. Applying the Z gate to 5 identical systems of only a single qubit each.
```cpp
#include <iostream>
#include "qlib.h"

using namespace qlib::quantum;
using namespace std;

int main(){
    //Create an ensemble of 5 independant qubits
    ensemble q(1, 5);
    //Apply the built in Z Gate to qubit 0 of each system in the ensemble
    q.apply(gates::Z, {0});
}
```

### Qlib - Runtime
Since compiling a new application every time you want to write a quantum algorithm can be a tedious effort, especially for large projects where C++ compilation can take from several minutes to several hours, the runtime was developed. The runtime is a command line program making use of the Qlib - Core that can quickly execute algorithms written in a quantum assembly language. While there are several examples of quantum programming languages in existence such as Quil and OpenQASM, I instead decided to implement my own language as I would not be using half of the provided features of those other languages and therefore I could simplify the syntax somewhat.

#### Obtaining a copy
See [Releases](#releases)

#### Compiling your own copy
These instructions are only provided for Ubuntu/Debian based operating systems. For Windows/Mac, you will need to use a C++14 enabled compiler and compile the `/src/runtime/main.cpp` file.
1. Obtain a copy of the Qlib - Runtime source code
    - Use git to clone the repository to your local machine
    ```git clone https://github.com/qkmaxware/Qlib.git```
    - Download a compressed copy of the repository from the [/releases](https://github.com/qkmaxware/Qlib/releases) or from the top of the GitHub page under "Clone or Download".
2. Navigate to the directory 
    ```cd Qlib```
3. Ensure you have `build essentials` installed. The makefile provided can be used to make sure you have the required tools. 
    - For linux compilation
    ```make install-linux-tools```
    - For windows cross-compilation
    ```make install-win-tools```
4. Build the runtime
    - For linux
    ```make build``` or ```make build-linux```
    - For windows
    ```make build-win```
    - For all
    ```make build-all```

#### QAS
QAS (Quantum Assembly Language) is the name I have given to the scripting language used by the Qlib - Runtime. This simple scripting language let's you perform the same operations as provided by the Qlib - Core, but with simpler syntax. Examples of QAS scripts are provided under the [/examples](https://github.com/qkmaxware/Qlib/tree/master/examples) folder and are based on algorithms from chapters of: `Kaye, P., Laflamme, R., & Mosca, M. (2007). An introduction to quantum computing. New York: Oxford University Press`. The syntax of the QAS language as it currently stands is expressed in EBNF grammar below.
```ebnf
(* Production Rules *)
script      =   { statement };
statement   =   include | declaration | operator | directive | measurement;

include     =   'include' STRING;
declaration =   ( 'qreg' | 'creg' ) id '[' INTEGER ']';
operator    =   id { qubit };
directive   =   NAME { id };
measurement =   'measure' ( id | qubit ) '->' ( id | bit );

id          = NAME;
qubit       = NAME INTEGER; 
bit         = NAME INTEGER;

(* Lexigraphics Rules *)
NAME        = ? [a-zA-Z][a-zA-Z]+ ?;
STRING      = '"' ? .* ? '"';
INTEGER     = ? [0-9]+ ?;
```

### Qlib - IDE
The Qlib - IDE is a front-end for the Qlib - Runtime. It is a java executable which contains basic syntax highlighting and code suggestions for the QAS language. It can be used to quickly write scripts and then invoke them in the runtime without resorting to terminal commands. 

#### Obtaining a copy 
See [Releases](#releases)

#### Compiling your own copy
1. Obtain a copy of the Qlib - IDE source code
    - Use git to clone the repository to your local machine
    ```git clone https://github.com/qkmaxware/Qlib.git```
    - Download a compressed copy of the repository from the [/releases](https://github.com/qkmaxware/Qlib/releases) or from the top of the GitHub page under "Clone or Download".
2. Open the `/src/ide` project folder in NetBeans IDE (or import Netbeans project into Eclipse)
3. Build using your IDE's default build method.

### Documentation
Documentation can be found in the `/docs` folder of the repository. These documents are not guaranteed to be up to date. 

You can generate your own copy of the API directory from source code using Doxygen. This can be done with the `make docs` command from the root of the repository or by using doxygen directly. 