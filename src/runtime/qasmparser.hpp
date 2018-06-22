#ifndef _QASM_PARSER_H
#define _QASM_PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "./lexer.hpp"
#include "./runtime.hpp"

using namespace std;

//Header back-references
class program;
bool try_parse_qasm(program& prog, std::string file_name, lexical::lexer& tokenizer);
//----------------------

class program {    
    public:
        std::vector<qasm::exec::executable*> lines;
        std::map<std::string, ulong> anchors;

        program(){}
        ~program(){
            for(std::vector<qasm::exec::executable*>::iterator it = lines.begin(); it != lines.end(); it++){
                delete (*it);
            }
        }

        void push(qasm::exec::executable* ptr){
            lines.push_back(ptr);
        }

        void push(std::vector<qasm::exec::executable*> lines){
            for(std::vector<qasm::exec::executable*>::iterator it = lines.begin(); it != lines.end(); it++){
                push(*it);
            }
        }
};

namespace qasm {
namespace rules {

    //subcircuit_application: Name Name*;
    bool try_parse_subcircuit_application(program& prog, std::vector<lexical::match>& tokens){
        if(tokens.size() < 1){
            return false;
        }
        if((tokens[0].lexemeptr)->name == "Reference"){
            qasm::exec::apply_subcircuit apply(tokens[0].content[0]);

            for(size_t i = 1; i < tokens.size(); i++){
                if((tokens[i].lexemeptr)->name != "Reference"){
                    return false;
                }
                apply.params.push_back(tokens[i].content[0]);
            }

            prog.push(new qasm::exec::apply_subcircuit(apply));
            return true;
        }else {
            return false;
        }
    }

    //gate_application: Name Index*; 
    bool try_parse_gate_application(program& prog, std::vector<lexical::match>& tokens){
        if(tokens.size() < 2){
            return false;
        }
        if((tokens[0].lexemeptr)->name == "Reference"){
            qasm::exec::apply_gate apply(tokens[0].content[0]);

            for(size_t i = 1; i < tokens.size(); i++){
                if((tokens[i].lexemeptr)->name != "Indexor"){
                    return false;
                }
                apply.param_names.push_back(tokens[i].content[1]);
                apply.param_indecies.push_back(std::stol(tokens[i].content[2]));
            }
            prog.push(new qasm::exec::apply_gate(apply));
            return true;
        }else {
            return false;
        }
    }

    //debug: Print Name; 
    bool try_parse_debug(program& prog, std::vector<lexical::match>& tokens){
        if(tokens.size() < 2){
            return false;
        }
        if(
            (tokens[0].lexemeptr)->name == "Print" && 
            (tokens[1].lexemeptr)->name == "Reference" 
        ){
            prog.push(new qasm::exec::print(tokens[1].content[0]));
            return true;
        }else{
            return false;
        }
    }

    //measurement: Measure Index Mapping Index;
    bool try_parse_measurement(program& prog, std::vector<lexical::match>& tokens){
        if(tokens.size() < 4){
            return false;
        }
        if(
            (tokens[0].lexemeptr)->name == "Measure" && 
            (tokens[1].lexemeptr)->name == "Indexor" &&
            (tokens[2].lexemeptr)->name == "Map" &&
            (tokens[3].lexemeptr)->name == "Indexor"
        ){
            prog.push(new qasm::exec::measurement(
                tokens[1].content[1], 
                std::stol(tokens[1].content[2]), 
                tokens[3].content[1], 
                std::stol(tokens[3].content[2])
            ));
            return true;
        }
        else if(
            (tokens[0].lexemeptr)->name == "Measure" && 
            (tokens[1].lexemeptr)->name == "Reference" &&
            (tokens[2].lexemeptr)->name == "Map" &&
            (tokens[3].lexemeptr)->name == "Reference"
        ){
            prog.push(new qasm::exec::measurement(
                tokens[1].content[1], 
                tokens[3].content[1]
            ));
            return true;
        }
        else{
            return false;
        }
    }

    //declaration: Name Name '[' Integer+ ']';  
    bool try_parse_declaration(program& prog, std::vector<lexical::match>& tokens){
        if(tokens.size() < 5){
            return false;
        }
        if(
            (tokens[0].lexemeptr)->name == "Reference" && 
            (tokens[1].lexemeptr)->name == "Reference" &&
            (tokens[2].lexemeptr)->name == "Left [" &&
            (tokens[3].lexemeptr)->name == "Integer" &&
            (tokens[4].lexemeptr)->name == "Right ]" 
        ){
            prog.push(new qasm::exec::declaration(
                tokens[0].content[0], 
                tokens[1].content[0], 
                std::stol(tokens[3].content[0])
            ));
            return true;
        }else{
            return false;
        }
    }

    //label: Dot Name ;
    bool try_parse_label(program& prog, std::vector<lexical::match>& tokens){
        if(tokens.size() < 2){
            return false;
        }
        if(
            (tokens[0].lexemeptr)->name == "Anchor" && 
            (tokens[1].lexemeptr)->name == "Reference"
        ){
            prog.anchors[tokens[1].content[0]] = prog.lines.size();
            return true;
        }else{
            return false;
        }
    }

    //include: Include String;
    bool try_parse_include(program& prog, lexical::lexer& tokenizer, std::vector<lexical::match>& tokens){
        if(tokens.size() < 2){
            return false;
        }
        if(
            (tokens[0].lexemeptr)->name == "Include" && 
            (tokens[1].lexemeptr)->name == "String"
        ){
            if(!try_parse_qasm(prog, tokens[1].content[0], tokenizer)){
                //throw exception?
            }
            return true;
        }else{
            return false;
        }
    }

    //
    //Catch all, try every sub-rule
    //
    bool try_parse_rule(program& prog, lexical::lexer& tokenizer, std::vector<lexical::match>& tokens){
        if(try_parse_include(prog, tokenizer, tokens)){
            return true;
        }
        if(try_parse_label(prog, tokens)){
            return true;
        }
        if(try_parse_declaration(prog, tokens)){
            return true;
        }
        if(try_parse_measurement(prog, tokens)){
            return true;
        }
        if(try_parse_debug(prog, tokens)){
            return true;
        }
        if(try_parse_gate_application(prog, tokens)){
            return true;
        }
        if(try_parse_subcircuit_application(prog, tokens)){
            return true;
        }
    }

}
}

class parseexception : public std::exception {
    private:
        std::string what_message;

    public:
        ulong r;
        ulong c;
        std::string file;
        std::string msg;

        parseexception(ulong row, ulong column, std::string file, std::string message) : r(row), c(column), file(file), msg(message){
            std::stringstream sb; 
            sb << msg << " in " << file << " at [" << r << ", " << c << "]";
            what_message = sb.str();
        }

        const char* what() const throw() {
            return what_message.c_str();
        }
};

bool try_parse_qasm(program& prog, std::string file_name, lexical::lexer& tokenizer){
    std::ifstream file(file_name);
	std::string line;
    ulong file_line = 1;

	while(std::getline(file, line)){
        try{
            std::vector<lexical::match> tokens = tokenizer.tokenize(file_name, line.begin(), line.end()); 
            if(tokens.size() == 0){
                file_line++;
                continue;
            }
            cout << "line " << file_line << endl;
            if(qasm::rules::try_parse_rule(prog, tokenizer, tokens)){

            }else{
                throw parseexception(file_line, 0, file_name, "Line doesn't match any valid statement types");
            }
            file_line++;
        }catch(lexical::lexicalexception& ex){
            throw lexical::lexicalexception(file_line, ex.c, ex.file, ex.msg);
        }
	}

    return true;  
}

#endif