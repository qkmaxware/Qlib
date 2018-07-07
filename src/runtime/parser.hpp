#ifndef _PARSING_H
#define _PARSING_H

#include "./lexer.hpp"

#include <memory> 
#include <iostream>
#include <vector>

using namespace std;
using namespace lexical;

/// <Summary>
/// Namespace for parser library
/// </Summary>
namespace parser {

/// <Summary>
/// Class representing how many times a thing is to be repeated
/// </Summary>
class repeat {
    private:
        long min;
        long max;
        bool hasMax;
 
    public:
        repeat(): min(1), max(1), hasMax(true) {}
        repeat(long min): min(min), max(0), hasMax(false) {}
        repeat(long min, long max) : min(min), max(max), hasMax(true) {}
        repeat(const repeat& rep) : min(rep.min), max(rep.max), hasMax(rep.hasMax) {}
 
        bool inRange(long x){
            return x >= min && (!hasMax || x <= max);
        }
 
        bool underMin(long x){
            return x < min;
        }
 
        bool overMax(long x){
            return hasMax && x > max;
        }
 
        bool hasRepetitionsRemaining(long x){
            return underMin(x) || inRange(x);
        }
};

/// <Summary>
/// Repeat 0 to infinite times
/// </Summary>
const repeat KLEENE_STAR     = repeat(0);
/// <Summary>
/// Repeat 1 to infinite times
/// </Summary>
const repeat KLEENE_PLUS     = repeat(1);
/// <Summary>
/// Repeat exactly once
/// </Summary>
const repeat KLEENE_ONCE     = repeat(1,1);
/// <Summary>
/// Repeat 0 or 1 times
/// </Summary>
const repeat KLEENE_OPTIONAL = repeat(0,1);

/// <Summary>
/// Class representing a list of tokens with a pointer that can be changed
/// </Summary>
class tokenqueue {
    private:
        /// <Summary>
        /// Token list
        /// </Summary>
        vector<match> tokens;
        /// <Summary>
        /// Index to currently selected token
        /// </Summary>
        size_t index;
    public:
        /// <Summary>
        /// Tokenqueue from vector
        /// </Summary>
        tokenqueue(vector<match>& toks) : tokens(toks), index(0) {}
        /// <Summary>
        /// Elements remaining in the queue
        /// </Summary>
        size_t length(){
            return tokens.size() - index;
        }
        /// <Summary>
        /// Test if queue has more tokens
        /// </Summary>
        bool hasNext(){
            return index < tokens.size();
        }
        /// <Summary>
        /// Get current index in order to restore back to this point
        /// </Summary>
        size_t getRestorePoint(){
            return index;
        }
        /// <Summary>
        /// Start from the begining of the queue
        /// </Summary>
        void reset(){
            restore(0);
        }
        /// <Summary>
        /// Set the pointer to a particular element
        /// </Summary>
        void restore(size_t restorePoint){
            index = restorePoint;
        }
        /// <Summary>
        /// Get the 'n'th element in the queue
        /// </Summary>
        match& operator[](int ind){
            return tokens[ind];
        }
        /// <Summary>
        /// Get the current element under the pointer
        /// </Summary>
        match& peek(){
            return tokens[index];
        }
        /// <Summary>
        /// Get the element under the pointer and increments the pointer
        /// </Summary>
        match& pop(){
            index++;
            return tokens[index - 1];
        }
        /// <Summary>
        /// Increment the pointer if possible
        /// </Summary>
        void next(){
            if(hasNext()){
                pop();
            }
        }
};

/// <Summary>
/// Class representing a single node in an abstract tree structure
/// </Summary>
struct parsetree {
    /// <Summary>
    /// Lexical match associated with this node
    /// </Summary>
    match value;
    /// <Summary>
    /// Children of this node (usually 2)
    /// </Summary>
    vector<parsetree> children;
    /// <Summary>
    /// Check if node has no children
    /// </Summary>
    bool isLeaf(){
        return children.size() < 1;
    }
    /// <Summary>
    /// Check if node has children
    /// </Summary>
    bool isBranch(){
       return !isLeaf();
    }
    /// <Summary>
    /// Test if the parse tree node is binary
    /// </Summary>
    bool isBinary(){
        return children.size() == 2;
    }
    /// <Summary>
    /// If tree is binary, get the left hand child (index 0)
    /// </Summary>
    parsetree& getLeft(){
        return children[0];
    }
    /// <Summary>
    /// If tree is binary, get the right hand child (index 1)
    /// </Summary>
    parsetree& getRight(){
        return children[1];
    }
    /// <Summary>
    /// Number of children elements
    /// </Summary>
    size_t size(){
        return children.size();
    }
    /// <Summary>
    /// Get a specific child element
    /// </Summary>
    parsetree& getChild(size_t i){
        return children[i];
    }
    /// <Summary>
    /// Print tree to standard out, usefull in debugging
    /// </Summary>
    void print(){
        if(isLeaf()){
            cout << "(match:" << value.toString() << ")";
        }
        else{
            cout << "(";
            bool first = true;
            for(size_t i = 0; i < children.size(); i++){
                if(!first){
                    cout << " ";
                }
                first = false;
                children[i].print();
            }
            cout << ")";
        }
    }
};

/// <Summary>
/// Interface for all parse rules
/// </Summary>
class rule {
    private:
    public:
        virtual~rule() {}
        /// <Summary>
        /// Virtual function to parse an input tokenqueue into a parse tree
        /// Returns true if parsing successful and sets the value under the parsetree pointer
        /// </Summary>
        virtual bool tryParse(tokenqueue& queue, parsetree* ref) { return false; }
};

/// <Summary>
/// A shared pointer to a rule
/// </Summary>
typedef std::shared_ptr<rule> ruleptr;

/// <Summary>
/// Class representing parsing elements that are repeated
/// </Summary>
class repeater: public rule {
    private: 
        ruleptr rl;
        repeat rep;
    public: 
        repeater(ruleptr r, repeat rep): rl(r),rep(rep) {
            
        }
        ~repeater(){
            
        }
        bool tryParse(tokenqueue& queue, parsetree* ref) { 
            parsetree cn;
            size_t rp = queue.getRestorePoint();
            while(rep.hasRepetitionsRemaining(cn.children.size())){
                parsetree node;
                size_t r = queue.getRestorePoint();
                if(!rl->tryParse(queue, &node)){
                    queue.restore(r);
                    break;
                }
                cn.children.push_back(node);
            }

            if(rep.inRange(cn.children.size())){
                *ref = cn;
                return true;
            }
            
            queue.restore(rp);
            return false;
        }
};

/// <Summary>
/// Class representing parsing elements that match a lexeme
/// </Summary>
class terminal: public rule {
    private: 
        lexeme& lex;
    public: 
        terminal(lexeme& lex): lex(lex) {
            
        }
        ~terminal(){
            
        }
        bool tryParse(tokenqueue& queue, parsetree* ref){
            parsetree node;
            if(queue.hasNext() && queue.peek().lexemeptr == &lex){
                node.value = queue.pop();
                *ref = node;
                return true;
            }
            return false;
        }
};

/// <Summary>
/// Class representing parsing elements that follow each other
/// </Summary>
class sequence: public rule {
    private: 
        ruleptr left;
        ruleptr right;
    public: 
        sequence(ruleptr first, ruleptr next): left(first), right(next) {
            
        }
        ~sequence(){
            
        }
        bool tryParse(tokenqueue& queue, parsetree* ref) {
            parsetree node;
            size_t r = queue.getRestorePoint();
            parsetree ln;
            parsetree rn;
            if(!left->tryParse(queue, &ln)){
                queue.restore(r);
                return false;
            }
            if(!right->tryParse(queue, &rn)){
                queue.restore(r);
                return false;
            }
            node.children.push_back(ln);
            node.children.push_back(rn);
            *ref = node;
            return true;
        }
};

/// <Summary>
/// Class representing parsing elements that are either one thing exclusive or another
/// </Summary>
class choice: public rule {
    private: 
        ruleptr left;
        ruleptr right;
    public: 
        choice(ruleptr first, ruleptr second): left(first), right(second) {
            
        }
        ~choice(){
            
        }
        bool tryParse(tokenqueue& queue, parsetree* ref){
            parsetree node;
            size_t r = queue.getRestorePoint();
            if(left->tryParse(queue, &node)){
                *ref = node;
                return true;
            }
            queue.restore(r);
            if(right->tryParse(queue, &node)){
                *ref = node;
                return true;
            }
            queue.restore(r);
            return false;
        }
};
}

// -------------------------------------------------------------------
// Unitary
// -------------------------------------------------------------------

/// <Summary>
/// Repeat a rule 
/// </Summary>
parser::ruleptr operator * (parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::repeater(right, parser::KLEENE_STAR));
}

/// <Summary>
/// Repeat a rule 
/// </Summary>
parser::ruleptr operator + (parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::repeater(right, parser::KLEENE_PLUS));
}

/// <Summary>
/// Repeat a rule 
/// </Summary>
parser::ruleptr operator!(parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::repeater(right, parser::KLEENE_OPTIONAL));
}

/// <Summary>
/// Create terminal rule and repeat it 
/// </Summary>
parser::ruleptr operator * (lexeme & right) {
  parser::ruleptr pt = parser::ruleptr((parser::rule * ) new parser::terminal(right));
  return parser::ruleptr((parser::rule * ) new parser::repeater(pt, parser::KLEENE_STAR));
}

/// <Summary>
/// Create terminal rule and repeat it 
/// </Summary>
parser::ruleptr operator + (lexeme & right) {
  parser::ruleptr pt = parser::ruleptr((parser::rule * ) new parser::terminal(right));
  return parser::ruleptr((parser::rule * ) new parser::repeater(pt, parser::KLEENE_PLUS));
}

/// <Summary>
/// Create terminal rule and repeat it 
/// </Summary>
parser::ruleptr operator!(lexeme & right) {
  parser::ruleptr pt = parser::ruleptr((parser::rule * ) new parser::terminal(right));
  return parser::ruleptr((parser::rule * ) new parser::repeater(pt, parser::KLEENE_OPTIONAL));
}

// -------------------------------------------------------------------
// Binary
// -------------------------------------------------------------------

/// <Summary>
/// Create sequence of rules
/// </Summary>
parser::ruleptr operator << (parser::ruleptr left, parser::ruleptr right) {
  return parser::ruleptr((parser::rule*) new parser::sequence(left, right));
}

/// <Summary>
/// Create terminal rule and then add it to a sequence of rules
/// </Summary>
parser::ruleptr operator << (parser::ruleptr left, lexeme & right) {
  return parser::ruleptr(
    (parser::rule*) new parser::sequence(
        left,
        parser::ruleptr((parser::rule*) new parser::terminal(right))
    ));
}

/// <Summary>
/// Create terminal rule and then add it to a sequence of rules
/// </Summary>
parser::ruleptr operator << (lexeme & left, parser::ruleptr right) {
  return parser::ruleptr(
    (parser::rule * ) new parser::sequence(
          parser::ruleptr((parser::rule*) new parser::terminal(left)), 
          right
    ));
}

/// <Summary>
/// Create terminal rule and then add it to a sequence of rules
/// </Summary>
parser::ruleptr operator << (lexeme & left, lexeme & right) {
  return parser::ruleptr((parser::rule*) new parser::sequence(
        parser::ruleptr((parser::rule*) new parser::terminal(left)),
        parser::ruleptr((parser::rule*) new parser::terminal(right))
    ));
}

/// <Summary>
/// Create a choice between 2 rules
/// </Summary>
parser::ruleptr operator | (parser::ruleptr left, parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(left, right));
}

/// <Summary>
/// Create a terminal rule and add it to a choice between 2 rules
/// </Summary>
parser::ruleptr operator | (parser::ruleptr left, lexeme & right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(left,
    parser::ruleptr((parser::rule * ) new parser::terminal(right))));
}

/// <Summary>
/// Create a terminal rule and add it to a choice between 2 rules
/// </Summary>
parser::ruleptr operator | (lexeme & left, parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(parser::ruleptr((parser::rule * ) new parser::terminal(left)), right));
}

/// <Summary>
/// Create a terminal rule and add it to a choice between 2 rules
/// </Summary>
parser::ruleptr operator | (lexeme & left, lexeme & right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(parser::ruleptr((parser::rule * ) new parser::terminal(left)),
    parser::ruleptr((parser::rule * ) new parser::terminal(right))));
}

#endif