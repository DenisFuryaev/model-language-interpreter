//
//  main.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 19.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>

#include "LexClass.hpp"
#include "LexerClass.hpp"
#include "IdentClass.hpp"
#include "Tabl_Ident_Class.hpp"
#include "Exceptions.hpp"
#include "Poliz.hpp"

Tabl_Ident TID(100);

class Parser {
    
public:
    Parser(const char * program_file_path): lexer(program_file_path), prog(1000){}
    void analyze();
    
public:
    Poliz prog;
    
private:
    Lexer lexer;
    
    Lex curr_lex;
    Lex::type_of_lex curr_lex_type;
    const char * curr_lex_value;

private:
    
    std::stack <Lex::type_of_lex> st_lex;
    
    void get_lex();
    void expect(Lex::type_of_lex lex_type, const char * error_message);
    void check_op();
    
    void program();
    void declarations();
    bool declaration();
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
    bool e1();
    bool e2();
    bool e3();
    bool e4();
    bool e5();
    bool e6();
    bool e7();
    
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
 <expression> -> <e1> | <e1> "=" <e1>
 <e1> -> <e2> | <e2> "or" <e2>
 <e2> -> <e3> | <e3> "and" <e3>
 <e3> -> <e4> | <e4> [ "<" | ">" | "!=" | "<=" | ">=" | "==" ] <e4>
 <e4> -> <e5> | <e5> [ "+" | "-" ] <e5>
 <e5> -> <e6> | <e6> [ "*" | "/" ] <e6>
 <e6> -> <e7> | <e7> "not" <e7>
 <e7> -> <identifier> | <number> | "true" | "false" | "("<expression>")"

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
    prog.print();
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
                    throw Exeption("variable: double declaration");
                break;
            case Ident::STR:
                ident = new StringIdent(var_name);
                if (!TID.put(ident))
                    throw Exeption("variable: double declaration");
                break;
            case Ident::BOOL:
                ident = new BoolIdent(var_name);
                if (!TID.put(ident))
                    throw Exeption("variable: double declaration");
                break;
            default:
                throw LexExeption(curr_lex.get_type(), curr_lex.get_value()); // no need for this throw ???
                break;
        }
        
        get_lex();
        if (curr_lex_type == Lex::ASSIGN) {
            get_lex();
            if  (((var_type == Ident::INT) && (curr_lex_type == Lex::NUM)) ||
                ((var_type == Ident::STR) && (curr_lex_type == Lex::STRING))  ||
                ((var_type == Ident::BOOL) && ((curr_lex_type == Lex::_TRUE) || (curr_lex_type == Lex::_FALSE)))) {
                ident->put_value(curr_lex_value);
            }
            else
                throw Exeption("variable: const and ident type conflict");
                //lexer.put_lex(curr_lex);
        }
        else lexer.put_lex(curr_lex);
    }
    else throw LexExeption(curr_lex.get_type(), curr_lex.get_value());
}

/*
    <operators> -> { <_operator> }
    <_operator> ->  "if"    "("<expression>")" <_operator> [else <_operator>]   |
                    "while" "("<expression>")" <_operator>                      |
                    "read"  "("<identifier>")" ";"                              |
                    "write" "("<expression> { ","<expression> }")" ";"          |
                    <composite_operator> |
                    <expression_operator>
    <composite_operator>  -> "{"<_operator>"}"
    <expression_operator> -> <expression>";"
*/

void Parser::operators() {
    while(_operator()) {}
}

bool Parser::_operator() {
    int pl0, pl1, pl2, pl3;
    
    get_lex();
    switch (curr_lex_type) {

        // "if" "(" <expression> ")" <_operator> else <_operator>
        case Lex::IF:
            
            expect(Lex::OPEN_PAREN, "_operator IF: expected OPEN_PAREN");
            expression();
            expect(Lex::CLOSE_PAREN, "_operator IF: expected CLOSE_PAREN");
            
            pl2 = prog.get_free();
            prog.put_blank();
            prog.put_lex(Lex(Lex::POLIZ_FGO));
            
            _operator();
            
            pl3 = prog.get_free();
            prog.put_blank();
            prog.put_lex(Lex(Lex::POLIZ_GO));
            prog.put_lex(Lex(Lex::POLIZ_LABEL, prog.get_free()), pl2);
            
            get_lex();
            if (curr_lex_type == Lex::ELSE) {
                _operator();
                prog.put_lex(Lex(Lex::POLIZ_LABEL, prog.get_free()), pl3);
            }
            else
                lexer.put_lex(curr_lex);
            
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

 <expression> -> <e1> { "=" <e1> }
 <e1> -> <e2> {"or" <e2> }
 <e2> -> <e3> { "and" <e3> }
 <e3> -> <e4> { [ "<" | ">" | "!=" | "<=" | ">=" | "==" ] <e4> }
 <e4> -> <e5> { [ "+" | "-" ] <e5> }
 <e5> -> <e6> { [ "*" | "/" ] <e6> }
 <e6> -> <e7> { "not" <e7> }
 <e7> -> <identifier> | <number> | "true" | "false" | "("<expression>")"
 
*/

bool Parser::expression() {
    bool result = e1();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        Lex::type_of_lex curr_lex_type_tmp = curr_lex_type;
        if (curr_lex_type != Lex::ASSIGN) {
            lexer.put_lex(curr_lex);
            break;
        }
        //st_lex.push(curr_lex_type);
        e1();
        prog.put_lex(Lex(curr_lex_type_tmp));
        //check_op();
    }
    
    return true;
}

// <e1> -> <e2> | <e2> "or" <e2>
bool Parser::e1() {
    bool result = e2();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        Lex::type_of_lex curr_lex_type_tmp = curr_lex_type;
        if (curr_lex_type != Lex::OR) {
            lexer.put_lex(curr_lex);
            break;
        }
        //st_lex.push(curr_lex_type);
        e2();
        prog.put_lex(Lex(curr_lex_type_tmp));
        //check_op();
    }

    
    return true;
}

// <e2> -> <e3> | <e3> "and" <e3>
bool Parser::e2() {
    bool result = e3();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        Lex::type_of_lex curr_lex_type_tmp = curr_lex_type;
        if (curr_lex_type != Lex::AND) {
            lexer.put_lex(curr_lex);
            break;
        }
        //st_lex.push(curr_lex_type);
        e3();
        prog.put_lex(Lex(curr_lex_type_tmp));
        //check_op();
    }

    
    return true;
}

// <e3> -> <e4> | <e4> [ "<" | ">" | "!=" | "<=" | ">=" | "==" ] <e4>
bool Parser::e3() {
    bool result = e4();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        Lex::type_of_lex curr_lex_type_tmp = curr_lex_type;
        if (!((curr_lex_type == Lex::LESS) || (curr_lex_type == Lex::BIGGER) || (curr_lex_type == Lex::NOT_EQUAL) ||
            (curr_lex_type == Lex::LESS_EQUAL) || (curr_lex_type == Lex::BIGGER_EQUAL) || (curr_lex_type == Lex::EQUAL))) {
            lexer.put_lex(curr_lex);
            break;
        }
        //st_lex.push(curr_lex_type);
        e4();
        prog.put_lex(Lex(curr_lex_type_tmp));
        //check_op();
    }
    
    return true;
}

// <e4> -> <e5> | <e5> [ "+" | "-" ] <e5>
bool Parser::e4() {
    bool result = e5();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        Lex::type_of_lex curr_lex_type_tmp = curr_lex_type;
        if (!((curr_lex_type == Lex::PLUS) || (curr_lex_type == Lex::MINUS))) {
            lexer.put_lex(curr_lex);
            break;
        }
        //st_lex.push(curr_lex_type);
        e5();
        prog.put_lex(Lex(curr_lex_type_tmp));
        //check_op();
    }
    
    return true;
}

// <e5> -> <e6> | <e6> [ "*" | "/" ] <e6>
bool Parser::e5() {
    bool result = e6();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        Lex::type_of_lex curr_lex_type_tmp = curr_lex_type;
        if (!((curr_lex_type == Lex::MUL) || (curr_lex_type == Lex::DIV))) {
            lexer.put_lex(curr_lex);
            break;
        }
        //st_lex.push(curr_lex_type);
        e6();
        prog.put_lex(Lex(curr_lex_type_tmp));
        //check_op();
    }
    
    return true;
}

// <e6> -> <e7> | <e7> "not" <e7>
bool Parser::e6() {
    bool result = e7();
    if (!result)
        return result;
    
    while (true) {
        get_lex();
        Lex::type_of_lex curr_lex_type_tmp = curr_lex_type;
        if (curr_lex_type != Lex::NOT) {
            lexer.put_lex(curr_lex);
            break;
        }
        //st_lex.push(curr_lex_type);
        e7();
        prog.put_lex(Lex(curr_lex_type_tmp));
        //check_op();
    }
    
    return true;
}

// <e7> -> <identifier> | <number> | "true" | "false" | "("<expression>")"
bool Parser::e7() {
    
    get_lex();
    switch (curr_lex_type) {
        case Lex::IDENT: {
            int var_index = TID.index_of(curr_lex_value);
            if (var_index >= 0) {
                prog.put_lex(Lex(Lex::IDENT, TID.index_of(curr_lex_value), curr_lex_value));
            }
            else
                throw Exeption("e7: undeclared variable");
            break;
        }
            
        case Lex::NUM:
//            st_lex.push(Lex::INT);
//            prog.put_lex(Lex(curr_lex));
            curr_lex.set_int_value(atoi(curr_lex_value));
            prog.put_lex(curr_lex);
            break;
            
        case Lex::_TRUE:
//            st_lex.push(Lex::BOOLEAN);
//            prog.put_lex(Lex(curr_lex_type));
            break;
            
        case Lex::_FALSE:
            st_lex.push(Lex::BOOLEAN);
            prog.put_lex(Lex(curr_lex_type));
            break;
            
        case Lex::NOT:
            st_lex.push(Lex::NOT);
            prog.put_lex(Lex(curr_lex_type));
            break;
            
        case Lex::OPEN_PAREN:
            expression();
            expect(Lex::CLOSE_PAREN, "e7: expected CLOSE_PAREN");
            break;
            
        default:
            lexer.put_lex(curr_lex);
            return false;
    }
    
    return true;
}

void Parser::check_op() {
    Lex::type_of_lex type_1, type_2, operation, t = Lex::INT, r = Lex::BOOLEAN;
    
    
    type_2 = st_lex.top();
    st_lex.pop();
    operation = st_lex.top();
    st_lex.pop();
    type_1 = st_lex.top();
    st_lex.pop();

    // need to write if
    
//    if (type_1 == type_2 && type_1 == t)
//        st_lex.push(r);
    
    prog.put_lex(Lex(operation));
}


//======================================================================================================================================================

int main(int argc, const char * argv[]) {
    
//    Lex lex;
    Parser parser("program.txt");
    try {
        parser.analyze();
    }
    catch (Exeption exeption) {
        std::cerr << "unhandled error: " << exeption.get_message() << std::endl;
    }

    TID.print();
    
    //=============| Lexer Debug |=============
    Lexer debug_lexer("program.txt");
    Lex debug_lex;
    do {
        debug_lex = debug_lexer.get_lex();
        std::cout << debug_lex;
    } while (debug_lex.get_type() != Lex::FIN);
    std::cout << std::endl;
    //=========================================
    
    return 0;
}
