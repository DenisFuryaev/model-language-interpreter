//
//  Parser.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 07.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include "Tabl_Ident.hpp"
#include "Poliz.hpp"
#include "Lexer.hpp"

#include <stack>
#include <map>
#include <list>
#include <string>

class Parser {
    
public:
    typedef std::map <std::string, Ident::var_type> VarMap;
    typedef std::map <std::string, VarMap> StructMap;
    
    Parser(const char * program_file_path): lexer(program_file_path), prog(1000), TID(100) {}
    void analyze();
    
public:
    Poliz prog;
    Tabl_Ident TID;
    StructMap struct_map;
    
private:
    Lexer lexer;
    
    Lex curr_lex;
    Lex::type_of_lex curr_lex_type;
    const char * curr_lex_value;

private:
    
    void get_lex();
    void expect(Lex::type_of_lex lex_type, const char * error_message);
    void check_op();
    
    void program();
    void declarations();
    bool struct_type_declaration();
    bool struct_declaration(const char * struct_name);
    
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
    
};


#endif /* Parser_hpp */
