#Qlib - Runtime:
##Runtime environment for Qlib - Core.
Qlib - Runtime is an executable runtime environment for executing scripts written in a custom quantum assembly language called **QAS**(Quantum. Assembly). 

##Installation
####linux
```
git clone <repo>
cd qlib
make build
```
You will find your executable in **/bin/run** directory of your local repository.
###windows
Build windows executable on a linux machine
```
git clone <repo>
cd qlib
make install-win-tools
make build-win
```
For 32 bit machines, you will find the executable at **/bin/run.w32.exe**
For 64 bit machines, you will find the executable at **/bin/run.w64.exe**
Copy the file to your windows machine

##Usage
The runtime is a command line utility to parse, and run QAS scripts using the Qlib - Core backend. To invoke the runtime, simply use terminal/cmd/powershell to execute the binary and pass in the path to the script file.
```
> <path-to-binary> <path-to-file>
```
for example
```
> ./qlib.exe ./my_script.qas
```

##QAS
####Summary
QAS is the programming language that is used by the Qlib - Runtime. Syntactically, QAS is organized as a list of quantum assembly instructions with one instruction per line. Instructions get executed one at a time in sequential order. The following sections describe each type of instruction in the QAS language.
####Register Declaration
```
qreg my_quantum_reg[2]
creg my_classic_reg[2]
```
The above two lines are instructions that tell the runtime to allocate space for either a quantum or classical register. The format of this instuction is:
```
type name[size]
```
The type is either qreg for "quantum register" or creg for "classical register". The name is any sequence of letters or the '_' character. The following names are examples of valid variable names:
```
my_qreg
q
QR
```
The following names are examples of invalid variable names:
```
myreg1
1
%$#a
```
Finally, the size is an integer indicating the number of bits or qubits in the register (depending on if its a classical or quantum register)
####Print 
The print statement is an instruction used in debugging and it will print either a qubit register or classical register to the terminal
```
qreg q[2]
creg c[2]

print q
print c
```
The above program will output
```
print: (1 + 0i)|0>
print: 00
```
Where the first list represents the state-vector of the quantum register 'q' and the second line represents the classical bits in classical register 'c'.
####Gate application
```
qreg q[4]

h q0
cnot q0 q1
```
Gate application is written as a gate's name followed by the qubits that the gate is to operate on. The qubits are identified by the name of the variable followed by an integer indicating the qubit index in the register. The indexes start at 0 and end at register.size - 1;
```
q0          - quantum register 'q', qubit 0
q1          - quantum register 'q', qubit 1
```
All of the gates that are defined in Qlib - Core can be used. These gates are defined as follows, where the QAS Name is the name is used as the first argument in the gate application statement.
<table>
    <tr>
        <th>Operator</th>
        <th>QAS Name</th>
        <th>Qlib Alias</th>
        <th>Number of Qubits</th>
    </tr>
    <tr>
        <td>Identity</td>
        <td>i</td>
        <td>gates::I</td>
        <td>1</td>
    </tr>
    <tr>
        <td>Hadamard</td>
        <td>h</td>
        <td>gates::H</td>
        <td>1</td>
    </tr>
    <tr>
        <td>Pauli X</td>
        <td>x</td>
        <td>gates::X</td>
        <td>1</td>
    </tr>
    <tr>
        <td>Pauli Y</td>
        <td>y</td>
        <td>gates::Y</td>
        <td>1</td>
    </tr>
    <tr>
        <td>Pauli Z</td>
        <td>z</td>
        <td>gates::Z</td>
        <td>1</td>
    </tr>
    <tr>
        <td>T</td>
        <td>t</td>
        <td>gates::T</td>
        <td>1</td>
    </tr>
    <tr>
        <td>Controlled NOT</td>
        <td>cnot, cx</td>
        <td>gates::CNOT, gates::CX</td>
        <td>2</td>
    </tr>
    <tr>
        <td>Controlled Y</td>
        <td>cy</td>
        <td>gates::CY</td>
        <td>2</td>
    </tr>
    <tr>
        <td>Controlled Z</td>
        <td>cz</td>
        <td>gates::CZ</td>
        <td>2</td>
    </tr>
    <tr>
        <td>Toffoli</td>
        <td>toffoli, ccnot</td>
        <td>gates::TOFFOLI, gates::CCNOT</td>
        <td>3</td>
    </tr>
</table>
####Measure
```
measure q0 -> c0
measure q -> c
```
The above 2 statements are the two different kinds of measurement statement. The first line indicates that we will measure qubit 0 of register 'q' and put that result on the 0th bit of classical register 'c'. The second line is simply syntactic sugar which will call 
```
measure qN -> cN 
```
where  {N &#8712; Z* | N < qreg.size}. In other words, N is an integer between 0 and the size of the quantum register. Measurements will put either 0 or 1 into a classical register, and will collapse the quantum register into a new superposition.
####Include
```
include "./other_file.qas"
```
The include statement will copy the contents of the referenced files into the source of the current file. This can be used to separate scripts into several smaller modules.
