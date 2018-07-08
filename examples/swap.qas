//Allocate
qreg q[2]
creg c[2]

//Swap 
x q1
cx q0 q1
h q0
h q1
cx q0 q1
h q0
h q1
cx q0 q1

//Measure
print c
measure q -> c
print c