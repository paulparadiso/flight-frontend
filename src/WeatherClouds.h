//
//  WeatherClouds.h
//  flight
//
//  Created by Paul Paradiso on 3/15/16.
//
//

#ifndef __flight__WeatherClouds__
#define __flight__WeatherClouds__

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

class WeatherClouds : public WeatherType{
public:
    WeatherClouds();
    void update(string _data);
    void update(){}
private:
    vector<string> visibleCloudTypes = {"OVC", "BKN", "SCT", "FEW"};
};

#endif /* defined(__flight__WeatherClouds__) */
