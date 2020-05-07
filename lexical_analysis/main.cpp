//
//  main.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 19.04.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "LexClass.hpp"
#include "LexerClass.hpp"
#include "IdentClass.hpp"
#include "Tabl_Ident_Class.hpp"
#include "Exceptions.hpp"
#include "Poliz.hpp"
#include "Parser.hpp"

//======================================================================================================================================================

int main(int argc, const char * argv[]) {
    
    Parser parser("program.txt");
    try {
        parser.analyze();
    }
    catch (Exeption exeption) {
        std::cerr << "unhandled error: " << exeption.get_message() << std::endl;
    }

    //TID.print();
    
    //=============| Lexer Debug |=============
//    Lexer debug_lexer("program.txt");
//    Lex debug_lex;
//    do {
//        debug_lex = debug_lexer.get_lex();
//        std::cout << debug_lex;
//    } while (debug_lex.get_type() != Lex::FIN);
//    std::cout << std::endl;
    //=========================================
    
    return 0;
}
