//
//  ShaderUniform.cpp
//  flight
//
//  Created by Paul Paradiso on 3/14/16.
//
//

#include "ShaderUniform.h"

ShaderUniform::ShaderUniform(){
    
}

ShaderUniform::ShaderUniform(string _type){
   
}

ShaderUniform::ShaderUniform(string _type, initializer_list<float> _l){
    type = _type;
    set(_l);
}

void ShaderUniform::set(initializer_list<float> _l){
    switch (type) {
        case "1i":
            data = new int(_l[0]);
            break;
            
        default:
            break;
    }
}

void * ShaderUniform::get(){
    return data;
}