//
//  IdentClass.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 01.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef IdentClass_hpp
#define IdentClass_hpp

#include <stdio.h>

#include "LexClass.hpp"

#define buf_size 50

class Ident {
    
public:
    enum var_type {INT, BOOL, STR};

    Ident(const char * name);
    char *  get_name();
    virtual var_type get_type() = 0;

private:
    char name[buf_size];
};

class StingIdent : public Ident {
    
public:
    StingIdent(const char * name)
    : Ident(name) {}

    virtual var_type get_type() { return Ident::STR; }
    
    char * get_value();
    void   put_value(const char * value);
    
private:
    char value[buf_size];
};

class IntIdent : public Ident {
    
public:
    IntIdent(const char * name)
    : Ident(name) {}

    virtual var_type get_type() { return Ident::INT; }
    
    int      get_value();
    void     put_value(int value);
    
private:
    int value;
};

class BoolIdent : public Ident {
    
public:
    BoolIdent(const char * name)
    : Ident(name) {}

    virtual var_type get_type() { return Ident::BOOL; }
    
    bool    get_value();
    void    put_value(bool value);
    
private:
    bool value;
};

#endif /* IdentClass_hpp */
