include "./my_includes.qasm"

//Define quantum and classical registers
qreg q[2]
creg c[2]

//Define sub-circuits
begin entangle q 
    h q0
    cnot q0 q1
end

//Begin main program
.main //Indenting not required as this is just an anchor to jump statements, but prefered for clean code
    entangle q
    measure q0 -> c0

    if c0 then
        measure q1 -> c1
    end

    print q
    print c