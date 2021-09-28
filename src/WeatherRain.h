//
//  WeatherRain.h
//  flight
//
//  Created by Paul Paradiso on 3/14/16.
//
//

#ifndef __flight__WeatherRain__
#define __flight__WeatherRain__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

class WeatherRain : public WeatherType{
public:
    WeatherRain();
    void update(){}
    void update(string _data);
private:
    
};

#endif /* defined(__flight__WeatherRain__) */
