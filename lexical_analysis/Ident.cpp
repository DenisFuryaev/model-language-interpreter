//
//  IdentClass.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 01.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <stdio.h>

#include "Ident.hpp"
#include <string>

Ident::Ident(const char * name) {
    strcpy(this->name, name);
}

StructIdent::StructIdent(const char * name, const char * s_name)
: Ident(name) {
    std::strcpy(struct_name, s_name);
}

char *  Ident::get_name() { return name; }

char *  StringIdent::get_value() { return value; }
int     IntIdent::get_value() { return value; }
int     LabelIdent::get_value() { return value; }
bool    BoolIdent::get_value() { return value; }

void    StringIdent::set_value(const char * value) { strcpy(this->value, value); }
void    IntIdent::set_value(int value) { this->value = value; }
void    LabelIdent::set_value(int value) { this->value = value; }
void    BoolIdent::set_value(bool value) { this->value = value; }

void    StringIdent::put_value(const char * value) { strcpy(this->value, value); }
void    IntIdent::put_value(const char * value) { this->value = atoi(value); }
void    LabelIdent::put_value(const char * value) { this->value = atoi(value); }
void    BoolIdent::put_value(const char * value) {
    if (!strcmp(value, "true"))
        this->value = true;
    else
        this->value = false;
}

void    StringIdent::dump_str_value() { std::cout << value; }
void    IntIdent::dump_str_value() { std::cout << value; }
void    LabelIdent::dump_str_value() { std::cout << value; }
void    BoolIdent::dump_str_value() {
    if (value)
        std::cout << "true";
    else
        std::cout << "false";
}
