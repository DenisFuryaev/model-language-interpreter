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

#include "Lex.hpp"

#define buf_size 50

class Ident {
    
public:
    enum var_type {INT, BOOL, STR, LABEL, STRUCT};

    Ident(const char * name);
    char *  get_name();
    virtual var_type get_type() = 0;
    virtual void put_value(const char * value) {};
    virtual void dump_str_value() {};

private:
    char name[buf_size];
};

class StringIdent : public Ident {
    
public:
    StringIdent(const char * name)
    : Ident(name) {}

    virtual var_type    get_type() { return Ident::STR; }
    char *              get_value();
    void                set_value(const char * value);
    virtual void        put_value(const char * value);
    virtual void        dump_str_value();

    
private:
    char value[buf_size];
};

class IntIdent : public Ident {
    
public:
    IntIdent(const char * name)
    : Ident(name) {}

    virtual var_type    get_type() { return Ident::INT; }
    int                 get_value();
    void                set_value(int value);
    virtual void        put_value(const char * value);
    virtual void        dump_str_value();
    
private:
    int value;
};

class BoolIdent : public Ident {
    
public:
    BoolIdent(const char * name)
    : Ident(name) {}

    virtual var_type    get_type() { return Ident::BOOL; }
    bool                get_value();
    void                set_value(bool value);
    virtual void        put_value(const char * value);
    virtual void        dump_str_value();
    
private:
    bool value;
};

class LabelIdent : public Ident {
    
public:
    LabelIdent(const char * name)
    : Ident(name) {}

    virtual var_type    get_type() { return Ident::LABEL; }
    int                 get_value();
    void                set_value(int value);
    virtual void        put_value(const char * value);
    virtual void        dump_str_value();
    
private:
    int value;
};

class StructIdent : public Ident {
    
public:
    StructIdent(const char * name,  const char * struct_name);

    virtual var_type    get_type() { return Ident::STRUCT; }
    const char * get_struct_name() { return struct_name; }
    
private:
    int value;
    char struct_name[20];
};

#endif /* IdentClass_hpp */
