//Allocate
qreg q[2]
creg c[2]
print q

//Swap 
x q1

//Measure
print q
measure q -> c 
print c