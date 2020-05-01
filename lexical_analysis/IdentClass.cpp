//
//  IdentClass.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 01.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include <stdio.h>

#include "IdentClass.hpp"

Ident::Ident(const char * name) {
    strcpy(this->name, name);
}

char * Ident::get_name() { return name; }

char * StingIdent::get_value() { return value; }
int      IntIdent::get_value() { return value; }
bool    BoolIdent::get_value() { return value; }

void StingIdent::put_value(const char * value)  { strcpy(this->value, value); }
void   IntIdent::put_value(int value)           { this->value = value; }
void  BoolIdent::put_value(bool value)          { this->value = value; }
