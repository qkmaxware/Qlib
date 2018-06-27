#ifndef _PARSING_H
#define _PARSING_H

#include "./lexer.hpp"

#include <memory> 
#include <iostream>
#include <vector>

using namespace std;
using namespace lexical;

namespace parser {

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
 
const repeat KLEENE_STAR     = repeat(0);
const repeat KLEENE_PLUS     = repeat(1);
const repeat KLEENE_ONCE     = repeat(1,1);
const repeat KLEENE_OPTIONAL = repeat(0,1);

class tokenqueue {
    private:
        vector<match> tokens;
        size_t index;
    public:
        tokenqueue(vector<match>& toks) : tokens(toks) {}
        size_t length(){
            return tokens.size() - index;
        }
        bool hasNext(){
            return index < tokens.size();
        }
        size_t getRestorePoint(){
            return index;
        }
        void reset(){
            restore(0);
        }
        void restore(size_t restorePoint){
            index = restorePoint;
        }
        match& operator[](int ind){
            return tokens[ind];
        }
        match& peek(){
            return tokens[index];
        }
        match& pop(){
            index++;
            return tokens[index - 1];
        }
        void next(){
            if(hasNext()){
                pop();
            }
        }
};

struct parsetree {
    match value;
    vector<parsetree> children;
        
    bool isLeaf(){
        return children.size() < 1;
    }
    bool isBranch(){
        return !isLeaf();
    }
};

class rule {
    private:
    public:
        virtual~rule() {}
        virtual bool tryParse(tokenqueue& queue, parsetree* ref) { return false; }
};

typedef std::shared_ptr<rule> ruleptr;

class repeater: public rule {
    private: 
        ruleptr rl;
        repeat rep;
    public: 
        repeater(ruleptr r, repeat rep): rl(r),rep(rep) {}
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

class terminal: public rule {
    private: 
        lexeme& lex;
    public: 
        terminal(lexeme& lex): lex(lex) {}
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

class sequence: public rule {
    private: 
        ruleptr left;
        ruleptr right;
    public: 
        sequence(ruleptr first, ruleptr next): left(first), right(next) {}
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

class choice: public rule {
    private: 
        ruleptr left;
        ruleptr right;
    public: 
        choice(ruleptr first, ruleptr second): left(first), right(second) {}
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

parser::ruleptr operator * (parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::repeater(right, parser::KLEENE_STAR));
}

parser::ruleptr operator + (parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::repeater(right, parser::KLEENE_PLUS));
}

parser::ruleptr operator!(parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::repeater(right, parser::KLEENE_OPTIONAL));
}

parser::ruleptr operator * (lexeme & right) {
  parser::ruleptr pt = parser::ruleptr((parser::rule * ) new parser::terminal(right));
  return parser::ruleptr((parser::rule * ) new parser::repeater(pt, parser::KLEENE_STAR));
}

parser::ruleptr operator + (lexeme & right) {
  parser::ruleptr pt = parser::ruleptr((parser::rule * ) new parser::terminal(right));
  return parser::ruleptr((parser::rule * ) new parser::repeater(pt, parser::KLEENE_PLUS));
}

parser::ruleptr operator!(lexeme & right) {
  parser::ruleptr pt = parser::ruleptr((parser::rule * ) new parser::terminal(right));
  return parser::ruleptr((parser::rule * ) new parser::repeater(pt, parser::KLEENE_OPTIONAL));
}

// -------------------------------------------------------------------
// Binary
// -------------------------------------------------------------------

parser::ruleptr operator << (parser::ruleptr left, parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::sequence(left, right));
}

parser::ruleptr operator << (parser::ruleptr left, lexeme & right) {
  return parser::ruleptr((parser::rule * ) new parser::sequence(left,
    parser::ruleptr((parser::rule * ) new parser::terminal(right))));
}

parser::ruleptr operator << (lexeme & left, parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::sequence(parser::ruleptr((parser::rule * ) new parser::terminal(left)), right));
}

parser::ruleptr operator << (lexeme & left, lexeme & right) {
  return parser::ruleptr((parser::rule * ) new parser::sequence(parser::ruleptr((parser::rule * ) new parser::terminal(left)),
    parser::ruleptr((parser::rule * ) new parser::terminal(right))));
}

parser::ruleptr operator | (parser::ruleptr left, parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(left, right));
}

parser::ruleptr operator | (parser::ruleptr left, lexeme & right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(left,
    parser::ruleptr((parser::rule * ) new parser::terminal(right))));
}

parser::ruleptr operator | (lexeme & left, parser::ruleptr right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(parser::ruleptr((parser::rule * ) new parser::terminal(left)), right));
}

parser::ruleptr operator | (lexeme & left, lexeme & right) {
  return parser::ruleptr((parser::rule * ) new parser::choice(parser::ruleptr((parser::rule * ) new parser::terminal(left)),
    parser::ruleptr((parser::rule * ) new parser::terminal(right))));
}

#endif