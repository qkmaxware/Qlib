#ifndef _PARSING_H
#define _PARSING_H

#include "./lexer.hpp"

#include <memory> 
#include <iostream>

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

class rule {
    private:
    public:
        virtual~rule() {}
        virtual void parse() {}
};

typedef std::shared_ptr < rule > sp;

class repeater: public rule {
    private: 
        sp rl;
        repeat rep;
    public: 
        repeater(sp r, repeat rep): rl(r),rep(rep) {}
};

class terminal: public rule {
    private: 
        lexeme & lex;
    public: 
        terminal(lexeme & lex): lex(lex) {}
};

class sequence: public rule {
    private: 
        sp left;
        sp right;
    public: 
        sequence(sp first, sp next): left(first), right(next) {}
};

class choice: public rule {
    private: 
        sp left;
        sp right;
    public: 
        choice(sp first, sp second): left(first), right(second) {}
};
}

// -------------------------------------------------------------------
// Unitary
// -------------------------------------------------------------------

parser::sp operator * (parser::sp right) {
  return parser::sp((parser::rule * ) new parser::repeater(right, parser::KLEENE_STAR));
}

parser::sp operator + (parser::sp right) {
  return parser::sp((parser::rule * ) new parser::repeater(right, parser::KLEENE_PLUS));
}

parser::sp operator!(parser::sp right) {
  return parser::sp((parser::rule * ) new parser::repeater(right, parser::KLEENE_OPTIONAL));
}

parser::sp operator * (lexeme & right) {
  parser::sp pt = parser::sp((parser::rule * ) new parser::terminal(right));
  return parser::sp((parser::rule * ) new parser::repeater(pt, parser::KLEENE_STAR));
}

parser::sp operator + (lexeme & right) {
  parser::sp pt = parser::sp((parser::rule * ) new parser::terminal(right));
  return parser::sp((parser::rule * ) new parser::repeater(pt, parser::KLEENE_PLUS));
}

parser::sp operator!(lexeme & right) {
  parser::sp pt = parser::sp((parser::rule * ) new parser::terminal(right));
  return parser::sp((parser::rule * ) new parser::repeater(pt, parser::KLEENE_OPTIONAL));
}

// -------------------------------------------------------------------
// Binary
// -------------------------------------------------------------------

parser::sp operator << (parser::sp left, parser::sp right) {
  return parser::sp((parser::rule * ) new parser::sequence(left, right));
}

parser::sp operator << (parser::sp left, lexeme & right) {
  return parser::sp((parser::rule * ) new parser::sequence(left,
    parser::sp((parser::rule * ) new parser::terminal(right))));
}

parser::sp operator << (lexeme & left, parser::sp right) {
  return parser::sp((parser::rule * ) new parser::sequence(parser::sp((parser::rule * ) new parser::terminal(left)), right));
}

parser::sp operator << (lexeme & left, lexeme & right) {
  return parser::sp((parser::rule * ) new parser::sequence(parser::sp((parser::rule * ) new parser::terminal(left)),
    parser::sp((parser::rule * ) new parser::terminal(right))));
}

parser::sp operator | (parser::sp left, parser::sp right) {
  return parser::sp((parser::rule * ) new parser::choice(left, right));
}

parser::sp operator | (parser::sp left, lexeme & right) {
  return parser::sp((parser::rule * ) new parser::choice(left,
    parser::sp((parser::rule * ) new parser::terminal(right))));
}

parser::sp operator | (lexeme & left, parser::sp right) {
  return parser::sp((parser::rule * ) new parser::choice(parser::sp((parser::rule * ) new parser::terminal(left)), right));
}

parser::sp operator | (lexeme & left, lexeme & right) {
  return parser::sp((parser::rule * ) new parser::choice(parser::sp((parser::rule * ) new parser::terminal(left)),
    parser::sp((parser::rule * ) new parser::terminal(right))));
}

#endif