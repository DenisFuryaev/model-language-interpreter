//
//  LexerClass.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 30.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>

#include "Lexer.hpp"

LexExeption::LexExeption(Lex::type_of_lex lex_type, const char * message)
    : Exeption(message) {
    this->lex_type = lex_type;
}
Lex LexExeption::get_lex_type() {
    return this->lex_type;
}

void  Lexer::read_char() { c = fgetc(fp); }

const char * Lexer::Reserved_Table[] = {"program", "write", "read", "and", "or", "not", "if", "else", "case", "of", "end", "do", "while", "for",
                                        "until", "continue", "break", "true", "false", "int", "real", "boolean", "string", "goto", "struct", NULL};           // change when new Lex is added in type_of_lex!!!!!"

const char * Lexer::Delim_Table[] = {":",";", ",", "(", ")", "{", "}", "+", "-", "-", "*", "/", ">", "<", "=", ">=", "<=", "==", "!=", "\"", "#", NULL}; // change when new Lex is added in type_of_lex!!!!!

Lexer::Lexer(const char * program_file_path)
    : pocket_stack() {
        
    fp = fopen(program_file_path, "r");
    if (!fp)
        std::cout << "file not found!!!\n";
    CS = H;
    buf_top = 0;
    read_char();
}

void  Lexer::add_char() {
    if (buf_top >= buf_size)
        throw Exeption("buffer overflow");
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

void Lexer::put_lex(Lex lex) {
    pocket_stack.push(lex);
}

Lex Lexer::get_lex() {
    if (!pocket_stack.empty()) {
        Lex temp = pocket_stack.top();
        pocket_stack.pop();
        return temp;
    }
    CS = H;
    int index;
    while(true) {
        if (feof(fp)) {
            if ((CS == DELIM) && (!strcmp(buf, "}")))
                return Lex(Lex::type_of_lex(Lex::CLOSE_BRACES));
            if (CS == STRING)
                throw Exeption("string not closed");
            if (CS == COMMENT)
                throw Exeption("comment not closed");
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
                if (isalpha(c) || isdigit(c) || (c == '.')) {
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
                else
                if (isalpha(c)) {
                    throw LexExeption(Lex::NUM, buf);
                }
                else return Lex(Lex::NUM, buf);
                break;
                
            case ALE:
                if (c == '=') {
                    add_char();
                    read_char();
                }
                if ((index = look(buf, Delim_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::COLON + index));
                throw Exeption("double ident");
                break;
            
            case DELIM:
                if ((index = look(buf, Delim_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::COLON + index));
                throw Exeption("no such ident");
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
