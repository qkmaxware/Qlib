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

/// <Summary>
/// Class representing a regex match in a lexed document including row and column indexes
/// </Summary>
struct match {
    public:
        /// <Summary>
        /// Pointer to the associated lexeme this match was extracted from
        /// </Summary>
        lexeme* lexemeptr;
        /// <Summary>
        /// Regular expression match contents
        /// </Summary>
        std::match_results<std::string::iterator> content;
        /// <Summary>
        /// Start row of match in source file
        /// </Summary>
        ulong row_start;
        /// <Summary>
        /// Start column of match in source file
        /// </Summary>
        ulong column_start;
        /// <Summary>
        /// End row of match in source file
        /// </Summary>
        ulong row_end;
        /// <Summary>
        /// End column of match in source file
        /// </Summary>
        ulong column_end;

        /// <Summary>
        /// Test if this match is valid
        /// </Summary>
        bool matchExists(){
           return !content.ready() || (content.empty() && content.size() == 0);
        }
};

/// <Summary>
/// Class representing a lexical literal using regular expressions
/// </Summary>
class lexeme {
    public:
        /// <Summary>
        /// Lexeme name
        /// </Summary>
        std::string name;
        /// <Summary>
        /// Lexeme regular expression pattern
        /// </Summary>
        std::string pattern;
        /// <Summary>
        /// Compiled regex lexeme
        /// </Summary>
        std::regex rx;
        /// <Summary>
        /// Flag to test if this lexeme can be skipped in results
        /// </Summary>
        bool skippable;

        /// <Summary>
        /// Create a lexeme from a name and regex pattern
        /// </Summary>
        lexeme(std::string name, std::string pattern): skippable(false), name(name), pattern(pattern), rx(pattern) {}

        /// <Summary>
        /// Test if this lexeme can be extracted from a string
        /// </Summary>
        bool matches(std::string::iterator begin, std::string::iterator end){
            return std::regex_search(begin, end, rx);
        }

        /// <Summary>
        /// Extract a match from a string
        /// </Summary>
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

/// <Summary>
/// Class representing an exception generated from the lexer
/// </Summary>
class lexicalexception : public std::exception {
    private:
        /// <Summary>
        /// Combined exception message
        /// </Summary>
        std::string what_message;

    public:
        /// <Summary>
        /// Row exception occurred on
        /// </Summary>
        ulong r;
        /// <Summary>
        /// Column exception occurred on
        /// </Summary>
        ulong c;
        /// <Summary>
        /// File name exception occurred in
        /// </Summary>
        std::string file;
        /// <Summary>
        /// Message for this exception
        /// </Summary>
        std::string msg;

        /// <Summary>
        /// Create a new exception
        /// </Summary>
        lexicalexception(ulong row, ulong column, std::string file, std::string message) : r(row), c(column), file(file), msg(message){
            std::stringstream sb; 
            sb << msg << " in " << file << " at [" << r << ", " << c << "]";
            what_message = sb.str();
        }

        /// <Summary>
        /// Print the message
        /// </Summary>
        const char* what() const throw() {
            return what_message.c_str();
        }
};

/// <Summary>
/// Class to tokenzie input string
/// </Summary>
class lexer {
    private:
        /// <Summary>
        /// List of lexemes to extract
        /// </Summary>
        std::vector<lexeme*> tokens;

    public:
        /// <Summary>
        /// Create a new lexer from a list of lexemes
        /// </Summary>
        lexer(std::vector<lexeme*> tokens): tokens(tokens){}

        /// <Summary>
        /// Tokenize an input string. Lexical exception will be thrown if an unrecognized symbol is reached
        /// </Summary>
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