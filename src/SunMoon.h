//
//  SunMoon.h
//  flight
//
//  Created by Paul Paradiso on 4/7/16.
//
//

#ifndef __flight__SunMoon__
#define __flight__SunMoon__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"
#include "SystemInfo.h"

class SunMoon : public WeatherType{
public:
    SunMoon();
    void update();
    void update(string _data);
    void draw();
private:
    ofImage sun;
    ofImage moon;
    ofVec2f moonPos;
    ofVec2f sunPos;
    ofVec2f rotatePoint(float _cx, float _cy, float _a, ofVec2f _p);
    float moonScale;
    float sunScale;
    bool bDaytime = true;
    int moonChunk = 0;
};

#endif /* defined(__flight__SunMoon__) */
