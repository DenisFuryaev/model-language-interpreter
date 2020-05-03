//
//  Exceptions.hpp
//  lexical_analysis
//
//  Created by Denis Furyaev on 03.05.2020.
//  Copyright © 2020 Denis Furyaev. All rights reserved.
//

#ifndef Exceptions_hpp
#define Exceptions_hpp

#include <stdio.h>

class Exeption {
public:
    Exeption(const char * message = nullptr);
    const char * get_message();
    
private:
    char message[100];
    
};

#endif /* Exceptions_hpp */
