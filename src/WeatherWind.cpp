//
//  WeatherWind.cpp
//  flight
//
//  Created by Paul Paradiso on 8/17/16.
//
//

#include "WeatherWind.h"

WeatherWind::WeatherWind(){
}

void WeatherWind::update(){
}

void WeatherWind::update(string _data){
    ofxJSONElement data;
    data.parse(_data);
    //auto wind = data["data"]["metar"]["conditions"]["wind"];
    float windMultiplier = -0.0001;
    if(data["data"]["metar"]["wind_direction"].asInt() > 180){
        windMultiplier = 0.00001;
    }
    float knots = ofToFloat(data["data"]["metar"]["wind_speed"].asString());
    if(sysInfo != nullptr){
        sysInfo->setKnots(ofToString(knots));
    }
    float windSpeed = knots * windMultiplier;
    if(windSpeed > 2.0){
        windSpeed = 2.0;
    }
    if(windSpeed < -2.0){
        windSpeed = -2.0;
    }
    cout << "Setting Cloud speed to " << windSpeed << endl;
    uniforms["u_time_x_mult"].set(windSpeed);
}