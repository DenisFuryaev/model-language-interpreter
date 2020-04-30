//
//  LexClass.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 30.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include "LexClass.hpp"

const char * type_of_lex_readable[] = {
    "NULL", "FIN",
    "NUM", "IDENT", "STRING",
    "PROGRAM", "WRITE", "READ", "AND", "OR", "NOT", "IF", "CASE", "OF", "END", "DO", "WHILE", "FOR", "UNTIL", "CONTINUE", "BREAK", "TRUE", "FALSE",
    "INT", "REAL", "BOOLEAN", "STRING_TYPE", "GOTO",
    "COLON", "SEMICOLON", "COMMA", "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACES", "CLOSE_BRACES", "PLUS", "MINUS", "MUL", "DIV", "BIGGER", "LESS", "ASSIGN", "BIGGER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "QOUTE"
};

Lex::type_of_lex Lex::get_type() {
    return lex_type;
}

char * Lex::get_value() {
    return lex_value;
}

Lex::Lex(type_of_lex type, const char * value) {
    lex_type = type;
    if (value)
        strcpy(lex_value, value);
    else lex_value[0] = '\0';
}

std::ostream & operator << (std::ostream & s, Lex lex) {
    s << "(" << type_of_lex_readable[lex.lex_type] << ", " << lex.lex_value << "), ";
    return s;
}
