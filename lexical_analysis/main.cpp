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
    void _operator();
    void composite_operator();
    void expression_operator();
    
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
                    <composite_operator> | <expression_operator>
 <composite_operator>   -> "{"<_operator>"}"
 <expression_operator>  -> <expression>
 
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

// <operators> -> { <_operator> }
/* <_operator> ->   "if"    "("<expression>")" <_operator> else <_operator> |
                    "while" "("<expression>")" <_operator>                  |
                    "read"  "("<identifier>")" ";"                          |
                    "write" "("<expression> { ","<expression> }")" ";"      |
                    <composite_operator> | <expression_operator>
   <composite_operator>   -> "{"<_operator>"}"
   <expression_operator>  -> <expression>
*/

void Parser::operators(){}
void Parser::_operator(){}
void Parser::composite_operator(){}
void Parser::expression_operator(){}

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
