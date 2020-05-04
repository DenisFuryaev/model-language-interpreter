//
//  LexClass.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 30.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef LexClass_hpp
#define LexClass_hpp

#include <stdio.h>
#include <iostream>

#define buf_size 50


class Lex {
    
public:
    enum type_of_lex {
        _NULL, FIN,
        NUM, IDENT, STRING,
        PROGRAM, WRITE, READ, AND, OR, NOT, IF, ELSE, CASE, OF, END, DO, WHILE, FOR, UNTIL, CONTINUE, BREAK, _TRUE, _FALSE,
        INT, REAL, BOOLEAN, STRING_TYPE, GOTO,
        COLON, SEMICOLON, COMMA, OPEN_PAREN, CLOSE_PAREN, OPEN_BRACES, CLOSE_BRACES, PLUS, MINUS, MUL, DIV, BIGGER, LESS, ASSIGN, BIGGER_EQUAL, LESS_EQUAL, EQUAL, NOT_EQUAL, QOUTE, L_COM, R_COM
    };
    Lex(type_of_lex type = _NULL, const char * value = NULL);
    type_of_lex get_type();
    char * get_value();
    friend std::ostream & operator << (std::ostream & s, Lex lex);
    
private:
    type_of_lex lex_type;
    char        lex_value[buf_size];
};

#endif /* LexClass_hpp */
