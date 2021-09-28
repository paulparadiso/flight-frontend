//
//  WeatherLightPollution.hpp
//  flight
//
//  Created by Paul Paradiso on 12/4/17.
//
//

#ifndef WeatherLightPollution_hpp
#define WeatherLightPollution_hpp

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

class WeatherLightPollution: public WeatherType{
public:
    WeatherLightPollution();
    //void update();
    void update(){};
    void update(string _data);
    //void draw();
private:
    ofImage img;
    ofPixels pix;
    float maxDist = 200.0;
    int numCars = 200;
};

#endif /* WeatherLightPollution_hpp */
