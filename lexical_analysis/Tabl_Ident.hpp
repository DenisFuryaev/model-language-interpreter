//
//  Tabl_Ident_Class.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 01.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef Tabl_Ident_Class_hpp
#define Tabl_Ident_Class_hpp

#include <stdio.h>

#include "Ident.hpp"

class Tabl_Ident {
    
public:
    Tabl_Ident(int max_size);
    ~Tabl_Ident();
    Ident * operator[](int k) const;
    bool put(Ident * ident);
    void print();
    int index_of(const char * name) const;
    
private:
    Ident ** pointer;
    int size;
    int len;
    
};

#endif /* Tabl_Ident_Class_hpp */
