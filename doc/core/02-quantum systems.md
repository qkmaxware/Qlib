#Qlib - Core: 
##1.2 Simulated Quantum Systems

##Table of contents
<!-- TOC -->

- [Qlib - Core:](#qlib---core)
    - [1.2 Simulated Quantum Systems](#12-simulated-quantum-systems)
    - [Table of contents](#table-of-contents)
        - [Types of systems](#types-of-systems)
        - [Qubit](#qubit)
            - [Description](#description)
            - [Qubit Operators](#qubit-operators)
            - [Measurement](#measurement)
        - [Qreg](#qreg)
            - [Description](#description-1)
            - [Qubit Operators](#qubit-operators-1)
            - [Measurement](#measurement-1)
        - [Ensemble](#ensemble)
            - [Description](#description-2)
            - [Qubit Operators](#qubit-operators-2)
            - [Measurement](#measurement-2)
        - [Quantum System Interface](#quantum-system-interface)

<!-- /TOC -->

###Types of systems
In Qlib, we have 3 types of simulated quantum-systems implemented which can be used to perform quantum simulations. These systems listed below include: a qubit which represents a single isolated qubit, a quantum register (qreg) representing a collection of multiple qubits, and an ensemble which represents a collection of independent quantum systems. Each of these systems is described in their own section in more detail.
```
qlib::quantum::qubit        - Single simulated qubit
qlib::quantum::qreg         - Register of multiple simulated qubits
qlib::quantum::ensemble     - Collection of multiple identical quantum systems
```

###Qubit
####Description
The `qubit` class represents a single quantum bit. As is the case in many quantum mechanical representations, the qubit can be written in [bra-ket](https://en.wikipedia.org/wiki/Bra%E2%80%93ket_notation) notation seen below in equation 1.1.0.

*Equation 1.1.0: A single qubit represented in bra-key format.*
```
|qubit> = a * |0> + b * |1>
```
In this notation, |0> and |1> represent column vectors in 2d space where `a` and `b` are complex scalar quantities. When the scalar quantities are squared, `a*a` is the probability that measurement of the qubit will return the value `0` and `b*b` is the probability that measurement of the qubit will return the value 1. The sum of `aa` and `bb` are subject to a normalization constraint such that `aa + bb = 1` or in words: "The probability of getting any one of the possibilities is 100%".

Since the bra-ket notation is similar to row and column vectors, we can also write the qubit in column vector format: `[a; b]`. This format is more convenient for showing how operations on the qubit affect its state.

####Qubit Operators
All quantum operators are square-[hermitian](https://en.wikipedia.org/wiki/Hermitian_matrix) operators on some N-dimensional [Hilbert space](https://en.wikipedia.org/wiki/Hilbert_space). In the case of a single qubit, this space is 2d. 

Since the Hilbert space for a single qubit is 2d, the operators are representable by any hermitian 2x2 matrices. Examples of such operators include the Pauli-Matrices and the Hadamard Transform. Operations on a single qubit can be represented in matrix form shown below. 
```
         U * |a> = |a'>

[U11, U12] * [a] = [U11 * a + U12 * b]
[U21, U22]   [b]   [U21 * a + U22 * b]
```
This is exactly how operations on a single qubit are represented in Qlib where matrix multiplication is applied to the state-vector `[a; b]`. 

####Measurement
Measurement on a qubit means forcing a qubit to decide if it is 0 or 1 rather than having some probability of being either 0 or 1. This involves some kind of "state-collapse" which takes the qubit from a superposition down to a definite value. We can describe this as one of two simple relations shown in Equation 1.1.1.

*Equation 1.1.1: State collapse of a single qubit from a superposition to either 0 or 1 respectively*
```
a * |0> + b * |1>     ->      1 * |0> + 0 * |1>
```
```
a * |0> + b * |1>     ->      0 * |0> + 1 * |1>
```

The probability amplitudes change from some values `a` and `b` to either `a` **or** `b`. Before measurement, the qubit has some probability of being either 0 or 1, but after measurement the system must be either 0 or 1 and has no chance of being the other. Since there is some probability of measuring 0 and some probability of measuring 1, measuring many identical qubits can actually give different results.  

The state of a qubit after measurment is the result of applying another type of operator called the projector to the state vector. In terms of linear algebra, a projector takes one vector and "projects" it onto another. This means that one can obtain the component of a vector in the direction of another. Equation 1.1.2 shows the application of such a projector to a qubit's state.

*Equation 1.1.2: State of a qubit represented as 'x' after 0 has been measured. New state is the result of applying the projector for 0 to the old superposition.*
```
|0><0| * |x>  = 1 * |0>
```
For convenience, we can assign the 0 and 1 vectors to vectors in 2d space such that `|0> = [1; 0]` and `|1> = [0; 1]`. This means that the projector for 0 then becomes: `|0><0| = [1, 0; 0, 0]`.

*Equation 1.1.3: In Qlib, the measurement of a single qubit is implemented with the following pseudo-code.*
```
pa ← qubit.a * qubit.a
pb ← 1 - pa

rng ← random(0.0, 1.0)
if (rng < pa)
    collapse(0)
    return 0
else
    collapse(1)
    return 1
```
###Qreg
####Description
The `qreg` class represents a quantum register composed of 'N' qubits where 'N' >= 1; A single qubit can be either 0 or 1, but a register of qubits can represent a much larger space and more possible measured values. If we have more than one qubit, the state vector is represented as the [Kronecker Tensor Product](https://en.wikipedia.org/wiki/Kronecker_product) of each qubit's state vector. In the case of 2 qubits, the space is the tensor product of the spaces for both qubits and can represent 4 values in superposition. Equation 1.1.4 shows how this works for 2 qubits.

*Equation 1.1.4: Tensor product of the state spaces for 2 qubits*
|x> ⊗ |y> = a|00> + b|01> + c|10> + d|11>
where:
|x> = x1|0> + x2|1>
|y> = y1|0> + y2|1>


More generally, for an 'N' qubit register, the state vector of the system is representable by the tensor product in equation 1.1.5

*Equation 1.1.5: State space of an 'N' qubit quantum register*
|Ao> &#8855;  ... &#8855;  |An> = a|0...0n> + ... + b|1...1n>


In qlib, we can compute the number of elements in the state vector by a left bit-shift.
```cpp
long states = 1 << Number_of_Qubits;
```
####Qubit Operators
Similarly to single qubits, operators are MxM hermitian matrices where M = 2^qubits. Most operators for quantum computing only operate on a single qubit, with a couple that take in more than one qubit. The reason for this is that we can construct a set of universal quantum gates from as little as 3 gates: {H, T, CNOT} (Phillip Kaye, Raymond Laflamme, Michele Mosca). By universal, we mean that any operation on the space can be represented by some combination of operators from the universal set up to some level level of precision.

To simplify the operators that needed to be coded in Qlib, the included operators are limited to one-qubit operators and controlled qubit operators. Since we can get a universal set of gates from a spanning set of one qubit operators and any 2-qubit operator, this is sufficient for performing any quantum computation. Applying a single qubit operator to an 'N' qubit system involves constructing an MxM matrix using only 2x2 identity matrices and the  single-qubit operator one wishes to apply. For example, if one wanted to apply the H or Hadamard operator to the 'K'th qubit in an 'N' qubit system (K < N), then the new operator in matrix form would look as it does in below in equation 1.1.6.

*Equation 1.1.6: Construction of an operator that applies 'H' to the Kth qubit of an N qubit register*
I<sub>0</sub> &#8855; I<sub>1</sub> &#8855; ... &#8855; H<sub>K</sub> &#8855; ... &#8855; I<sub>N</sub>

However, the construction of such a matrix would not be a time or space efficient computation on a classical machine. This is because the computer would have the allocation of an MxM matrix and perform the calculations for all intermediate tensor products rather than just allocating the memory for the single-qubit 2x2 matrix. In their paper discussing how to build their qHipster quantum simulator, Mikhail et el described an approach where one-qubit operators could be applied to arbitrarily large state vectors by applying the operator to different pairs of states separated by a stride of 2^k where k is the index of the qubit being operated on (left-to-right) (Mikhail Smelyanskiy, Nicolas P. D. Sawaya, Alán Aspuru-Guzik). In this implementation we assume that states are ordered sequentially. The pseudo-code for this is shown below in equation 1.1.7.

*Equation 1.1.7: Algorithm for applying a one-qubit operator to an 'N' qubit register with 2^N states. The state vector is labeled "α" and the 2x2 operator is labeled as "q"*
```
for g ← 0; g < 2; g += 2^(k+1) do
    for i ← g; i < g + 2^k; i++ do
        α(i)' ← q11 · α(i) + q12 · α(i+2^K)
        α(i+2^K)' ← q21 · α(i) + q22 · α(i+2^K)
    end for
end for
```
From here, we can extend this idea of applying one-qubit operators to pairs of states to controlled operators. In essence, we do the same procedure as above, however we skip applying the operator to pairs of states where the second-qubit or "control" qubit is unset (0). If one wanted to apply the controlled-X (CNOT) gate to a 2-qubit register then we would apply the X operator to the states where the control qubit is set such as `|10> and |11>` where the second qubit is set (1) but would would skip `|00> and |01>` because the second qubit is unset (0). This extension continues for 3 qubit gates with 2 control qubits.

####Measurement
Measurement of an 'N' qubit system can be described in two ways. The first is a complete measurement where the whole system collapses to a specific state. This is similar to the method for single-qubit measurements where the system collapses into either 0 or 1. For complete measurements of muli-qubit systems, the system collapses into some binary string of 0's and 1's rather than just 0 or 1 for the single qubit system. For instance, with a 2-qubit system, this kind of measurement could collapse the whole system to state '01' where the first qubit collapsed to state 1 and the second qubit collapsed to state 0. The second way to describe a measurement, is a partial measurement of a specific qubit in the system. In this description, the measured qubit collapses to either state 0 or 1 but the other qubits remain in a superposition. This would allow us to determine the value of a particular qubit without losing all the information stored in the superposition of the other bits. This is the implementation of measurement that is used in Qlib. Equation 1.1.8 shows the pseudo-code for this operation.

*Equation 1.1.8: Algorithm for measuring the kth qubit of an n-qubit register in Qlib.*
```
p0 ← sum amplitude^2 where qubit 'k' in state label is 0
p1 ← sum amplitude^2 where qubit 'k' in state label is 1

rng ← random(0.0, 1.0)

if(rng < p0)
    collapse(k, 0)
    return 0;
else
    collapse(k, 1)
    return 1
```
The collapse function here takes in an index of the qubit we wish to measure and outputs the measured value whilst collapsing the state of that specific qubit while leaving the others untouched. If 0 is measured, then all state amplitudes of the system where the 'k'th qubit is set to 1 are set to 0, and the state-vector is then normalized. Alternatively, if a value of 1 was measured then amplitudes where the 'k'th qubit was 0 are set to 0 and then the state-vector is normalized.

###Ensemble
####Description
An ensemble represents a collection of identical systems. Since quantum computations are probabilistic in nature, and the desired answer may not have 100% likelihood of occurring, an ensemble allows us to get the correct answer with a higher likelihood. The idea is that is you have several identical systems and you have a 75% chance of getting answer 'A' and a 25% chance of getting answer 'B', then the majority of these systems will yield answer 'A'. Since the majority of systems yielded 'A', then that is the value that is returned. Ensembles in Qlib can technically store any quantum systems in any order (including other ensembles), but are most effective when all the stored quantum systems are of the same type (usually a quantum register). An example of creating an ensemble can be seen below in example 1.1.0

*Example 1.1.0: Code in c++ for creating an ensemble of 10 qubits.*
```cpp
vector<qsystem*> systems;
for(int i = 0; i < 10; i++){
    systems.push_back(new qubit());
}
ensemble en(systems);
``` 

####Qubit Operators
Since an ensemble is just a collection of systems, quantum operators behaviours are described by the stored systems. The ensemble simply facilitates applying the given operator to all the systems at the same time. 

####Measurement
Similarly to operator application, the ensemble itself does not actually measure anything. Instead, the ensemble tells each stored system to measure itself, and then returns the most common result from all the systems. 

###Quantum System Interface
Each of the systems above shared the same kinds of operations (Apply Operator, Measure). Since these operations are shared for each type of quantum system, Qlib implements each of these systems as super-classes of a base "quantum system" interface. This interface definition can be seen below.
```cpp
//----------------------------------------------------------
// Interface definition
//----------------------------------------------------------
class qsystem: public xobject {
    private:
    public:
        //Modify the state of the system by applying a quantum gate
        virtual void apply(qlib::quantum::gates::igate& gate,std::vector<ulong> inputBits) = 0;

        //Measure a specific qubit, collapse the state of the system and return the measured value
        virtual i32 measure(i32 qubit) = 0;

        //Human readable output
        virtual std::string toString(){
            return xobject::toString();
        };
};
```