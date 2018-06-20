grammar qasm;

//Parsing rules
program: statement*;
statement: (include | label | declaration | measurement | gate_application | subcircuit_application | subcircuit | debug | classic_logic) Separator;

//include "./package.qasm"
include: Include String;
//.section
label: Dot Name ;
//qreg q[4]
declaration: Name Name '[' Integer+ ']';    
//measure q1 -> b1    
measurement: Measure Index Mapping Index;   
//cnot q0    
gate_application: Name Index*;  
//entangle q
subcircuit_application: Name Name*;
//begin entangle q
//  h q0
//  cnot q0 q1
//end                     
subcircuit: Start Name Name* Separator statement* End;
//print q  
debug: Print Name;                              
//If, loop or jump
classic_logic: condition | loop | jump;
//if c0 then
//  h q0
//end
condition: If Index Then Separator statement* End;
//loop 4 for
//   h q0
//end
loop: Loop Integer For Separator statement* End;
//goto .label if c[3]
jump: Goto label If Index;

//Lexer rules
Dot: '.';
Goto: 'goto';
Start: 'begin';
Mapping: '->';
Measure: 'measure';
Print: 'print';
End: 'end';
If: 'if';
Then: 'then';
For: 'for';
Loop: 'loop';
Include: 'include';

MultiLineComment: '/*'  ~('\r' | '\n')* '*/' -> skip;
SingleLineComment: '//' ~('\r' | '\n')* -> skip;
String: '"' (~[\r\n"] | '\\"')* '"';

Index: Name Integer; 
Integer: [0-9]+;
Name: [a-z_][a-zA-Z_]*;
Separator: '\r'?'\n';