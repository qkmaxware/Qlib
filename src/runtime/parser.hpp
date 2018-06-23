#ifndef _PARSING_H
#define _PARSING_H

#include "./lexer.hpp"

/*
namespace parsing {

/// <Summary>
/// Class representing how many times something can be repeates
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
        virtual repeat& repetitions() = 0;
};

class choice : public rule {
    private:
        rule& left;
        rule& right;
        repeat rep;

    public:
        choice(rule& left, rule& right) : left(left), right(right) {}
        choice(repeat rep, rule& left, rule& right) : rep(rep), left(left), right(right) {}

        rule& car(){
            return left;
        }

        rule& cdr(){
            return right;
        }

        repeat& repetitions() {
            return rep;
        }
};

class sequence : public rule {
    private:
        rule& first;
        rule& next;
        repeat rep;

    public:
        sequence(rule& first, rule& next) : first(first), next(next) {}
        sequence(repeat rep, rule& first, rule& next) : rep(rep), first(first), next(next) {}

        rule& car(){
            return first;
        }

        rule& cdr(){
            return next;
        }

        repeat& repetitions() {
            return rep;
        }
};

class literal : public rule {
    private:
        lexical::lexeme& lex;
        repeat rep;

    public:
        literal(lexical::lexeme& lex): lex(lex) {}
        literal(lexical::lexeme& lex, repeat rep): lex(lex), rep(rep) {}

        lexical::lexeme& token(){
            return lex;
        }

        repeat& repetitions() {
            return rep;
        }
};

// -----------------------------------------------------------
// KLEENE operators
// -----------------------------------------------------------
literal operator *(literal& right){
    return literal(right.token(), KLEENE_STAR);
};

choice operator *(choice& right){
    return choice(KLEENE_STAR, right.car(), right.cdr());
};

sequence operator *(sequence& right){
    return sequence(KLEENE_STAR, right.car(), right.cdr());
};

literal operator +(literal& right){
    return literal(right.token(),KLEENE_PLUS);
};

sequence operator +(sequence& right){
    return sequence(KLEENE_PLUS, right.car(), right.cdr());
};

choice operator +(choice& right){
    return choice(KLEENE_PLUS, right.car(), right.cdr());
};

literal operator ~(literal& right){
    return literal(right.token(), KLEENE_OPTIONAL);
};

sequence operator ~(sequence& right){
    return sequence(KLEENE_OPTIONAL, right.car(), right.cdr());
};

choice operator ~(choice& right){
    return choice(KLEENE_OPTIONAL, right.car(), right.cdr());
};

// -----------------------------------------------------------
// Logical operators
// -----------------------------------------------------------

sequence operator >> (rule& left, rule& right) {
    return sequence(left, right);
};

choice operator | (rule& left, rule& right) {
    return choice(left, right);
};

// ----------------------------------------------------------
// Example use
// ----------------------------------------------------------


choice s = (literal(token) >> *literal(token2)) | literal(token3);


}*/


#endif