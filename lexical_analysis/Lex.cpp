//
//  LexClass.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 30.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include "Lex.hpp"

const char * type_of_lex_readable[] = {
    "NULL", "FIN",
    "NUM", "IDENT", "STRING",
    "PROGRAM", "WRITE", "READ", "AND", "OR", "NOT", "IF", "ELSE", "CASE", "OF", "END", "DO", "WHILE", "FOR", "UNTIL", "CONTINUE", "BREAK", "TRUE", "FALSE",
    "INT", "REAL", "BOOLEAN", "STRING_TYPE", "GOTO", "STRUCT",
    "COLON", "SEMICOLON", "COMMA", "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACES", "CLOSE_BRACES", "PLUS", "MINUS", "NEGATIVE", "MUL", "DIV", "BIGGER", "LESS", "ASSIGN", "BIGGER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "QOUTE", "COMMENT",
    "POLIZ_LABEL", "POLIZ_GO", "POLIZ_FGO"
    
};

Lex::type_of_lex Lex::get_type() {
    return lex_type;
}

void Lex::set_int_value(int int_value) {
    this->int_value = int_value;
}

Lex::Lex(type_of_lex type, int value, const char * str_value) {
    lex_type = type;
    int_value = value;
    if (str_value)
        strcpy(lex_value, str_value);
    else
        lex_value[0] = '\0';
}

char * Lex::get_str_value() {
    return lex_value;
}

int Lex::get_int_value() {
    return int_value;
}

Lex::Lex(type_of_lex type, const char * value) {
    int_value = -1;
    lex_type = type;
    if (value)
        strcpy(lex_value, value);
    else
        lex_value[0] = '\0';
}

std::ostream & operator << (std::ostream & s, Lex lex) {
    s << "(" << type_of_lex_readable[lex.lex_type];
    if (lex.lex_value[0] != '\0')
        s << ", " << lex.lex_value;
    if (lex.int_value >= 0)
        s << ", " << lex.int_value;
    s << ") ";
    return s;
}
