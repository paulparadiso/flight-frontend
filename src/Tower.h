//
//  Tower.h
//  flight
//
//  Created by Paul Paradiso on 4/8/16.
//
//

#ifndef __flight__Tower__
#define __flight__Tower__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"
#include "ofxJSON.h"
#include "AnimatedObject.h"

class Tower : public WeatherType{
public:
    Tower();
    ~Tower(){}
    void update();
    void update(string _data);
    void draw();
    void setAirport(string _a);
private:
    ofVec2f towerPos;
    ofImage towerImg;
    AnimatedObject *ao = nullptr;
    float scale;
    bool bAnimated = false;
};

#endif /* defined(__flight__Tower__) */
