//
//  WeatherStars.h
//  flight
//
//  Created by Paul Paradiso on 8/17/16.
//
//

#ifndef __flight__WeatherStars__
#define __flight__WeatherStars__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

class WeatherStars : public WeatherType{
public:
    WeatherStars();
    void update();
    void update(string _data){}
    void draw();
private:
    ofImage stars;
};

#endif /* defined(__flight__WeatherStars__) */
