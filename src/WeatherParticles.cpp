//
//  WeatherParticles.cpp
//  flight
//
//  Created by Paul Paradiso on 12/4/17.
//
//

#include "WeatherParticles.h"

WeatherParticles::WeatherParticles(){
    
}

void WeatherParticles::update(string _data){
    ofxJSONElement data;
    data.parse(_data);
    int pm25 = ofToInt(data["data"]["pm25"].asString());
    int pm10 = ofToInt(data["data"]["pm10"].asString());
    int significant = pm25;
    if(pm10 > significant){
        significant  = pm10;
    }
    float diff = DOWNSHIFT_MAX - DOWNSHIFT_MIN;
    float downshiftVal = DOWNSHIFT_MIN + diff * (AQI_MAX / (float)significant);
    uniforms["downshift"].set(downshiftVal);
}
