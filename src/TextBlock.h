//
//  TextBlock.hpp
//  flight
//
//  Created by Paul Paradiso on 12/30/17.
//
//

#ifndef TextBlock_hpp
#define TextBlock_hpp

#include <stdio.h>
#include "ofMain.h"

#define TEXT_WIDTH 100.0

class TextBlock{
public:
    TextBlock();
    void setup(string _h, int _x, int _y);
    void setHeaderFont(string _f, int _s);
    void setBodyFont(string _f, int _s);
    void setDrawPos(float _x, float _y);
    void setHeaderColor(int _r, int _g, int _b, int _a = 255);
    void setHeaderColor(ofColor _c);
    void setBodyColor(int _r, int _g, int _b, int _a = 255);
    void setBodyColor(ofColor _c);
    void clearBody();
    void setLimit(int _l);
    void setHeaderString(string _s);
    void addBodyString(string _s, bool top=false);
    void addBlock(string _s, int _width);
    float getBodyHeight();
    ofVec2f getDrawPos(){return drawPos;}
    void draw();
private:
    ofTrueTypeFont headerFont;
    ofTrueTypeFont bodyFont;
    string header;
    vector<string> body;
    ofVec2f drawPos;
    ofColor headerColor;
    ofColor bodyColor;
    int limit = 5;
};

#endif /* TextBlock_hpp */
