//
//  Poliz.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 03.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include "Poliz.hpp"
#include "Exceptions.hpp"

Poliz::Poliz(int max_size){
    size = max_size;
    pointer = new Lex[max_size];
    free = 0;
}

Poliz::~Poliz(){
    delete []pointer;
}

void Poliz::put_lex(Lex lex) {
    pointer[free] = lex;
    free++;
}

void Poliz::put_lex(Lex lex, int place) {
    pointer[place] = lex;
}

void Poliz::put_blank() {
    free++;
}

int Poliz::get_free() {
    return free;
}

Lex & Poliz::operator[](int index) {
     if (index > size)
         throw Exeption("POLIZ: out of array");
    else
        if (index > free)
            throw Exeption("POLIZ: indefinite element of array");
        else
            return pointer[index];
}

void Poliz::print() {
    for (int i = 0; i < free; i++) {
        std::cout << "[" << i << "] ";
        std::cout << pointer[i] << std::endl;
    }
    std::cout << std::endl;
}
