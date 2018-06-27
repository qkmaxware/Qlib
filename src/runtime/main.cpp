//-------------------------------------
//Version information: MAJOR.MINOR.PATCH
//-------------------------------------
#define MAJOR 0
#define MINOR 1
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
#include "./qasmparser.hpp"
#include "./runtime.hpp"

using namespace std;
using namespace lexical;
using namespace parser;

int main(int arg_count, char* arg_values[]){
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
		&_anchor, &_goto, &_start, &_mapping, &_measure, &_print, &_end, &_if, &_then, &_for, &_loop, &_include,
		&_lineComment, &_whitespace,
		&_string, &_indexor, &_reference, &_integer, &_leftb, &_rightb
	});

	//-------------------------------------
	//Create rules
	//-------------------------------------
	ruleptr import_ 			= _include << _string;
	ruleptr label_ 				= _anchor << _reference;
	ruleptr print_ 				= _print << _reference;
	ruleptr application_ 		= _reference << *_indexor;
	ruleptr typedecl_			= _reference << _reference << _leftb << _integer << _rightb;
	ruleptr measurement_		= _measure << ((_indexor << _mapping << _indexor) | (_reference << _mapping << _reference));

	//-------------------------------------
	//Read in parameters
	//-------------------------------------
	//First arg is the program name
	if(arg_count < 2){
		cout << NAME << " version:" << MAJOR << "." << MINOR << "." << PATCH << endl;
		return 0;
	}

	//Arg is filename
	program prog;
	qasm::runtime::environment env;
	if(try_parse_qasm(prog, arg_values[1], tokenizer)){
		//qasmenv env;
		//prog.run(env);
		for(std::vector<qasm::exec::executable*>::iterator it = prog.lines.begin(); it != prog.lines.end(); it++){
			(*it)->invoke_rootprogram(env);	
		}
	}

	//Program and executable lines are cleaned up by program destructor
	return 0;
}
