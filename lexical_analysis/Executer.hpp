//
//  Executer.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 09.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef Executer_hpp
#define Executer_hpp

#include <stdio.h>
#include "Poliz.hpp"
#include "LexClass.hpp"
#include "Tabl_Ident_Class.hpp"

#include <stack>

class Executer {
public:
    void execute(Poliz & prog);
    Executer(const Tabl_Ident * TID);
    
private:
    void to_int(Lex lex_arg_1, int & int_arg_1, Lex::type_of_lex & type_arg_1) const;
    Ident * find_ident(Lex lex) const;
    BoolIdent * find_bool_ident(Lex lex) const;
    IntIdent * find_int_ident(Lex lex) const;
    StringIdent * find_str_ident(Lex lex) const;
    
private:
    Lex curr_lex;
    const Tabl_Ident * TID;
};

#endif /* Executer_hpp */
