#ifndef _QASM_COMPILER_H
#define _QASM_COMPILER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <algorithm>

#include "./lexer.hpp"
#include "./parser.hpp"
#include "./runtime.hpp"
#include "./scanner.hpp"

using namespace std;
using namespace lexical;
using namespace parser;

//Header back-references
//class program;
//bool try_parse_qasm(program& prog, std::string file_name, lexical::lexer& tokenizer);
//----------------------

/// <Summary>
/// Class containing all instructions to execute
/// </Summary>
class program {    
    public:
        /// <Summary>
        /// List of executable instructions
        /// </Summary>
        std::vector<qasm::exec::executable*> lines;
        /// <Summary>
        /// Map of name to line index of each anchor for jump operations
        /// </Summary>
        std::map<std::string, ulong> anchors;

        program(){}
        ~program(){
            for(std::vector<qasm::exec::executable*>::iterator it = lines.begin(); it != lines.end(); it++){
                delete (*it);
            }
        }

        /// <Summary>
        /// Add instruction
        /// </Summary>
        void push(qasm::exec::executable* ptr){
            lines.push_back(ptr);
        }

        /// <Summary>
        /// Add several instructions
        /// </Summary>
        void push(std::vector<qasm::exec::executable*> lines){
            for(std::vector<qasm::exec::executable*>::iterator it = lines.begin(); it != lines.end(); it++){
                push(*it);
            }
        }
};

/// <Summary>
/// Class for exceptions occuring during parsing of lexical tokens
/// </Summary>
class parseexception : public std::exception {
    private:
        /// <Summary>
        /// Combined message
        /// </Summary>
        std::string what_message;

    public:
        /// <Summary>
        /// Row of cause
        /// </Summary>
        ulong r;
        /// <Summary>
        /// Column of cause
        /// </Summary>
        ulong c;
        /// <Summary>
        /// Filename cause occurred in
        /// </Summary>
        std::string file;
        /// <Summary>
        /// Cause of exception
        /// </Summary>
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

/*
namespace qasm {
namespace rules {

    /// <Summary>
    /// Parse subcircuit_application: Name Name*;
    /// </Summary>
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

    /// <Summary>
    /// Parse gate_application: Name Index*; 
    /// </Summary>
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

    /// <Summary>
    /// Parse debug: Print Name;  
    /// </Summary>
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

    /// <Summary>
    /// Parse measurement: Measure Index Mapping Index;
    /// </Summary>
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
                tokens[1].content[0], 
                tokens[3].content[0]
            ));
            return true;
        }
        else{
            return false;
        }
    }

    /// <Summary>
    /// Parse declaration: Name Name '[' Integer+ ']';  
    /// </Summary>
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

    /// <Summary>
    /// Parse label: Dot Name ;  
    /// </Summary>
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

    /// <Summary>
    /// Parse include: Include String;
    /// </Summary>
    bool try_parse_include(program& prog, lexical::lexer& tokenizer, std::vector<lexical::match>& tokens){
        if(tokens.size() < 2){
            return false;
        }
        if(
            (tokens[0].lexemeptr)->name == "Include" && 
            (tokens[1].lexemeptr)->name == "String"
        ){
            //std::filesystem::path path(tokens[1].content[0]);
            if(!try_parse_qasm(prog, tokens[1].content[0], tokenizer)){
                //throw exception?
            }
            return true;
        }else{
            return false;
        }
    }

    /// <Summary>
    /// Test every rule against a list of matched tokens, If rule is matched an instruction is added to the program and this returns true
    /// </Summary>
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
        return false;
    }

}
}

/// <Summary>
/// Try and parse a file into a program
/// </Summary>
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
*/

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Compiler class definition
//------------------------------------------------------------------------------
/// <Summary>
/// Class wrapping up all compiler components
/// </Summary>
class compiler {
    public:
        lexical::lexer& tokenizer;
        vector<parser::ruleptr>& rules; 
        vector<qasm::exec::executable* (*)(parser::parsetree&)>& converters;
        vector<void(*)(compiler&, qasm::exec::executable*)>& events;
        program& prog;

        compiler(
            lexical::lexer& tokenizer, 
            vector<parser::ruleptr>& rules, 
            vector<qasm::exec::executable* (*)(parser::parsetree&)>& converters,
            vector<void(*)(compiler&, qasm::exec::executable*)>& events,
            program& prog
        ): tokenizer(tokenizer), rules(rules), converters(converters), events(events), prog(prog)
        {}

        bool try_parse_file(std::string filename){
            scanner s(filename);
            std::string line;
            //Iterate over file
            while(s.next(line)){
                try{
                    //Tokenize
                    vector<match> tokens = tokenizer.tokenize(filename, line.begin(), line.end());

                    //Skip empty lines
                    if(tokens.size() < 1)
                        continue;
                    bool ismatch = false;

                    //Iterate over all rules determining if the tokens match
                    size_t index = 0;
                    for(vector<parser::ruleptr>::iterator it = rules.begin(); it != rules.end(); it++){
                        parser::parsetree pt;
                        parser::tokenqueue q(tokens);
                        if((*it)->tryParse(q, &pt)){
                            ismatch = true;
                            //Do post event
                            if(index < converters.size()){
                                qasm::exec::executable* ptr = converters[index](pt);
                                if(index < events.size()){
                                    events[index](*this, ptr);
                                }else{
                                    delete ptr;
                                }
                            }
                            break;
                        }
                        index++;
                    }
                    if(ismatch == false){
                        throw parseexception(s.getLineNumber() - 1, 0, filename, "Line is not a valid statement");
                    }
                }catch(lexical::lexicalexception& ex){
                    throw lexical::lexicalexception(s.getLineNumber() - 1, ex.c, ex.file, ex.msg);
                }
            }
            return true;
        }
};

//------------------------------------------------------------------------------
// Converters
//------------------------------------------------------------------------------
/// <Summary>
/// Convert parse tree to import executable
/// </Summary>
qasm::exec::executable* convert_import (parser::parsetree& ref){
    qasm::exec::import* rule = new qasm::exec::import(ref.getRight().value.content[0]);
    return rule;
}
/// <Summary>
/// Convert parse tree to anchor executable
/// </Summary>
qasm::exec::executable* convert_anchor (parser::parsetree& ref){
    qasm::exec::label* rule = new qasm::exec::label(ref.getRight().value.content[0]);
    return rule;
}
/// <Summary>
/// Convert parse tree to pint executable
/// </Summary>
qasm::exec::executable* convert_print (parser::parsetree& ref){
    qasm::exec::print* rule = new qasm::exec::print(ref.getRight().value.content[0]);
    return rule;
}
/// <Summary>
/// Convert parse tree to gate executable
/// </Summary>
qasm::exec::executable* convert_gate_application (parser::parsetree& ref){
    string name;
    vector<string> params;
    vector<u64> indecies;

    //Far left leaf is name
    //Remainder are indexors
    stack<parser::parsetree*> list;
    list.push(&ref);
    while(!list.empty()){
        parser::parsetree* node = list.top();
        list.pop();
        if(node->isBranch()){
            for(size_t t = 0; t < node->size(); t++){
                list.push(&(node->getChild(t)));
            }
        }else{
            if(node->value.lexemeptr->name == "Reference"){
                name = node->value.content[0];
            }else if(node->value.lexemeptr->name == "Indexor"){
                params.push_back(node->value.content[1]);
                indecies.push_back(std::stol(node->value.content[2]));
            }
        }
    }
    
    std::reverse(params.begin(), params.end());
    std::reverse(indecies.begin(), indecies.end());

    qasm::exec::apply_gate* rule = new qasm::exec::apply_gate(name);
    rule->param_names = params;
    rule->param_indecies = indecies;
    return rule;
}
/// <Summary>
/// Convert parse tree to declaration executable
/// </Summary>
qasm::exec::executable* convert_typedef (parser::parsetree& ref){
    string name;
    string type;
    u64 size;

    name = ref.getLeft().getLeft().getLeft().getRight().value.content[0];
    type = ref.getLeft().getLeft().getLeft().getLeft().value.content[0];
    size = std::stol(ref.getLeft().getRight().value.content[0]);

    qasm::exec::declaration* ptr = new qasm::exec::declaration(type, name, size);
    return ptr;
}
/// <Summary>
/// Convert parse tree to measurement executable
/// </Summary>
qasm::exec::executable* convert_measurement (parser::parsetree& ref){
    //two ways to measure
    if(ref.getRight().getRight().value.lexemeptr->name == "Reference"){
        string first = ref.getRight().getLeft().getLeft().value.content[0];
        string second = ref.getRight().getRight().value.content[0];

        return new qasm::exec::measurement(first, second);
    }else if (ref.getRight().getRight().value.lexemeptr->name == "Indexor"){
        string first = ref.getRight().getLeft().getLeft().value.content[1];
        string fi = ref.getRight().getLeft().getLeft().value.content[2];
        string second = ref.getRight().getRight().value.content[1];
        string si = ref.getRight().getRight().value.content[2];

        return new qasm::exec::measurement(first, std::stol(fi), second, std::stol(si));
    }else{  
        throw parseexception(-1, 0, "Nan", "Measurement invalid despite passing syntax check");
    }
}

/// <Summary>
/// Convert parse tree to pint executable
/// </Summary>
qasm::exec::executable* convert_reset (parser::parsetree& ref){
    if(ref.getRight().value.lexemeptr->name == "Indexor"){
        string type = ref.getLeft().getRight().value.content[0];
        string name = ref.getRight().value.content[1];
        string index = ref.getRight().value.content[2];

        qasm::exec::reset* rule = new qasm::exec::reset(name, type, std::stol(index));
        return rule;
    }
    else{
        string type = ref.getLeft().getRight().value.content[0];
        string name = ref.getRight().value.content[0];

        qasm::exec::reset* rule = new qasm::exec::reset(name, type);
        return rule;
    }
}

//------------------------------------------------------------------------------
// Post conversion hooks
//------------------------------------------------------------------------------
/// <Summary>
/// Discard converted value
/// </Summary>
void post_none (compiler& comp, qasm::exec::executable* runable){
    delete runable;
}
/// <Summary>
/// Add converted value to program to execute
/// </Summary>
void post_add (compiler& comp, qasm::exec::executable* runable){
    comp.prog.push(runable);
}
/// <Summary>
/// Load another file into the program
/// </Summary>
void post_import (compiler& comp, qasm::exec::executable* runable){
    qasm::exec::import* ptr = (qasm::exec::import*)runable;
    string file = ptr->filename;
    comp.try_parse_file(file);
    delete runable;
}
/// <Summary>
/// Add a jump anchor to the program
/// </Summary>
void post_anchor (compiler& comp, qasm::exec::executable* runable){
    qasm::exec::label* ptr = (qasm::exec::label*)runable;
    comp.prog.anchors[ptr->name] = comp.prog.lines.size();
    delete runable;
}

#endif