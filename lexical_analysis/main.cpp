//
//  main.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 19.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>

#define buf_size 50


const char * type_of_lex_readable[] = {
    "NULL", "FIN",
    "NUM", "IDENT", "STRING",
    "PROGRAM", "WRITE", "READ", "AND", "OR", "NOT", "IF", "CASE", "OF", "END", "DO", "WHILE", "FOR", "UNTIL", "CONTINUE", "BREAK", "TRUE", "FALSE",
    "INT", "REAL", "BOOLEAN", "STRING_TYPE", "GOTO",
    "COLON", "SEMICOLON", "COMMA", "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACES", "CLOSE_BRACES", "PLUS", "MINUS", "MUL", "DIV", "BIGGER", "LESS", "ASSIGN", "BIGGER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "QOUTE"
};

//---------------------------------| Lex |-------------------------------------
class Lex {
    
public:
    enum type_of_lex {
        _NULL, FIN,
        NUM, IDENT, STRING,
        PROGRAM, WRITE, READ, AND, OR, NOT, IF, CASE, OF, END, DO, WHILE, FOR, UNTIL, CONTINUE, BREAK, _TRUE, _FALSE,
        INT, REAL, BOOLEAN, STRING_TYPE, GOTO,
        COLON, SEMICOLON, COMMA, OPEN_PAREN, CLOSE_PAREN, OPEN_BRACES, CLOSE_BRACES, PLUS, MINUS, MUL, DIV, BIGGER, LESS, ASSIGN, BIGGER_EQUAL, LESS_EQUAL, EQUAL, NOT_EQUAL, QOUTE, L_COM, R_COM
    };
    Lex(type_of_lex type = _NULL, const char * value = NULL);
    type_of_lex get_type() { return lex_type; }
    char * get_value() { return lex_value; }
    friend std::ostream & operator << (std::ostream & s, Lex lex);
    
private:
    type_of_lex lex_type;
    char        lex_value[buf_size];
};

//---------------------------------| Lex |-------------------------------------

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

//-------------------------------| Lexer |--------------------------------------
class Lexer {
    
public:
    Lexer(const char * program_file_path);
    ~Lexer() { fclose(fp); }
    Lex get_lex();
    
private:
    void  read_char() { c = fgetc(fp); }
    void  clear();
    int   look(const char * buf, const char ** list);
    void  add_char();
    
private:
    FILE* fp;
    char  c;
    char  buf[buf_size];
    int   buf_top;
    enum  state {H, LITERAL, NUM, DELIM, STRING, COMMENT};
    state CS;
    static const char * Reserved_Table[];
    static const char * Delim_Table[];
    
};
//-------------------------------| Lexer |--------------------------------------


const char * Lexer::Reserved_Table[] = {"program", "write", "read", "and", "or", "not", "if", "case", "of", "end", "do", "while", "for", "until", "continue", "break", "true", "false", "int", "real", "boolean", "string", "goto", NULL}; // change when new Lex is added in type_of_lex!!!!!"
const char * Lexer::Delim_Table[] = {":",";", ",", "(", ")", "{", "}", "+", "-", "*", "/", ">", "<", "=", ">=", "<=", "==", "!=", "\"", NULL}; // change when new Lex is added in type_of_lex!!!!!

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
        throw ;
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
                if (c == '"') {
                    clear(); read_char(); CS = STRING;
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
            
            case DELIM:
                if (c == '=') {
                    add_char();
                    read_char();
                }
                if ((index = look(buf, Delim_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::COLON + index));
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
                
            default:
                break;
        }
    }
}

//======================================================================================================================================================

int main(int argc, const char * argv[]) {
    
    Lexer lexer("program.txt");
    Lex lex;
    do {
        try {
            lex = lexer.get_lex();
        }
        catch (Lex::type_of_lex lex_type){
            std::cerr << "unhandled error in: " << type_of_lex_readable[lex_type] << std::endl;
            break;
        }
        std::cout << lex;
    }   while(lex.get_type() != Lex::FIN);
    std::cout << "\n";
    
    return 0;
}
