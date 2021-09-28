//
//  WeatherBackground.h
//  flight
//
//  Created by Paul Paradiso on 4/1/16.
//
//

#ifndef __flight__WeatherBackground__
#define __flight__WeatherBackground__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"
#include "SystemInfo.h"

class WeatherBackground : public WeatherType{
public:
    WeatherBackground();
    void update(string _update){}
    void update();
    
private:
    float dayStartR = 0.333;
    float dayStartG = 0.333;
    float dayStartB = 0.333;
    float dayEndR = 1;
    float dayEndG = 1;
    float dayEndB = 1;
    float nightStartR = 0.232;
    float nightStartG = 0.232;
    float nightStartB = 0.232;
    float nightEndR = 0;
    float nightEndG = 0;
    float nightEndB = 0;
};

#endif /* defined(__flight__WeatherBackground__) */
