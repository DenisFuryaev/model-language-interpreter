//
//  LexerClass.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 30.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>

#include "LexerClass.hpp"

void  Lexer::read_char() { c = fgetc(fp); }


const char * Lexer::Reserved_Table[] = {"program", "write", "read", "and", "or", "not", "if", "case", "of", "end", "do", "while", "for",
                                        "until", "continue", "break", "true", "false", "int", "real", "boolean", "string", "goto", NULL};           // change when new Lex is added in type_of_lex!!!!!"

const char * Lexer::Delim_Table[] = {":",";", ",", "(", ")", "{", "}", "+", "-", "*", "/", ">", "<", "=", ">=", "<=", "==", "!=", "\"", "#", NULL}; // change when new Lex is added in type_of_lex!!!!!

Lexer::Lexer(const char * program_file_path) {
    fp = fopen(program_file_path, "r");
    if (!fp)
        std::cout << "file not found!!!\n";
    CS = H;
    buf_top = 0;
    read_char();
}

void  Lexer::add_char() {
    if (buf_top >= buf_size)
        throw -1;
    buf[buf_top++] = c;
}

void Lexer::clear() {
    buf_top = 0;
    for (int i = 0; i < buf_size; i++)
        buf[i] = '\0';
}

int Lexer::look(const char * buf, const char ** list) {
    int i = 0;
    while(list[i]) {
        if (!strcmp(buf, list[i]))
            return i;
        i++;
    }
    return -1;
}

Lex Lexer::get_lex() {
    CS = H;
    int index;
    while(true) {
        if (feof(fp)) {
            if (CS == STRING)
                throw Lex::STRING;
            if (CS == COMMENT)
                throw -2;
            return Lex(Lex::FIN);
        }
        switch (CS) { 
            case H:
                if ((c == ' ') || (c == '\n') || (c == '\t') || (c == '\r')) {
                    read_char();
                    break;
                }
                else
                if (isalpha(c)) {
                    clear(); add_char(); read_char(); CS = LITERAL;
                    break;
                }
                else
                if (isdigit(c)) {
                    clear(); add_char(); read_char(); CS = NUM;
                    break;
                }
                if ((c == '>') || (c == '<') || (c == '=') || (c == '!')) {
                    clear(); add_char(); read_char(); CS = ALE;
                    break;
                }
                if (c == '"') {
                    clear(); read_char(); CS = STRING;
                    break;
                }
                if (c == '#') {
                    clear(); read_char(); CS = COMMENT;
                    break;
                }
                else
                    clear(); add_char(); read_char(); CS = DELIM;
                break;
                
            case LITERAL:
                if (isalpha(c) || isdigit(c)) {
                    add_char(); read_char();
                }
                else
                if ((index = look(buf, Reserved_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::PROGRAM + index));
                else return Lex(Lex::IDENT, buf);
                break;
            
            case NUM:
                if (isdigit(c)) {
                    add_char(); read_char();
                }
                if (isalpha(c)) {
                    throw Lex::NUM;
                }
                else return Lex(Lex::NUM, buf);
                break;
                
            case ALE:
                if (c == '=') {
                    add_char();
                    read_char();
                }
                if ((index = look(buf, Delim_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::COLON + index));
                throw c;
                break;
            
            case DELIM:
                if ((index = look(buf, Delim_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::COLON + index));
                throw c;
                break;
                
            case STRING:
                if (c != '"') {
                    add_char(); read_char();
                }
                else {
                    read_char();
                    return Lex(Lex::STRING, buf);
                }
                break;
                
            case COMMENT:
                if (c != '#') {
                    add_char(); read_char();
                }
                else {
                    read_char();
                    CS = H;
                }
                break;
                
            default:
                break;
        }
    }
}
