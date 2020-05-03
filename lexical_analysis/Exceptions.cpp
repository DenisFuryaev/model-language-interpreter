//
//  Exceptions.cpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 03.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#include "Exceptions.hpp"
#include <stdio.h>
#include <iostream>

Exeption::Exeption(const char * message) {
    strcpy(this->message, message);
}

const char * Exeption::get_message() {
    return message;
}
