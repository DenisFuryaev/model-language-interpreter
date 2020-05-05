//
//  Tabl_Ident_Class.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 01.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include "Tabl_Ident_Class.hpp"

Tabl_Ident::Tabl_Ident(int max_size) {
    size = max_size;
    pointer = new Ident * [size];
    top = 1;
}

Tabl_Ident::~Tabl_Ident() {
    delete []pointer;
}

Ident * Tabl_Ident::operator[](int k) {
    return pointer[k];
}

int Tabl_Ident::put(Ident * ident) {
    for (int i = 1; i < top; i++)
        if (!strcmp(ident->get_name(), pointer[i]->get_name()))
            return 0;
    pointer[top] = ident;
    top++;
    return top - 1;
}

void Tabl_Ident::print() {
    for (int i = 1; i < top; i++) {
        std::cout << pointer[i]->get_type() << " " <<  pointer[i]->get_name() << " ";
        pointer[i]->dump_str_value();
        std::cout << std::endl;
    }
}
