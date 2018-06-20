#ifndef _LEXICAL_H
#define _LEXICAL_H

#include <iostream>
#include <regex>
#include <vector>
#include <exception>
#include <sstream>

// ----------------------------------
// Notes
// ----------------------------------
//Regex syntax defined in c++ as http://www.cplusplus.com/reference/regex/ECMAScript/

namespace lexical {

class lexeme;
class match;

struct match {
    public:
        lexeme* lexemeptr;
        std::match_results<std::string::iterator> content;
        ulong row_start;
        ulong column_start;
        ulong row_end;
        ulong column_end;

        bool matchExists(){
           return !content.ready() || (content.empty() && content.size() == 0);
        }
};

class lexeme {
    public:
        std::string name;
        std::string pattern;
        std::regex rx;
        bool skippable;

        lexeme(std::string name, std::string pattern): skippable(false), name(name), pattern(pattern), rx(pattern) {}

        bool matches(std::string::iterator begin, std::string::iterator end){
            return std::regex_search(begin, end, rx);
        }

        match extract(ulong row, ulong column, std::string::iterator begin, std::string::iterator end){
            match m;
            m.lexemeptr = this;

            std::match_results<std::string::iterator> sm;
            std::regex_search(begin, end, sm, rx);
            
            m.content = sm;
            m.row_start = row;
            m.column_start = column;
            
            std::string::iterator endClone = begin + m.content.length();
            for(std::string::iterator beginClone = begin; beginClone != endClone; beginClone++){
                if(*beginClone == '\n'){
                    row++;
                    column = 0;
                }else{
                    column++;
                }
            }
            
            m.row_end = row;
            m.column_end = column;
            
            return m;
        }

};

class lexicalexception : public std::exception {
    private:
        std::string what_message;

    public:
        ulong r;
        ulong c;
        std::string file;
        std::string msg;

        lexicalexception(ulong row, ulong column, std::string file, std::string message) : r(row), c(column), file(file), msg(message){
            std::stringstream sb; 
            sb << msg << " in " << file << " at [" << r << ", " << c << "]";
            what_message = sb.str();
        }

        const char* what() const throw() {
            return what_message.c_str();
        }
};

class lexer {
    private:
        std::vector<lexeme*> tokens;

    public:
        lexer(std::vector<lexeme*> tokens): tokens(tokens){}

        std::vector<match> tokenize(std::string file, std::string::iterator begin, std::string::iterator end){
            std::vector<match> matches;
            ulong row = 0;
            ulong column = 0;

            //Loop until content done
            while(begin != end){
                bool foundMatch = false;
                match m;
                //Loop over all valid lexemes
                for(std::vector<lexeme*>::iterator it = tokens.begin(); it != tokens.end(); it++){
                    foundMatch = (*it)->matches(begin, end);
                    if(foundMatch){
                        m = (*it)->extract(row, column, begin, end);
                        if(!(*it)->skippable)
                            matches.push_back(m);
                        break;
                    }
                }

                if(foundMatch){
                    //Set begin to end of last match
                    begin += m.content.length();
                    row = m.row_end;
                    column = m.column_end;
                }
                else{
                    //Throw exception
                    std::stringstream sb; 
                    sb << "Unrecognized token '" << *begin << "'";
                    throw lexicalexception(row, column, file, sb.str());
                }
            }

            return matches;
        }
};

}


#endif