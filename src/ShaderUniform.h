//
//  ShaderUniform.h
//  flight
//
//  Created by Paul Paradiso on 3/14/16.
//
//

#ifndef __flight__ShaderUniform__
#define __flight__ShaderUniform__

#include <stdio.h>
#include "ofMain.h"

class ShaderUniform{
public:
    
    /*
     *
     * Constructor with variadic arguments. Type and number of
     * arguments should agree with uniform type.
     *
     */
    
    ShaderUniform();
    ShaderUniform(string _type);
    ShaderUniform(string _type, );
    void set(initializer_list<float> _l);
    void * get();
    void update(ofShader & _shader);
    
private:
    string type = nullptr;
    void * data;
};

#endif /* defined(__flight__ShaderUniform__) */
