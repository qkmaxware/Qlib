##Table of contents
<!-- TOC -->

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
    - [Quantum System Interface](#quantum-system-interface)

<!-- /TOC -->

###Types of systems
In Qlib, we have 3 types of implemented quantum-systems which can be used to perform quantum simulations.
```
qlib::quantum::qubit        - Single simulated qubit
qlib::quantum::qreg         - Register of multiple simulated qubits
qlib::quantum::ensemble     - Collection of multiple identical quantum systems
```

###Qubit
####Description
The `qubit` class represents a single quantum bit. This representation in [bra-ket](https://en.wikipedia.org/wiki/Bra%E2%80%93ket_notation) notation is equivalent to:
```
|qubit> = a * |0> + b * |1>
```
`a` and `b` are both complex numbers where `a*a` is the probability that measurement of the qubit will return `0` and `b*b` is the probability that measurement of the qubit will return 1. The sum of `aa` and `bb` are subject to a normalization constraint such that `aa + bb = 1` or in words: "The probability of getting any one of the possibilities is 100%".

This can also be witten as a column vector with values `[a; b]` which is more conventient for describing the operations that can be performed on a qubit. 

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
Measurement on a qubit means forcing a qubit to decide if it is 0 or 1 rather than having some probability of 0 or 1. This involes some kind of "state-collapse". Since there is some probability of measuring 0 and some probability of measuring 1 measuring many identical qubits can actaully give different results. We can describe this as one of two relations.
```
a * |0> + b * |1>     ->      1 * |0> + 0 * |1>
```
```
a * |0> + b * |1>     ->      0 * |0> + 1 * |1>
```
The probability amplitudes change from some values `a` and `b` to either `a` **or** `b`. Before measurement, the qubit has some probability of being either 0 or 1, but after measurement the system must be either 0 or 1 and has no change of being the other. 

The state of a qubit after measurment is the result of applying the projector to the state vector. If we measure the value 0, then the state vector has collaped such that it can be described by:
```
|0><0| * |x>  = 1 * |0>
```
For convenience, we can say that `|0> = [1; 0]` and `|1> = [0; 1]` such that `|0><0| = [1, 0; 0, 0]`.

In Qlib, the measurement of a single qubit is implemented with the following psudo-code.
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
The `qreg` class represents a quantum register composed of 'N' qubits where 'N' >= 1; A single qubit can be either 0 or 1, but a register of qubits can represent an space of 2^N. If we have 2 qubits, the state vector is represented as the [Kronecker Tensor Product](https://en.wikipedia.org/wiki/Kronecker_product) of each qubit's state vector.

|x> &#8855; |y> = a|00> + b|01> + c|10> + d|11>
where:
|x> = x1|0> + x2|1>
|y> = y1|0> + y2|1>

More generally, for an 'N' qubit register, the state vector of the system is representable by the tensor product below.
|Ao> &#8855; ... &#8855; |An> = a|0...0n> + ... + b|1...1n>

In qlib, we can compute the number of elements in the state vector by a left bit-shift.
```cpp
long states = 1 << Number_of_Qubits;
```
####Qubit Operators
Similarily to single qubits, operators are MxM hermitian matrices where M = 2^qubits. Most operators for quantum computing only operate on a single qubit, with a couple that take in more than one qubit. The reason for this is that we can constuct a set of universal quantum gates from as little as 3 gates: {H, T, CNOT} ([Phillip Kaye, Raymond Laflamme, Michele Mosca; 2007]()). By universal we mean that any operator on the space can be represented by some combination of gates from the universal set up to some level arbitrary level of precision.

Application of operators in Qlib are limited to one-qubit operators and controlled qubit operators to simplify the code. Since we can get a universal set of gates from just one qubit opertors and any 2-qubit operator, this is sufficient. Applying a single qubit operator to an 'N' qubit systems involves constructing an MxM matrix using only 2x2 identity matrices and the  single-qubit operator one wishes to apply. For example, if one wanted to apply the H opertor to the 'K'th qubit in an 'N' qubit system (K < N), then the new operator in matrix form would look as it does in below.

I<sub>0</sub> &#8855; I<sub>1</sub> &#8855; ... &#8855; H<sub>K</sub> &#8855; ... &#8855; I<sub>N</sub>

However, this would not be an time or space efficient computation to perform on a computer as it would involve the allocation of an MxM matrix rather than just using a single-qubit 2x2 matrix. Intel described an alternative method in their paper discussing how the build their qHipster quantum simulator ([Mikhail Smelyanskiy, Nicolas P. D. Sawaya, Alán Aspuru-Guzik; 2016](https://arxiv.org/abs/1601.07195)). In this implementation we assume that states are ordered sequentially and we apply the 2x2 matrix to different pairs of values separated by a stride of 2^k where k is the index of the qubit being operated on (left-to-right). The psudo-code for this is shown below where the state vector is labeled "α" and the 2x2 operator is labeled as "q".
```
for g ← 0; g < 2; g += 2^(k+1) do
    for i ← g; i < g + 2^k; i++ do
        α(i)' ← q11 · α(i) + q12 · α(i+2^K)
        α(i+2^K)' ← q21 · α(i) + q22 · α(i+2^K)
    end for
end for
```
From here, we can extend this idea of applying one-qubit operators to pairs of states to controlled operators. In essence, we do the same procedure as above but we skip applying the opertro to pairs of states where the second-qubit or "control" qubit is unset. If one wanted to apply the controlled-X (CNOT) gate to a 2-qubit register with the first qubit is the target and the second qubit is the control, then we would apply the X operator to the state pair `|10> and |11>` since the second qubit is set (1) but would skip `|00> and |01>` because the second qubit is unset (0). This extention continues for 3 qubit gates with 2 control qubits.

####Measurement
Measurement of an 'N' qubit system can be described in two ways. The first is a complete measurement where the whole system collapses to a specific state. This is similar to the method for single-qubit measurements where the system collapses into either 0 or 1, but instead of 0 or 1 the system is some binary string of 0's and 1's. For instance, with a 2-qubit system a complete measurment could collapse the whole system to state '01' where the first qubit collapsed to state 1 and the second qubit collapsed to state 0. The second way to describe a measurement, is a partial measurement of a specific qubit in the system. In this description, the measured qubit collapses to either state 0 or 1 but the other qubits remain in a superposition. This description is more general because we could apply this to each qubit sequentially and the end result is the same as applying a complete measurement. The basic algorithm for a partial measurement is shown in psudo-code below.
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
The collapse function here takes in the qubit index and the measured value. If 0 is measured, then all state amplitudes in the superposition where the 'k'th qubit is 1 are set to 0, and the state-vector is then normalized. Alternatively if 1 was measured then state amplitudes where the 'k'th qubit was 0 are set to 0 and then the state-vector is normalized.

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