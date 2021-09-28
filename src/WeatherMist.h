//
//  WeatherMist.h
//  flight
//
//  Created by Paul Paradiso on 8/2/16.
//
//

#ifndef __flight__WeatherMist__
#define __flight__WeatherMist__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

class WeatherMist : public WeatherType{
public:
    WeatherMist();
    void update();
    void update(string _data);
private:
};
#endif /* defined(__flight__WeatherMist__) */
