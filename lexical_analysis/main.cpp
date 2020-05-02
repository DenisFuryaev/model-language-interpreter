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

Tabl_Ident TID(100);

class Parser {
    
public:
    Parser(const char * program_file_path): lexer(program_file_path){}
    
private:
    Lexer lexer;
    
    Lex curr_lex;
    Lex::type_of_lex curr_lex_type;
    const char * curr_lex_value;

private:
    
    void get_lex();
    void analyze();
    
    void program();
    void declarations();
    void declaration();
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
    curr_lex = lexer.get_lex();
    curr_lex_type = curr_lex.get_type();
    curr_lex_value = curr_lex.get_value();
}

void Parser::analyze() {
    get_lex();
    program();
    std::cout << "successfully parsed" << std::endl;
}

 // <program> ->  program "{" <declarations> <operators> "}"
void Parser::program() {
    if (curr_lex_type == Lex::PROGRAM)
        get_lex();
    else
        throw curr_lex;
    if (curr_lex_type == Lex::OPEN_BRACES)
        get_lex();
    else
        throw curr_lex;
    
    declarations();
    operators();
    
    if (curr_lex_type == Lex::CLOSE_BRACES)
        get_lex();
    else
        throw curr_lex;
    if (curr_lex_type != Lex::FIN)
        throw curr_lex;
}

// <declarations> -> { <declaration>";" }
void Parser::declarations() {
    declaration();
    if (curr_lex_type == Lex::SEMICOLON)
        get_lex();
    else
        throw curr_lex;
}

// <declaration> -> <type> <variable> { ","<variable> }
// <type>        -> "int" | "string" | "boolean"
void Parser::declaration() {
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
            throw curr_lex;
    }
    
    variable(var_type);
    
    while (curr_lex_type == Lex::COMMA)
        variable(var_type);
}

// <variable> -> <identifier> | <identifier> "=" <constant>
// <constant> -> <integer> | <string> | <logical>
void Parser::variable(Ident::var_type var_type) {
    if (curr_lex_type == Lex::IDENT) {
        // put ident in table of idents
        const char * var_name = curr_lex_value;
        Ident * ident = NULL;
        int index = -1;
        
        switch (var_type) {
            case Ident::INT:
                ident = new IntIdent(var_name);
                if ((index = TID.put(ident)))
                    throw curr_lex; // double declaration error
                break;
            case Ident::STR:
                ident = new StringIdent(var_name);
                if ((index = TID.put(ident)))
                    throw curr_lex; // double declaration error
                break;
            case Ident::BOOL:
                ident = new BoolIdent(var_name);
                if ((index = TID.put(ident)))
                    throw curr_lex; // double declaration error
                break;
            default:
                throw curr_lex; // no need for this throw ???
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
        }
    }
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

//======================================================================================================================================================

int main(int argc, const char * argv[]) {
    
    Lexer lexer("program.txt");
    Lex lex;
    do {
        try {
            lex = lexer.get_lex();
        }
        catch (Lex::type_of_lex lex_type){
            //std::cerr << "unhandled error in: " << type_of_lex_readable[lex_type] << std::endl;
            
            // why does main cpp program don't khow about type_of_lex_readable ??????
            
            std::cerr << "unhandled error in: " << lex_type << std::endl;
            break;
        }
        catch (char c){
            std::cerr << "unhandled error in char: " << c << std::endl;
            break;
        }
        catch (int x){
            if (x == -1)
                std::cerr << "unhandled error: buffer overflow" << std::endl;
            if (x == -2)
                std::cerr << "unhandled error: comment does not closedÇ" << std::endl;
            break;
        }
        std::cout << lex;
    }   while(lex.get_type() != Lex::FIN);
    std::cout << "\n";
    
    return 0;
}
