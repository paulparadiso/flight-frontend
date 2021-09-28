//
//  WeatherWind.h
//  flight
//
//  Created by Paul Paradiso on 8/17/16.
//
//

#ifndef __flight__WeatherWind__
#define __flight__WeatherWind__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

class WeatherWind : public WeatherType{
public:
    WeatherWind();
    void update();
    void update(string _data);
private:
};

#endif /* defined(__flight__WeatherWind__) */
