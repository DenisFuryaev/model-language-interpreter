//
//  Executer.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 09.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include "Executer.hpp"
#include "Exceptions.hpp"
#include "Parser.hpp"

Executer::Executer(const Tabl_Ident * TID, Parser::StructMap & s_map)
 : struct_map(s_map) {
    this->TID = TID;
}

void Executer::to_int(Lex lex_arg_1, int & int_arg_1, Lex::type_of_lex & type_arg_1) const {
    switch (lex_arg_1.get_type()) {
        case Lex::IDENT: {
            Ident * ident = (*TID)[lex_arg_1.get_int_value()];
            
            switch (ident->get_type()) {
            
                case Ident::INT: {
                    IntIdent * int_ident = dynamic_cast <IntIdent*> (ident);
                    if (!int_ident)
                        throw Exeption("dynamic_cast eroor");
                    int_arg_1 = int_ident->get_value();
                    type_arg_1 = Lex::INT;
                    break;
                }
                    
                case Ident::STR: {
                    StringIdent * string_ident = dynamic_cast <StringIdent*> (ident);
                    if (!string_ident)
                        throw Exeption("dynamic_cast eroor");
                    int_arg_1 = (int)strlen(string_ident->get_value());
                    type_arg_1 = Lex::STRING;
                    break;
                }
                    
                default:
                    throw Exeption("wrong type of ident in comparison");
                    break;
            }
            break;
        }
            
        case Lex::STRING: {
            int_arg_1 = (int)strlen(lex_arg_1.get_str_value());
            type_arg_1 = Lex::STRING;
            break;
        }
            
        case Lex::NUM:
        case Lex::INT:
            int_arg_1 = lex_arg_1.get_int_value();
            type_arg_1 = Lex::INT;
            break;
            
        default:
            throw Exeption("wrong type of const in comparison");
            break;
    }
}

Ident * Executer::find_ident(Lex lex) const {
    int index = TID->index_of(lex.get_str_value());
    if (index < 0)
        throw Exeption("undeclared ident");
    return (*TID)[index];
}

BoolIdent* Executer::find_bool_ident(Lex lex) const {
    Ident * ident = find_ident(lex);
    if (ident->get_type() != Ident::BOOL)
        throw Exeption("ident type must be bool");
    
    BoolIdent * bool_ident = dynamic_cast<BoolIdent *>(ident);
    if (!bool_ident)
        throw Exeption("dynamic_cast eroor");
    
    return bool_ident;
}

IntIdent* Executer::find_int_ident(Lex lex) const {
    Ident * ident = find_ident(lex);
    if (ident->get_type() != Ident::INT)
        throw Exeption("ident type must be int");
    
    IntIdent * int_ident = dynamic_cast<IntIdent *>(ident);
    if (!int_ident)
        throw Exeption("dynamic_cast eroor");
    
    return int_ident;
}

StringIdent* Executer::find_str_ident(Lex lex) const {
    Ident * ident = find_ident(lex);
    if (ident->get_type() != Ident::STR)
        throw Exeption("ident type must be str");
    
    StringIdent * string_ident = dynamic_cast<StringIdent *>(ident);
    if (!string_ident)
        throw Exeption("dynamic_cast eroor");
    
    return string_ident;
}

void Executer::execute(Poliz & prog) {
    std::stack<Lex> args;
    int index = 0, size = prog.get_free();
    Lex lex_arg_1, lex_arg_2;
    int int_arg_1, int_arg_2;
    Lex::type_of_lex type_arg_1, type_arg_2;
    
    while (index < size) {
        
        curr_lex = prog[index];
        switch (curr_lex.get_type()) {
            case Lex::_TRUE:
            case Lex::_FALSE:
            case Lex::NUM:
            case Lex::STRING:
            case Lex::POLIZ_LABEL:
                args.push(curr_lex);
                break;
                
            case Lex::AND: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                bool bool_arg_1;
                
                switch (lex_arg_1.get_type()) {
                    case Lex::IDENT:
                        bool_arg_1 = find_bool_ident(lex_arg_1)->get_value();
                        break;
                        
                    case Lex::_TRUE:
                    case Lex::_FALSE:
                        bool_arg_1 = lex_arg_1.get_int_value();
                        break;
                        
                    default:
                        throw Exeption("AND: arg type error");
                        break;
                }
                
                switch (lex_arg_2.get_type()) {
                    case Lex::IDENT:
                        if (bool_arg_1 && find_bool_ident(lex_arg_2)->get_value())
                            args.push(Lex(Lex::_TRUE, 1));
                        else
                            args.push(Lex(Lex::_FALSE, 0));
                        break;
                        
                    case Lex::_TRUE:
                    case Lex::_FALSE:
                        if (bool_arg_1 && lex_arg_2.get_int_value())
                            args.push(Lex(Lex::_TRUE, 1));
                        else
                            args.push(Lex(Lex::_FALSE, 0));
                        break;
                        
                    default:
                        throw Exeption("AND: arg type error");
                        break;
                }
                
                break;
            }
                
            case Lex::OR: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                bool bool_arg_1;
                
                switch (lex_arg_1.get_type()) {
                    case Lex::IDENT:
                        bool_arg_1 = find_bool_ident(lex_arg_1)->get_value();
                        break;
                        
                    case Lex::_TRUE:
                    case Lex::_FALSE:
                        bool_arg_1 = lex_arg_1.get_int_value();
                        break;
                        
                    default:
                        throw Exeption("OR: arg type error");
                        break;
                }
                
                switch (lex_arg_2.get_type()) {
                    case Lex::IDENT: {
                        if (bool_arg_1 || find_bool_ident(lex_arg_2)->get_value())
                            args.push(Lex(Lex::_TRUE, 1));
                        else
                            args.push(Lex(Lex::_FALSE, 0));
                        break;
                    }
                        
                    case Lex::_TRUE:
                    case Lex::_FALSE:
                        if (bool_arg_1 || lex_arg_2.get_int_value())
                            args.push(Lex(Lex::_TRUE, 1));
                        else
                            args.push(Lex(Lex::_FALSE, 0));
                        break;
                        
                    default:
                        throw Exeption("OR: arg type error");
                        break;
                }
                
                break;
            }
            
            case Lex::NOT:
                lex_arg_1 = args.top();
                args.pop();
                
                switch (lex_arg_1.get_type()) {
                        
                    case Lex::IDENT:
                        if (!find_bool_ident(lex_arg_1)->get_value())
                            args.push(Lex(Lex::_TRUE, 1));
                        else
                            args.push(Lex(Lex::_FALSE, 0));
                        break;
                        
                    case Lex::_TRUE:
                        args.push(Lex(Lex::_FALSE, 0));
                        break;
                        
                    case Lex::_FALSE:
                        args.push(Lex(Lex::_TRUE, 1));
                        break;
                        
                    default:
                        throw Exeption("NOT: arg type error in NOT");
                        break;
                }
            
            break;
            
            case Lex::IDENT: {
                args.push(curr_lex);
                break;
            }
                
            case Lex::POLIZ_GO: {
                Lex arg_lex = args.top();
                
                if (arg_lex.get_type() == Lex::IDENT) {
                    Ident * curr_ident = find_ident(arg_lex);
                    if (curr_ident->get_type() == Ident::LABEL) {
                        LabelIdent * label_ident = dynamic_cast<LabelIdent *>(curr_ident);
                        if (!label_ident)
                            throw Exeption("dynamic_cast eroor");
                        index = label_ident->get_value() - 1;
                    }
                    else
                        throw Exeption("POLIZ_GO: ident type must be label");
                }
                else
                    index = args.top().get_int_value() - 1;
                
                args.pop();
                break;
            }
                
            case Lex::POLIZ_FGO: {
                int jmp_index = args.top().get_int_value();
                args.pop();
 
                Lex jump_condition = args.top();
                
                switch (jump_condition.get_type()) {
                    case Lex::IDENT:
                        if (!find_bool_ident(jump_condition)->get_value())
                            index = jmp_index - 1;
                        break;
                        
                    case Lex::_TRUE:
                    case Lex::_FALSE:
                        if (!jump_condition.get_int_value())
                            index = jmp_index - 1;
                        break;
                        
                    default:
                        throw Exeption("POLIZ_FGO: condition must be BOOLEAN");
                        break;
                }
                args.pop();
                break;
            }
                
            case Lex::ASSIGN: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                const char * str_arg_1;
                
                
                switch (lex_arg_1.get_type()) {
                    case Lex::IDENT: {
                        Ident * ident = (*TID)[lex_arg_1.get_int_value()];
                        
                        switch (ident->get_type()) {
                        
                            case Ident::INT: {
                                IntIdent * int_ident = dynamic_cast <IntIdent*> (ident);
                                if (!int_ident)
                                    throw Exeption("dynamic_cast eroor");
                                int_arg_1 = int_ident->get_value();
                                type_arg_1 = Lex::INT;
                                break;
                            }
                                
                            case Ident::STR: {
                                StringIdent * string_ident = dynamic_cast <StringIdent*> (ident);
                                if (!string_ident)
                                    throw Exeption("dynamic_cast eroor");
                                str_arg_1 = string_ident->get_value();
                                type_arg_1 = Lex::STRING;
                                break;
                            }
                                
                            case Ident::BOOL: {
                                BoolIdent * bool_ident = dynamic_cast <BoolIdent*> (ident);
                                if (!bool_ident)
                                    throw Exeption("dynamic_cast eroor");
                                int_arg_1 = bool_ident->get_value();
                                type_arg_1 = Lex::BOOLEAN;
                                break;
                            }
                                
                            case Ident::STRUCT: {
                                break;
                            }
                                
                            default:
                                throw Exeption("wrong type of ident in assign");
                                break;
                        }
                        break;
                    }
                        
                    case Lex::STRING:
                        str_arg_1 = lex_arg_1.get_str_value();
                        type_arg_1 = Lex::STRING;
                        break;
                        
                    
                    case Lex::INT:
                    case Lex::NUM:
                        int_arg_1 = lex_arg_1.get_int_value();
                        type_arg_1 = Lex::INT;
                        break;
                    
                    case Lex::_TRUE:
                    case Lex::_FALSE:
                        int_arg_1 = lex_arg_1.get_int_value();
                        type_arg_1 = Lex::BOOLEAN;
                        break;
                        
                    default:
                        throw Exeption("wrong type of const in assign");
                        break;
                }
                
                
                
                if (lex_arg_2.get_type() != Lex::IDENT)
                    throw Exeption("first assign arg should be variable");
                
                Ident * ident = (*TID)[lex_arg_2.get_int_value()];
                        
                switch (ident->get_type()) {
                        
                    case Ident::INT: {
                        IntIdent * int_ident = dynamic_cast <IntIdent*> (ident);
                        if (!int_ident)
                            throw Exeption("dynamic_cast error");
                        if (type_arg_1 != Lex::INT)
                            throw Exeption("Assign types conflict");
                        int_ident->set_value(int_arg_1);
                        args.push(lex_arg_2);
                        break;
                        }
                                
                    case Ident::STR: {
                        StringIdent * string_ident = dynamic_cast <StringIdent*> (ident);
                        if (!string_ident)
                            throw Exeption("dynamic_cast error");
                        if (type_arg_1 != Lex::STRING)
                            throw Exeption("Assign types conflict");
                        string_ident->set_value(str_arg_1);
                        args.push(lex_arg_2);
                        break;
                    }
                                
                    case Ident::BOOL: {
                        BoolIdent * bool_ident = dynamic_cast <BoolIdent*> (ident);
                        if (!bool_ident)
                            throw Exeption("dynamic_cast error");
                        if (!((type_arg_1 == Lex::_TRUE) || (type_arg_1 == Lex::_TRUE) || (type_arg_1 == Lex::BOOLEAN)))
                            throw Exeption("Assign types conflict");
                        bool_ident->set_value(int_arg_1);
                        args.push(lex_arg_2);
                        break;
                    }

                    case Ident::STRUCT: {
                        StructIdent * struct_ident = dynamic_cast <StructIdent*> (ident);
                        if (!struct_ident)
                            throw Exeption("dynamic_cast eroor");
                        
                        
                        for (Parser::VarMap::iterator i = struct_map[struct_ident->get_struct_name()].begin(); i != struct_map[struct_ident->get_struct_name()].end(); i++) {
                            char name_2[20];
                            strcpy(name_2, struct_ident->get_name());
                            strcat(name_2, ".");
                            strcat(name_2, i->first.c_str());
                            
                            char name_1[20];
                            strcpy(name_1, lex_arg_1.get_str_value());
                            strcat(name_1, ".");
                            strcat(name_1, i->first.c_str());
                            
                            Ident * ident_1 = (*TID)[TID->index_of(name_1)];
                            Ident * ident_2 = (*TID)[TID->index_of(name_2)];

                            if (ident_1->get_type() != ident_2->get_type())
                                throw Exeption("struct: struct operand expected");
                            
                            switch (ident_1->get_type()) {
                                case Ident::INT: {
                                    IntIdent * int_ident_1 = dynamic_cast <IntIdent*> (ident_1);
                                    IntIdent * int_ident_2 = dynamic_cast <IntIdent*> (ident_2);
                                    int_ident_2->set_value(int_ident_1->get_value());
                                    break;
                                }
                                case Ident::STR: {
                                    StringIdent * str_ident_1 = dynamic_cast <StringIdent*> (ident_1);
                                    StringIdent * str_ident_2 = dynamic_cast <StringIdent*> (ident_2);
                                    str_ident_2->set_value(str_ident_1->get_value());
                                    break;
                                }
                                case Ident::BOOL: {
                                    BoolIdent * bool_ident_1 = dynamic_cast <BoolIdent*> (ident_1);
                                    BoolIdent * bool_ident_2 = dynamic_cast <BoolIdent*> (ident_2);
                                    bool_ident_2->set_value(bool_ident_1->get_value());
                                    break;
                                }
                                default:
                                    throw LexExeption(curr_lex.get_type(), curr_lex.get_str_value());
                                    break;
                            }
                        }
                        args.push(lex_arg_2);
                        break;
                    }
                                
                    default:
                        throw Exeption("wrong type of ident in assign");
                        break;
                }
                                    
                break;
            }
                
            case Lex::DIV: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (!((type_arg_1 == type_arg_2) && (type_arg_2 == Lex::INT)))
                    throw Exeption("args type conflict");
                
                if (!int_arg_1)
                    throw Exeption("division by zero");
                    
                int result = (int_arg_2 / int_arg_1);
                args.push(Lex(type_arg_1, result));
                
                break;
            }
                
            case Lex::MUL: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (!((type_arg_1 == type_arg_2) && (type_arg_2 == Lex::INT)))
                    throw Exeption("args type conflict");
                
                int result = (int_arg_2 * int_arg_1);
                args.push(Lex(type_arg_1, result));
                
                break;
            }
                
            case Lex::MINUS: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (!((type_arg_1 == type_arg_2) && (type_arg_2 == Lex::INT)))
                    throw Exeption("args type conflict");
                
                int result = (int_arg_2 - int_arg_1);
                args.push(Lex(type_arg_1, result));
                
                break;
            }
                
            case Lex::NEGATIVE: {
                lex_arg_1 = args.top();
                args.pop();
                
                switch (lex_arg_1.get_type()) {
                    case Lex::NUM:
                        args.push(Lex(lex_arg_1.get_type(), -atoi(lex_arg_1.get_str_value())));
                        break;

                    case Lex::IDENT:
                        args.push(Lex(Lex::NUM, -find_int_ident(lex_arg_1)->get_value()));
                        break;
    
                    default:
                        throw Exeption("NEGATIVE: ident or num must be");
                        break;
                }
                break;
            }
                 
            case Lex::PLUS: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                const char * str_arg_1;
                
                switch (lex_arg_1.get_type()) {
                    case Lex::IDENT: {
                        Ident * ident = (*TID)[lex_arg_1.get_int_value()];
                        
                        switch (ident->get_type()) {
                        
                            case Ident::INT: {
                                IntIdent * int_ident = dynamic_cast <IntIdent*> (ident);
                                if (!int_ident)
                                    throw Exeption("dynamic_cast eroor");
                                int_arg_1 = int_ident->get_value();
                                type_arg_1 = Lex::INT;
                                break;
                            }
                                
                            case Ident::STR: {
                                StringIdent * string_ident = dynamic_cast <StringIdent*> (ident);
                                if (!string_ident)
                                    throw Exeption("dynamic_cast eroor");
                                str_arg_1 = string_ident->get_value();
                                type_arg_1 = Lex::STRING;
                                break;
                            }
                                
                            default:
                                throw Exeption("wrong type of ident in PLUS");
                                break;
                        }
                        break;
                    }
                        
                    case Lex::STRING:
                        str_arg_1 = lex_arg_1.get_str_value();
                        type_arg_1 = Lex::STRING;
                        break;
                        
                    case Lex::INT:
                    case Lex::NUM:
                        int_arg_1 = lex_arg_1.get_int_value();
                        type_arg_1 = Lex::INT;
                        break;
                        
                    default:
                        throw Exeption("wrong type of const in PLUS");
                        break;
                }
                
                switch (lex_arg_2.get_type()) {
                    case Lex::IDENT: {
                        Ident * ident = (*TID)[lex_arg_2.get_int_value()];
                        
                        switch (ident->get_type()) {
                        
                            case Ident::INT: {
                                if ((type_arg_1 != Lex::INT) && (type_arg_1 != Lex::NUM))
                                    throw Exeption("args type conflict");
                                IntIdent * int_ident = dynamic_cast <IntIdent*> (ident);
                                if (!int_ident)
                                    throw Exeption("dynamic_cast eroor");
                                int_ident->set_value(int_ident->get_value() + int_arg_1);
                                args.push(lex_arg_2);
                                break;
                            }
                                
                            case Ident::STR: {
                                if (type_arg_1 != Lex::STRING)
                                    throw Exeption("args type conflict");
                                StringIdent * string_ident = dynamic_cast <StringIdent*> (ident);
                                if (!string_ident)
                                    throw Exeption("dynamic_cast eroor");
                                string_ident->put_value(strcat(string_ident->get_value(), str_arg_1));
                                args.push(lex_arg_2);
                                break;
                            }
                                
                            default:
                                throw Exeption("wrong type of ident in PLUS");
                                break;
                        }
                        break;
                    }
                        
                    case Lex::STRING:
                        if (type_arg_1 != Lex::STRING)
                            throw Exeption("args type conflict");
                        args.push(Lex(Lex::STRING, strcat(lex_arg_2.get_str_value(), str_arg_1)));
                        break;
                        
                    case Lex::INT:
                    case Lex::NUM:
                        if ((type_arg_1 != Lex::INT) && (type_arg_1 != Lex::NUM))
                            throw Exeption("args type conflict");
                        args.push(Lex(Lex::NUM, lex_arg_2.get_int_value() + int_arg_1));
                        break;
                        
                    default:
                        throw Exeption("wrong type of const in PLUS");
                        break;
                }
                    
                break;
            }
                
            case Lex::READ: {
                lex_arg_1 = args.top();
                args.pop();
                
                if (lex_arg_1.get_type() != Lex::IDENT)
                    throw Exeption("read op must be an ident");

                Ident * ident = (*TID)[lex_arg_1.get_int_value()];
                
                switch (ident->get_type()) {
                
                    case Ident::INT: {
                        IntIdent * int_ident = dynamic_cast <IntIdent*> (ident);
                        if (!int_ident)
                            throw Exeption("dynamic_cast eroor");
                        int num;
                        std::cin >> num;
                        int_ident->set_value(num);
                        break;
                    }
                        
                    case Ident::STR: {
                        StringIdent * string_ident = dynamic_cast <StringIdent*> (ident);
                        if (!string_ident)
                            throw Exeption("dynamic_cast eroor");
                        char str[20];
                        std::cin >> str;
                        string_ident->set_value(str);
                        break;
                    }
                        
                    default:
                        throw Exeption("wrong type of ident in READ");
                        break;
                }
                
                break;
            }
                
            case Lex::WRITE: {
                int arg_count = curr_lex.get_int_value();
                
                // stack for right order write (not back order)
                std::stack<Lex> temp_stack;
                while (arg_count > 0) {
                    lex_arg_1 = args.top();
                    args.pop();
                    temp_stack.push(lex_arg_1);
                    arg_count--;
                }
                
                arg_count = curr_lex.get_int_value();
                while (arg_count > 0) {
                    lex_arg_1 = temp_stack.top();
                    temp_stack.pop();
                    switch (lex_arg_1.get_type()) {
                        case Lex::IDENT: {
                            Ident * ident = (*TID)[lex_arg_1.get_int_value()];
                            
                            switch (ident->get_type()) {
                            
                                case Ident::INT: {
                                    IntIdent * int_ident = dynamic_cast <IntIdent*> (ident);
                                    if (!int_ident)
                                        throw Exeption("dynamic_cast eroor");
                                    std::cout << int_ident->get_value() << std::endl;
                                    break;
                                }
                                    
                                case Ident::STR: {
                                    StringIdent * string_ident = dynamic_cast <StringIdent*> (ident);
                                    if (!string_ident)
                                        throw Exeption("dynamic_cast eroor");
                                    std::cout << string_ident->get_value() << std::endl;
                                    break;
                                }
                                    
                                case Ident::BOOL: {
                                    BoolIdent * bool_ident = dynamic_cast <BoolIdent*> (ident);
                                    if (!bool_ident)
                                        throw Exeption("dynamic_cast eroor");
                                    
                                    if (bool_ident->get_value())
                                        std::cout << "true" << std::endl;
                                    else
                                        std::cout << "false" << std::endl;
                                    break;
                                }
                                    
                                default:
                                    throw Exeption("wrong type of ident in write");
                                    break;
                            }
                            break;
                        }
                            
                        case Lex::STRING:
                            std::cout << lex_arg_1.get_str_value() << std::endl;
                            break;
                            
                        case Lex::INT:
                        case Lex::NUM:
                            std::cout << lex_arg_1.get_int_value() << std::endl;
                            break;
                            
                        case Lex::_TRUE:
                            std::cout << "true" << std::endl;
                            break;
                            
                        case Lex::_FALSE:
                            std::cout << "false" << std::endl;
                            break;
                            
                        default:
                            throw Exeption("wrong type of const in write");
                            break;
                    }
                    arg_count--;
                }
                break;
            }
                
            case Lex::LESS: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (type_arg_1 != type_arg_2)
                    throw Exeption("args type conflict");
                
                int result = (int_arg_2 < int_arg_1);
                if (result)
                    args.push(Lex(Lex::_TRUE, 1));
                else
                    args.push(Lex(Lex::_FALSE, 0));
                
                break;
            }
            
            case Lex::BIGGER: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (type_arg_1 != type_arg_2)
                    throw Exeption("args type ");
                
                int result = (int_arg_2 > int_arg_1);
                if (result)
                    args.push(Lex(Lex::_TRUE, 1));
                else
                    args.push(Lex(Lex::_FALSE, 0));
                
                break;
            }
                
            case Lex::LESS_EQUAL: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (type_arg_1 != type_arg_2)
                    throw Exeption("args type ");
                
                int result = (int_arg_2 <= int_arg_1);
                if (result)
                    args.push(Lex(Lex::_TRUE, 1));
                else
                    args.push(Lex(Lex::_FALSE, 0));
                
                break;
            }
                
            case Lex::BIGGER_EQUAL: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (type_arg_1 != type_arg_2)
                    throw Exeption("args type ");
                
                int result = (int_arg_2 >= int_arg_1);
                if (result)
                    args.push(Lex(Lex::_TRUE, 1));
                else
                    args.push(Lex(Lex::_FALSE, 0));
                
                break;
            }
                
            case Lex::EQUAL: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (type_arg_1 != type_arg_2)
                    throw Exeption("args type ");
                
                int result = (int_arg_2 == int_arg_1);
                if (result)
                    args.push(Lex(Lex::_TRUE, 1));
                else
                    args.push(Lex(Lex::_FALSE, 0));
                
                break;
            }
                
            case Lex::NOT_EQUAL: {
                lex_arg_1 = args.top();
                args.pop();
                lex_arg_2 = args.top();
                args.pop();
                
                to_int(lex_arg_1, int_arg_1, type_arg_1);
                to_int(lex_arg_2, int_arg_2, type_arg_2);
                
                if (type_arg_1 != type_arg_2)
                    throw Exeption("args type ");
                
                int result = (int_arg_2 != int_arg_1);
                if (result)
                    args.push(Lex(Lex::_TRUE, 1));
                else
                    args.push(Lex(Lex::_FALSE, 0));
                break;
            }
                
            default:
                throw Exeption("POLIZ: unexpected lex");
                break;
        }
        index++;
        
    }
    std::cout << std::endl << "  Execution is Finished" << std::endl;
}
