//
//  WeatherOzone.cpp
//  flight
//
//  Created by Paul Paradiso on 12/4/17.
//
//

#include "WeatherOzone.h"

WeatherOzone::WeatherOzone(){
    
}

void WeatherOzone::update(string _data){
    ofxJSONElement data;
    data.parse(_data);
    int ozone = ofToInt(data["data"]["o3"].asString());
    float diff = DOWNSHIFT_MAX - DOWNSHIFT_MIN;
    float downshiftVal = DOWNSHIFT_MIN + diff * (AQI_MAX / (float)ozone);
    uniforms["downshift"].set(downshiftVal);
}
