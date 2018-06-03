#ifndef _QLIB_QUANTUM_QREG_H
#define _QLIB_QUANTUM_QREG_H

namespace qlib {
namespace quantum {
	
class qreg {

    private:
        uint qubits;

    public:
        qreg(uint qubits){
            if(qubits < 1)
                qubits = 1;
            this->qubits = qubits;
        }
};
	
}
}

#endif