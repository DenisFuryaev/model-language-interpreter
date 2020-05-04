//
//  main.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 19.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>

#include "LexClass.hpp"
#include "LexerClass.hpp"
#include "IdentClass.hpp"
#include "Tabl_Ident_Class.hpp"
#include "Exceptions.hpp"
#include "Poliz.hpp"

Tabl_Ident TID(100);

class Parser {
    
public:
    Parser(const char * program_file_path): lexer(program_file_path){}
    void analyze();
    
private:
    Lexer lexer;
    
    Lex curr_lex;
    Lex::type_of_lex curr_lex_type;
    const char * curr_lex_value;

private:
    
    void get_lex();
    void expect(Lex::type_of_lex lex_type, const char * error_message);
    
    void program();
    void declarations();
    bool declaration();
    //void type();
    void variable(Ident::var_type var_type);
    void constant();
    void integer();
    void sign();
    void string();
    void logical();
    void operators();
    bool _operator();
    void composite_operator();
    bool expression_operator();
    bool expression();
    bool expression_1();
    bool T();
    bool F();
    
};
//------------------------------------------------------------------------------------------------------------------------------------------------------

/*

 <program>      ->  program "{" <declarations> <operators> "}"
 <declarations> ->  { <declaration>";" }
 <declaration>  ->  <type> <variable> { ","<variable> }
 <type>         ->  "int" | "string" | "boolean"
 <variable>     ->  <identifier> | <identifier> "=" <constant>
 <constant>     ->  <integer> | <string> | <logical>
 <integer>      ->  [<sign>] <numeral> { <numeral> }
 <sign>         ->  "+" | "-"
 <string>       ->  """{ literal }"""
 <logical>      ->  "true" | "false"
 <operators>    ->  { <_operator> }
 <_operator>    ->  "if"    "("<expression>")" <_operator> else <_operator> |
                    "while" "("<expression>")" <_operator>                  |
                    "read"  "("<identifier>")" ";"                          |
                    "write" "("<expression> { ","<expression> }")" ";"      |
                    "for"   "("[<expression>]";"[<expression>]";"[<expression>]")" <_operator> |
                    <composite_operator> | <expression_operator>
 <composite_operator>   -> "{"<_operator>"}"
 <expression_operator>  -> <expression>";"
 <expression>   -> <expression_1> [ "<" | ">" | "!=" | "<=" | ">=" | "==" ] <expression_1> | <expression_1>
 <expression1_> -> <T> { [ "+" | "-" | "or" ] <T> }
 <T>            -> <F> { [ "*" | "/" | "and" ] <F> }
 <F>            -> <identifier> | <number> | <L> | "not" <F> | "("<expression>")"
 <L>            -> "true" | "false"
*/


void Parser::get_lex() {
    try {
        curr_lex = lexer.get_lex();
    }
    catch(LexExeption lex_exeption) {
        std::cout << "unhandled error in lexer: " << lex_exeption.get_message() << std::endl;
    }
    catch(Exeption exeption) {
        std::cout << "unhandled error in lexer: " << exeption.get_message() << std::endl;
    }
    curr_lex_type = curr_lex.get_type();
    curr_lex_value = curr_lex.get_value();
}

void Parser::expect(Lex::type_of_lex lex_type, const char * error_message) {
    get_lex();
    if (curr_lex_type != lex_type)
        throw Exeption(error_message);
}

void Parser::analyze() {
    program();
    std::cout << "successfully parsed" << std::endl;
}

 // <program> ->  program "{" <declarations> <operators> "}"
void Parser::program() {
    get_lex();
    if (curr_lex_type != Lex::PROGRAM)
        throw LexExeption(curr_lex.get_type(), curr_lex.get_value());
    
    get_lex();
    if (curr_lex_type != Lex::OPEN_BRACES)
        throw LexExeption(curr_lex.get_type(), curr_lex.get_value());
    
    declarations();
    operators();
    
    get_lex();
    if (curr_lex_type == Lex::CLOSE_BRACES)
        get_lex();
    else
        throw LexExeption(curr_lex.get_type(), curr_lex.get_value());
    if (curr_lex_type != Lex::FIN)
        throw LexExeption(curr_lex.get_type(), curr_lex.get_value());
}

// <declarations> -> { <declaration>";" }
void Parser::declarations() {
    while (declaration()) {}
}

// <declaration> -> <type> <variable> { ","<variable> }
// <type>        -> "int" | "string" | "boolean"
bool Parser::declaration() {
    get_lex();
    Ident::var_type var_type;
    switch (curr_lex_type) {
        case Lex::INT:
            var_type = Ident::INT;
            break;
        case Lex::STRING_TYPE:
            var_type = Ident::STR;
            break;
        case Lex::BOOLEAN:
            var_type = Ident::BOOL;
            break;
        default:
            lexer.put_lex(curr_lex);
            return false;
    }
    
    while (true) {
        variable(var_type);
        get_lex();
        if (curr_lex_type != Lex::COMMA) {
            lexer.put_lex(curr_lex);
            break;
        }
    }
    
    get_lex();
    if (curr_lex_type != Lex::SEMICOLON)
        throw LexExeption(curr_lex.get_type(), curr_lex.get_value());
    return true;
}

// <variable> -> <identifier> | <identifier> "=" <constant>
// <constant> -> <integer> | <string> | <logical>
void Parser::variable(Ident::var_type var_type) {
    get_lex();
    if (curr_lex_type == Lex::IDENT) {
        // put ident in table of idents
        const char * var_name = curr_lex_value;
        Ident * ident = NULL;
        
        switch (var_type) {
            case Ident::INT:
                ident = new IntIdent(var_name);
                if (!TID.put(ident))
                    throw Exeption("double declaration"); // double declaration error
                break;
            case Ident::STR:
                ident = new StringIdent(var_name);
                if (!TID.put(ident))
                    throw Exeption("double declaration"); // double declaration error
                break;
            case Ident::BOOL:
                ident = new BoolIdent(var_name);
                if (!TID.put(ident))
                    throw Exeption("double declaration"); // double declaration error
                break;
            default:
                throw LexExeption(curr_lex.get_type(), curr_lex.get_value()); // no need for this throw ???
                break;
        }
        
        get_lex();
        if (curr_lex_type == Lex::ASSIGN) {
            get_lex();
            if  (((var_type == Ident::INT) || (curr_lex_type == Lex::NUM)) ||
                ((var_type == Ident::STR) || (curr_lex_type == Lex::STRING))  ||
                ((var_type == Ident::BOOL) || (curr_lex_type == Lex::_TRUE) || (curr_lex_type == Lex::_FALSE))) {
                ident->put_value(curr_lex_value);
            }
            else
                lexer.put_lex(curr_lex);
        }
        else lexer.put_lex(curr_lex);
    }
    else throw LexExeption(curr_lex.get_type(), curr_lex.get_value());
}

/*
    <operators> -> { <_operator> }
    <_operator> ->  "if"    "("<expression>")" <_operator> else <_operator> |
                    "while" "("<expression>")" <_operator>                  |
                    "read"  "("<identifier>")" ";"                          |
                    "write" "("<expression> { ","<expression> }")" ";"      |
                    <composite_operator> |
                    <expression_operator>
    <composite_operator>  -> "{"<_operator>"}"
    <expression_operator> -> <expression>";"
    <expression>   -> <expression_1> [ "<" | ">" | "!=" | "<=" | ">=" | "=="] <expression_1> | <expression_1>
    <expression1_> -> <T> { [ "+" | "-" | "or" | "="] <T> }
    <T>            -> <F> { [ "*" | "/" | "and" ] <F> }
    <F>            -> <identifier> | <number> | <L> | "not" <F> | "("<expression>")"
    <L>            -> "true" | "false"
*/

void Parser::operators() {
    while(_operator()) {}
}

bool Parser::_operator() {
    
    get_lex();
    switch (curr_lex_type) {

        // "if" "(" <expression> ")" <_operator> else <_operator>
        case Lex::IF:
            
            expect(Lex::OPEN_PAREN, "_operator IF: expected OPEN_PAREN");
            expression();
            expect(Lex::CLOSE_PAREN, "_operator IF: expected CLOSE_PAREN");
            _operator();
            expect(Lex::ELSE, "_operator IF: expected ELSE");
            _operator();
            
            return true;
            break;
            
        // "for" "(" [<expression>] ";" [<expression>] ";" [<expression>] ")" <_operator>
        case Lex::FOR:
        
            expect(Lex::OPEN_PAREN, "_operator FOR: expected OPEN_PAREN");
            expression();
            expect(Lex::SEMICOLON, "_operator FOR: expected SEMICOLON");
            expression();
            expect(Lex::SEMICOLON, "_operator FOR: expected SEMICOLON");
            expression();
            expect(Lex::CLOSE_PAREN, "_operator FOR: expected CLOSE_PAREN");
            _operator();
        
            return true;
            break;
            
        // "while" "(" <expression> ")" <_operator>
        case Lex::WHILE:
            
            expect(Lex::OPEN_PAREN, "_operator WHILE: expected OPEN_PAREN");
            expression();
            expect(Lex::CLOSE_PAREN, "_operator WHILE: expected CLOSE_PAREN");
            _operator();
            
            return true;
            break;
            
        // "read" "(" <identifier> ")" ";"
        case Lex::READ:
            
            expect(Lex::OPEN_PAREN, "_operator READ: expected OPEN_PAREN");
            expect(Lex::IDENT, "_operator READ: expected IDENT");
            expect(Lex::CLOSE_PAREN, "_operator READ: expected CLOSE_PAREN");
            expect(Lex::SEMICOLON, "_operator READ: expected SEMICOLON");
            
            return true;
            break;
            
        // "write" "(" <expression> { "," <expression> }" )" ";"
        case Lex::WRITE:
            expect(Lex::OPEN_PAREN, "_operator WRITE: expected OPEN_PAREN");
            while (true) {
                expression();
                get_lex();
                if (curr_lex_type != Lex::COMMA) {
                    lexer.put_lex(curr_lex);
                    break;
                }
            }
            expect(Lex::CLOSE_PAREN, "_operator WRITE: expected CLOSE_PAREN");
            expect(Lex::SEMICOLON, "_operator WRITE: expected SEMICOLON");
            
            return true;
            break;
            
        case Lex::OPEN_BRACES:
            lexer.put_lex(curr_lex);
            composite_operator();
            return true;
            break;
            
        default:
            lexer.put_lex(curr_lex);
            bool result = expression_operator();
            return result;
            break;
    }
    
    
}

// <composite_operator> -> "{" <operators> "}"
void Parser::composite_operator() {
    
    expect(Lex::OPEN_BRACES, "composite_operator: expected OPEN_BRACES");
    operators();
    expect(Lex::CLOSE_BRACES, "composite_operator: expected CLOSE_BRACES");
}

// <expression_operator> -> <expression>";"
bool Parser::expression_operator() {
    bool result = expression();
    if (!result)
        return result;
    
    get_lex();
    if (curr_lex_type != Lex::SEMICOLON)
        throw Exeption("expression_operator: SEMICOLON missing");
    
    return result;
}


/*
 <expression>   -> <expression_1> [ "<" | ">" | "!=" | "<=" | ">=" | "==" ] <expression_1> | <expression_1>
 <expression1_> -> <T> { [ "+" | "-" | "or" | "=" ] <T> }
 <T>            -> <F> { [ "*" | "/" | "and" ] <F> }
 <F>            -> <identifier> | <number> | <L> | "not" <F> | "("<expression>")"
 <L>            -> "true" | "false"
*/



// <expression> -> <expression_1> [ "<" | ">" | "!=" | "<=" | ">=" | "==" ] <expression_1> | <expression_1>
bool Parser::expression() {
    bool result = expression_1();
    if (!result)
        return result;
    
    get_lex();
    if ((curr_lex_type == Lex::LESS) || (curr_lex_type == Lex::BIGGER) || (curr_lex_type == Lex::NOT_EQUAL) ||
        (curr_lex_type == Lex::LESS_EQUAL) || (curr_lex_type == Lex::BIGGER_EQUAL) || (curr_lex_type == Lex::EQUAL))
        expression_1();
    else
        lexer.put_lex(curr_lex);
    
    return result;
}

// <expression1_> -> <T> { [ "+" | "-" | "or" | "=" ] <T> }
bool Parser::expression_1() {
    bool result =  T();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        if ((curr_lex_type == Lex::PLUS) || (curr_lex_type == Lex::MINUS) || (curr_lex_type == Lex::OR) || (curr_lex_type == Lex::ASSIGN))
            T();
        else {
            lexer.put_lex(curr_lex);
            break;
        }
    }
    return result;
}

// <T> -> <F> { [ "*" | "/" | "and" ] <F> }
bool Parser::T() {
    bool result = F();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        if ((curr_lex_type == Lex::MUL) || (curr_lex_type == Lex::DIV) || (curr_lex_type == Lex::AND))
            F();
        else {
            lexer.put_lex(curr_lex);
            break;
        }
    }
    return result;
}

// <F> -> <identifier> | <number> | <L> | "not" <F> | "("<expression>")"
// <L> -> "true" | "false"
bool Parser::F() {
    
    get_lex();
    switch (curr_lex_type) {
        case Lex::IDENT:
            
            break;
            
        case Lex::NUM:
        
            break;
            
        case Lex::_TRUE:
        
            break;
            
        case Lex::_FALSE:
        
            break;
            
        case Lex::NOT:
        
            break;
            
        case Lex::OPEN_PAREN:
            expression();
            if (curr_lex_type != Lex::CLOSE_PAREN)
                throw Exeption("F: CLOSE_PAREN missing");
            break;
            
        default:
            lexer.put_lex(curr_lex);
            return false;
            //throw Exeption("F: member error");
            //break;
    }
    
    return true;
}

//======================================================================================================================================================

int main(int argc, const char * argv[]) {
    
    //Lexer lexer("program.txt");
    Lex lex;
    Parser parser("program.txt");
    try {
        parser.analyze();
    }
    catch (Exeption exeption) {
        std::cerr << "unhandled error: " << exeption.get_message() << std::endl;
    }

    
    TID.print();
    
    return 0;
}
