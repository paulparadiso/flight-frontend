//
//  WeatherOzone.hpp
//  flight
//
//  Created by Paul Paradiso on 12/4/17.
//
//

#ifndef WeatherOzone_hpp
#define WeatherOzone_hpp

#include <stdio.h>
#include "ofMain.h"
#include "WeatherType.h"

#define DOWNSHIFT_MIN -0.47
#define DOWNSHIFT_MAX 0.77

#define AQI_MAX 300

class WeatherOzone: public WeatherType{
public:
    WeatherOzone();
    void update(){}
    void update(string _data);
private:
};
#endif /* WeatherOzone_hpp */
