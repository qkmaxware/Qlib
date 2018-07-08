#Qlib: 
##Quantum Computer Simulator, Runtime, and IDE
###About Qlib
Qlib is another quantum computer simulator designed with a focus on readability, simplicity of use, and no external dependencies. Part of this project is designed to act as an aid in learning about quantum computations and quantum algorithms. This project is actually grouped into 3 distinct sub-projects each with their own purposes. These projects are described below.

###Releases
For compiled executables see: [/releases](/releases)

###Qlib - Core
Qlib - Core is a c++ implemented quantum computer simulator. It allows for quantum gates to be applied to multi-qubit quantum registers. It can be readily implemented into any other c++ project and is a header only library. 

###Qlib - Runtime
Since compiling a new application every time you write a quantum algorithm can be a tedious effort, especially for large projects where c++ compilation can take from several minutes to several hours, the runtime was developed. The runtime is a command line program making use of the Qlib - Core that can quickly execute algorithms written in a quantum assembly language. While there are several examples of quantum programming languages in existence such as Quil and OpenQASM, I instead decided to implement my own language as I would not be using half of the provided features of those other languages and therefore I could simplify the syntax somewhat.

###Qlib - IDE
The Qlib - IDE is a front-end for the Qlib - Runtime. It is a java executable which contains basic syntax highlighting, code completion, and documentation. It can be used to quickly write scripts and then invoke them in the runtime without resorting to terminal commands. 

###Documentation
Extensive documentation and an auto-generated api are able to be accessed in the "doc" directory. This documentation includes installation instructions, examples, tutorials, implementation details, class and method descriptions, and defined constants.

###Notes
The Qlib project and all sub-projects are licensed under the MIT licence.
