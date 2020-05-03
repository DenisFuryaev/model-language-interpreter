//
//  Poliz.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 03.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef Poliz_hpp
#define Poliz_hpp

#include <stdio.h>
#include "LexClass.hpp"

class Poliz {
public:
    Poliz(int max_size);
    ~Poliz();
    void put_lex(Lex lex);
    void put_lex(Lex lex, int place);
    void blank();
    int get_free();
    Lex & operator[](int index);
    void print();
    
private:
    Lex * pointer;
    int size;
    int free;
    
};

#endif /* Poliz_hpp */
