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

#include "./lexer.hpp"
#include "./parser.hpp"
#include "./qasmparser.hpp"

#include "./../core/qlib.h"

using namespace std;
using namespace lexical;

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
	lexeme _string("String", "^\\\"[^\\r\\n]*\\\""); 
	lexeme _indexor("Indexor", "^([a-z_][a-zA-Z_]*)([0-9]+)");
	lexeme _reference("Reference", "^[a-z_][a-zA-Z_]*");
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
	if(try_parse_qasm(prog, arg_values[1], tokenizer)){
		//qasmenv env;
		//prog.run(env);
		//for(std::vector<qasm::exec::executable*>::iterator it = lines.begin(); it != lines.end(); it++){
			//(*it)->invoke_rootprogram(prog, env);
		//}
	}
	
	//Program and executable lines are cleaned up by program destructor
	return 0;
}
