//
//  Tabl_Ident_Class.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 01.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include "Tabl_Ident.hpp"

Tabl_Ident::Tabl_Ident(int max_size) {
    size = max_size;
    pointer = new Ident * [size];
    len = 0;
}

Tabl_Ident::~Tabl_Ident() {
    delete []pointer;
}

Ident * Tabl_Ident::operator[](int k) const {
    return pointer[k];
}

bool Tabl_Ident::put(Ident * ident) {
    for (int i = 0; i < len; i++)
        if (!strcmp(ident->get_name(), pointer[i]->get_name()))
            return false;
    pointer[len] = ident;
    len++;
    return true;
}

void Tabl_Ident::print() {
    for (int i = 0; i < len; i++) {
        std::cout << pointer[i]->get_type() << " " <<  pointer[i]->get_name() << " ";
        pointer[i]->dump_str_value();
        std::cout << std::endl;
    }
}

int Tabl_Ident::index_of(const char * name) const {
    for (int i = 0; i < len; i++)
        if (!strcmp(name, pointer[i]->get_name()))
           return i;
    return -1;
}
