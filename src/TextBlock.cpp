//
//  TextBlock.cpp
//  flight
//
//  Created by Paul Paradiso on 12/30/17.
//
//

#include "TextBlock.h"

TextBlock::TextBlock(){
    float scale = ofGetHeight() / 1080.f;
    headerFont.load("animations/fonts/roboto/RobotoCondensed-Regular.ttf", 12 * scale);
    bodyFont.load("animations/fonts/roboto/RobotoCondensed-Regular.ttf", 14 * scale);
    headerColor.set(100,100,100 );
    bodyColor.set(0, 0, 0);
    //drawPos.set(20,20);
    //header = "AIRPORT";
    //body.push_back("JFK");
}

void TextBlock::setup(string _h, int _x, int _y){
    setHeaderString(_h);
    setDrawPos(_x, _y);
}

void TextBlock::setHeaderFont(string _f, int _s){
    headerFont.load(_f, _s);
}

void TextBlock::setBodyFont(string _f, int _s){
    float scale = ofGetHeight() / 1080.f;
    bodyFont.load(_f, float(_s) * scale);
}

void TextBlock::setDrawPos(float _x, float _y){
    drawPos.set(_x, _y);
}

void TextBlock::setHeaderColor(int _r, int _g, int _b, int _a){
    headerColor.set(_r, _g, _b, _a);
}

void TextBlock::setHeaderColor(ofColor _c){
    headerColor = _c;
}

void TextBlock::setBodyColor(int _r, int _g, int _b, int _a){
    bodyColor.set(_r, _g, _b, _a);
}

void TextBlock::setBodyColor(ofColor _c){
    bodyColor = _c;
}

void TextBlock::setHeaderString(string _s){
    header = _s;
}

void TextBlock::addBodyString(string _s, bool top){
    if(!top){
        if(body.size() < limit){
            body.push_back(_s);
        }
    } else {
        body.insert(body.begin(), _s);
        body.resize(limit);
    }
}

void TextBlock::clearBody(){
    body.clear();
}

void TextBlock::setLimit(int _l){
    limit = _l;
}

void TextBlock::addBlock(string _s, int _width){
    string tmp = "";
    auto tokens = ofSplitString(_s, " ");
    for(auto tIter = tokens.begin(); tIter != tokens.end(); tIter++){
        if(bodyFont.getStringBoundingBox(tmp + *tIter, 0, 0).width < _width){
            tmp += *tIter + " ";
        } else {
            body.push_back(tmp);
            tmp = *tIter + " ";
        }
    }
    body.push_back(tmp);
}

float TextBlock::getBodyHeight(){
    string bodyString = "";
    for(auto sIter = body.begin(); sIter != body.end(); sIter++){
        bodyString += *sIter + "\n";
    }
    return bodyFont.getStringBoundingBox(bodyString, 0, 0).getHeight();
}

void TextBlock::draw(){
    int yStart = drawPos.y;
    int textHeight = headerFont.getLineHeight();
    string bodyString = "";
    for(auto sIter = body.begin(); sIter != body.end(); sIter++){
        bodyString += *sIter + "\n";
    }
    ofSetColor(headerColor);
    headerFont.drawString(header, drawPos.x, drawPos.y);
    ofSetColor(bodyColor);
    bodyFont.drawString(bodyString, drawPos.x + 20, drawPos.y + textHeight);
    ofSetColor(255,255,255);
}
