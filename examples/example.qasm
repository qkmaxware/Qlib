//include "./my_includes.qasm"
 
//Define quantum and classical registers
qreg q[2]
creg c[2]

//Begin main program
.main   
    h q0
    cnot q0 q1
    measure q0 -> c0
    measure q1 -> c1
    print q
    print c