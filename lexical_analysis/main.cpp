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
    void descriptions();
    void description();
    void type();
    void variable();
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

 <program>      ->  program "{" <descriptions> <operators> "}"
 <descriptions> ->  { description>";" }
 <description>  ->  <type> <variable> { ","<variable> }
 <type>         ->  int | string | boolean
 <variable>     ->  <identifier> | <identifier> "=" <constant>
 <constant>     ->  <integer> | <string> | <logical>
 <integer>      ->  [<sign>] <numeral> { <numeral> }
 <sign>         ->  "+" | "-"
 <string>       ->  """{ literal }"""
 <logical>      ->  "true" | "false"
 <operators>    ->  { <operator> }
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

void Parser::program() {
    
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


//const char * type_of_lex_readable[] = {
//    "NULL", "FIN",
//    "NUM", "IDENT", "STRING",
//    "PROGRAM", "WRITE", "READ", "AND", "OR", "NOT", "IF", "CASE", "OF", "END", "DO", "WHILE", "FOR", "UNTIL", "CONTINUE", "BREAK", "TRUE", "FALSE",
//    "INT", "REAL", "BOOLEAN", "STRING_TYPE", "GOTO",
//    "COLON", "SEMICOLON", "COMMA", "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACES", "CLOSE_BRACES", "PLUS", "MINUS", "MUL", "DIV", "BIGGER", "LESS", "ASSIGN", "BIGGER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "QOUTE"
//};

//---------------------------------| Lex |-------------------------------------
//class Lex {
//
//public:
//    enum type_of_lex {
//        _NULL, FIN,
//        NUM, IDENT, STRING,
//        PROGRAM, WRITE, READ, AND, OR, NOT, IF, CASE, OF, END, DO, WHILE, FOR, UNTIL, CONTINUE, BREAK, _TRUE, _FALSE,
//        INT, REAL, BOOLEAN, STRING_TYPE, GOTO,
//        COLON, SEMICOLON, COMMA, OPEN_PAREN, CLOSE_PAREN, OPEN_BRACES, CLOSE_BRACES, PLUS, MINUS, MUL, DIV, BIGGER, LESS, ASSIGN, BIGGER_EQUAL, LESS_EQUAL, EQUAL, NOT_EQUAL, QOUTE, L_COM, R_COM
//    };
//    Lex(type_of_lex type = _NULL, const char * value = NULL);
//    type_of_lex get_type();
//    char * get_value();
//    friend std::ostream & operator << (std::ostream & s, Lex lex);
//
//private:
//    type_of_lex lex_type;
//    char        lex_value[buf_size];
//};

//---------------------------------| Lex |-------------------------------------

//Lex::type_of_lex Lex::get_type() { return lex_type; }
//char * Lex::get_value() { return lex_value; }
//
//Lex::Lex(type_of_lex type, const char * value) {
//    lex_type = type;
//    if (value)
//        strcpy(lex_value, value);
//    else lex_value[0] = '\0';
//}
//
//std::ostream & operator << (std::ostream & s, Lex lex) {
//    s << "(" << type_of_lex_readable[lex.lex_type] << ", " << lex.lex_value << "), ";
//    return s;
//}

//-------------------------------| Lexer |--------------------------------------
//class Lexer {
//
//public:
//    Lexer(const char * program_file_path);
//    ~Lexer() { fclose(fp); }
//    Lex get_lex();
//
//private:
//    void  read_char();
//    void  clear();
//    int   look(const char * buf, const char ** list);
//    void  add_char();
//
//private:
//    FILE* fp;
//    char  c;
//    char  buf[buf_size];
//    int   buf_top;
//    enum  state {H, LITERAL, NUM, DELIM, STRING, COMMENT, ALE};
//    state CS;
//    static const char * Reserved_Table[];
//    static const char * Delim_Table[];
//
//};
//-------------------------------| Lexer |--------------------------------------

//void  Lexer::read_char() { c = fgetc(fp); }
//
//
//const char * Lexer::Reserved_Table[] = {"program", "write", "read", "and", "or", "not", "if", "case", "of", "end", "do", "while", "for", "until", "continue", "break", "true", "false", "int", "real", "boolean", "string", "goto", NULL}; // change when new Lex is added in type_of_lex!!!!!"
//const char * Lexer::Delim_Table[] = {":",";", ",", "(", ")", "{", "}", "+", "-", "*", "/", ">", "<", "=", ">=", "<=", "==", "!=", "\"", "#", NULL}; // change when new Lex is added in type_of_lex!!!!!
//
//Lexer::Lexer(const char * program_file_path) {
//    fp = fopen(program_file_path, "r");
//    if (!fp)
//        std::cout << "file not found!!!\n";
//    CS = H;
//    buf_top = 0;
//    read_char();
//}
//
//void  Lexer::add_char() {
//    if (buf_top >= buf_size)
//        throw -1;
//    buf[buf_top++] = c;
//}
//
//void Lexer::clear() {
//    buf_top = 0;
//    for (int i = 0; i < buf_size; i++)
//        buf[i] = '\0';
//}
//
//int Lexer::look(const char * buf, const char ** list) {
//    int i = 0;
//    while(list[i]) {
//        if (!strcmp(buf, list[i]))
//            return i;
//        i++;
//    }
//    return -1;
//}
//
//Lex Lexer::get_lex() {
//    CS = H;
//    int index;
//    while(true) {
//        if (feof(fp)) {
//            if (CS == STRING)
//                throw Lex::STRING;
//            if (CS == COMMENT)
//                throw -2;
//            return Lex(Lex::FIN);
//        }
//        switch (CS) { /* dfg */
//            case H:
//                if ((c == ' ') || (c == '\n') || (c == '\t') || (c == '\r')) {
//                    read_char();
//                    break;
//                }
//                else
//                if (isalpha(c)) {
//                    clear(); add_char(); read_char(); CS = LITERAL;
//                    break;
//                }
//                else
//                if (isdigit(c)) {
//                    clear(); add_char(); read_char(); CS = NUM;
//                    break;
//                }
//                if ((c == '>') || (c == '<') || (c == '=') || (c == '!')) {
//                    clear(); add_char(); read_char(); CS = ALE;
//                    break;
//                }
//                if (c == '"') {
//                    clear(); read_char(); CS = STRING;
//                    break;
//                }
//                if (c == '#') {
//                    clear(); read_char(); CS = COMMENT;
//                    break;
//                }
//                else
//                    clear(); add_char(); read_char(); CS = DELIM;
//                break;
//
//            case LITERAL:
//                if (isalpha(c) || isdigit(c)) {
//                    add_char(); read_char();
//                }
//                else
//                if ((index = look(buf, Reserved_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::PROGRAM + index));
//                else return Lex(Lex::IDENT, buf);
//                break;
//
//            case NUM:
//                if (isdigit(c)) {
//                    add_char(); read_char();
//                }
//                if (isalpha(c)) {
//                    throw Lex::NUM;
//                }
//                else return Lex(Lex::NUM, buf);
//                break;
//
//            case ALE:
//                if (c == '=') {
//                    add_char();
//                    read_char();
//                }
//                if ((index = look(buf, Delim_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::COLON + index));
//                throw c;
//                break;
//
//            case DELIM:
//                if ((index = look(buf, Delim_Table)) >= 0) return Lex(Lex::type_of_lex(Lex::COLON + index));
//                throw c;
//                break;
//
//            case STRING:
//                if (c != '"') {
//                    add_char(); read_char();
//                }
//                else {
//                    read_char();
//                    return Lex(Lex::STRING, buf);
//                }
//                break;
//
//            case COMMENT:
//                if (c != '#') {
//                    add_char(); read_char();
//                }
//                else {
//                    read_char();
//                    CS = H;
//                }
//                break;
//
//            default:
//                break;
//        }
//    }
//}
