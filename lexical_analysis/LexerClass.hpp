//
//  LexerClass.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 30.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef LexerClass_hpp
#define LexerClass_hpp

#include <stdio.h>
#include <iostream>

#include "LexClass.hpp"
#include "Exceptions.hpp"

#define buf_size 50

class LexExeption : public Exeption {
public:
    LexExeption(Lex::type_of_lex lex_type, const char * message = nullptr);
    
private:
    const char * message;
    Lex::type_of_lex lex_type;
};

class Lexer {
    
public:
    Lexer(const char * program_file_path);
    ~Lexer() { fclose(fp); }
    Lex  get_lex();
    void put_lex(Lex lex);
    
private:
    void  read_char();
    void  clear();
    int   look(const char * buf, const char ** list);
    void  add_char();
    
    
private:
    FILE* fp;
    char  c;
    char  buf[buf_size];
    int   buf_top;
    enum  state {H, LITERAL, NUM, DELIM, STRING, COMMENT, ALE};
    state CS;
    Lex   pocket;
    static const char * Reserved_Table[];
    static const char * Delim_Table[];
    
};

#endif /* LexerClass_hpp */
