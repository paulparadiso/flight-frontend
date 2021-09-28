//
//  WeatherParticulate.h
//  flight
//
//  Created by Paul Paradiso on 3/15/16.
//
//

#ifndef __flight__WeatherParticulate__
#define __flight__WeatherParticulate__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

class WeatherParticulate : public WeatherType{
public:
    WeatherParticulate();
    void update(string _data);
    void update(){}
private:
};

#endif /* defined(__flight__WeatherParticulate__) */
