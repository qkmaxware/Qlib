//-------------------------------------
//Version information: MAJOR.MINOR.PATCH
//-------------------------------------
#define MAJOR 1
#define MINOR 0
#define PATCH 0

#define NAME "Qlib Runtime"

//-------------------------------------
//File start
//-------------------------------------
#include <iostream>
#include <fstream>

#include "./../core/qlib.h"

#include "./lexer.hpp"
#include "./parser.hpp"
#include "./compiler.hpp"
#include "./runtime.hpp"
#include "./scanner.hpp"
#include "./qcircuit_exporter.hpp" 
#include "./argparser.hpp"

using namespace std;
using namespace lexical;
using namespace parser;

int main(int arg_count, char* arg_values[]){
	//-------------------------------------
	//Read in command line parameters
	//-------------------------------------
	argz arg(arg_count, arg_values);

	//-------------------------------------
	//Configure lexical tokens
	//-------------------------------------
	//Keywords
	lexeme _anchor("Anchor", "^\\.");
	lexeme _goto("Goto", "^goto\\b");
	lexeme _start("Start", "^begin\\b");
	lexeme _mapping("Map", "^->");
	lexeme _measure("Measure", "^measure\\b");
	lexeme _print("Print", "^print\\b");
	lexeme _end("End", "^end\\b");
	lexeme _if("If", "^if\\b");
	lexeme _then("Then", "^then\\b");
	lexeme _for("For", "^for\\b");
	lexeme _loop("Loop", "^loop\\b");
	lexeme _include("Include", "^include\\b");
	lexeme _reset("Zero", "^zero\\b");
	
	//Skippables
	lexeme _lineComment("Line Comment", "^//[^\\r\\n]*");
		_lineComment.skippable = true;
	lexeme _whitespace("Whitespace", "^[\\t\\f\\v\\r\\n ]+");
		_whitespace.skippable = true;

	//Literals
	lexeme _string("String", "^\\\"[^\\r\\n]*?\\\""); 
	lexeme _indexor("Indexor", "^([a-zA-Z_]+)([0-9]+)");
	lexeme _reference("Reference", "^[a-zA-Z_]+");
	lexeme _integer("Integer", "^[0-9]+");
	lexeme _leftb("Left [", "^\\[");
	lexeme _rightb("Right ]", "^\\]");
	
	//-------------------------------------
	//Create tokenizer
	//-------------------------------------
	lexer tokenizer({
		&_anchor, &_goto, &_start, &_mapping, &_measure, &_print, &_end, &_if, &_then, &_for, &_loop, &_include, &_reset,
		&_lineComment, &_whitespace,
		&_string, &_indexor, &_reference, &_integer, &_leftb, &_rightb
	});

	//-------------------------------------
	//Create rules
	//-------------------------------------
	//include "filename"
	ruleptr import_ 			= _include << _string;
	//zero qreg q
	ruleptr reset_				= _reset << _reference << (_reference | _indexor);
	//.label
	ruleptr label_ 				= _anchor << _reference;
	//print q
	ruleptr print_ 				= _print << _reference;
	//cnot q0 q1
	ruleptr application_ 		= _reference << *_indexor;
	//myfunc q
	/**///ruleptr fncall_				= _reference << *_reference;
	//qreg myreg[3]
	ruleptr typedecl_			= (((_reference << _reference) << _leftb) << _integer) << _rightb;
	//measure q -> c
	ruleptr measurement_		= _measure << ((_indexor << _mapping << _indexor) | (_reference << _mapping << _reference));
	//start myfunc
	/**///ruleptr fndecl_				= _start << _reference;
	//end
	/**///ruleptr fnend_				= _end;
	//gate mygate 0, 1, 1, 0
	/**///ruleptr gate_			= _gate << _reference << _complex << _comma << _complex << _comma << _complex << _comma << _complex; 

	//-------------------------------------
	//Create parse lists
	//-------------------------------------
	vector<ruleptr> rules 
		= {import_, label_, print_, typedecl_, application_, measurement_, reset_};

	vector<qasm::exec::executable* (*)(parser::parsetree&)> converters
		= {convert_import, convert_anchor, convert_print, convert_typedef, convert_gate_application, convert_measurement, convert_reset};

	vector<void(*)(compiler&, qasm::exec::executable*)> post_events
		= {post_import, post_anchor, post_add, post_add, post_add, post_add, post_add};

	//-------------------------------------
	//Begin compilation
	//-------------------------------------
	program prog;
	qasm::runtime::environment env;
	compiler cmp(tokenizer, rules, converters, post_events, prog);

	//New
	//-------------------------------------
	//Run in file mode
	//-------------------------------------
	if(arg.arguments.size() > 0){
		//Arg is filename
		std::string filename = arg.arguments[0];
		bool b = cmp.try_parse_file(filename);

		if(!arg.hasFlag("-noeval") && b && prog.lines.size() > 0){
			for(std::vector<qasm::exec::executable*>::iterator it = prog.lines.begin(); it != prog.lines.end(); it++){
				(*it)->invoke_rootprogram(env);	
			}
		}
	}
	//-------------------------------------
	//Run in interactive mode
	//-------------------------------------
	else if(arg.hasFlag("-i")){
		cout << NAME << " interactive mode. Type `quit` to exit" << endl;
		std::string line;
		while(true){
			std::getline(cin, line);
			if(line == "quit")
				break;
			cmp.try_parse_line("interactive terminal", prog.lines.size(), line);
			if(!arg.hasFlag("-noeval")){
				prog.lines[prog.lines.size() - 1]->invoke_rootprogram(env);
			}
		}
	}
	//-------------------------------------
	//Run in help mode
	//-------------------------------------
	else{
		cout << NAME << " version:" << MAJOR << "." << MINOR << "." << PATCH << endl;
		cout << "Usage: executable <filename> [options]" << endl << endl;
		cout << "-i      :: " << "Enter interactive mode if no filename is supplied." << endl;
		cout << "-svg    :: " << "Export a circuit diagram in Scalable Vector Graphics format." << endl;
		cout << "-noeval :: " << "Load but do not evaluate quantum scripts. Usually used with -svg." << endl;
		return 0;
	}
	//Old		
	/*if(try_parse_qasm(prog, filename, tokenizer)){
		for(std::vector<qasm::exec::executable*>::iterator it = prog.lines.begin(); it != prog.lines.end(); it++){
			(*it)->invoke_rootprogram(env);	
		}
	}*/

	if(arg.hasFlag("-svg")){
		exportQuantumCircuit("export.svg", prog);
	}

	//Program and executable lines are cleaned up by program destructor
	return 0;
}
