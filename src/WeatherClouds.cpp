//
//  WeatherClouds.cpp
//  flight
//
//  Created by Paul Paradiso on 3/15/16.
//
//

#include "WeatherClouds.h"

WeatherClouds::WeatherClouds(){
    
}

void WeatherClouds::update(string _data){
    ofxJSONElement data;
    //cout << _data << endl;
    data.parse(_data);
    //auto wind = data["data"]["metar"]["conditions"]["wind"];
    //bool bCloudsFound = false;
    //string report = data["data"]["metar"]["report"].asString();
    //cout << "Metar:" << report << endl;
    //string clouds = data["data"]["metar"]["cloud_type"].asString();
    /*
    for(auto vIter = visibleCloudTypes.begin(); vIter != visibleCloudTypes.end(); vIter++){
        int found = report.find(*vIter);
        if(found > -1){
            loadPreset(*vIter);
            bCloudsFound = true;
            break;
        } else {
            cout << (*vIter) << " not found in metar." << found << endl;
        }
    }
    */
    /*
    if(find(visibleCloudTypes.begin(), visibleCloudTypes.end(), clouds) != visibleCloudTypes.end()){
        loadPreset(clouds);
    } else {
        loadPreset("None");
        cout << "No cloud preset None." << endl;
    }
    */
    string clouds = data["data"]["processed"]["clouds"].asString();
    bool havePreset = false;
    for(auto cIter = visibleCloudTypes.begin(); cIter != visibleCloudTypes.end(); cIter++){
        if(*cIter  == clouds){
            havePreset = true;
        }
    }
    if(havePreset){
        loadPreset(clouds);
    } else {
        loadPreset("None");
    }
    float windMultiplier = -1;
    float knots = ofToFloat(data["data"]["processed"]["wind_speed"].asString()) / 500.0;
    //cout << "Setting wind speed to " << knots << endl;
    //float windSpeed = (knots / 100.0 * 2.0) * windMultiplier;
    float windSpeed = knots * windMultiplier;
    if(windSpeed > 2.0){
        windSpeed = 2.0;
    }
    if(windSpeed < -2.0){
        windSpeed = -2.0;
    }
    if(sysInfo != nullptr){
        sysInfo->setClouds(clouds);
    }
    cout << "Setting cloud speed to " << windSpeed << endl;
    uniforms["u_time_x_mult"].set(windSpeed);
}