//
//  WeatherRain.cpp
//  flight
//
//  Created by Paul Paradiso on 3/14/16.
//
//

#include "WeatherRain.h"

WeatherRain::WeatherRain(){
    
}

void WeatherRain::update(string _data){
    cout << "Rain received new data." << endl;
    ofxJSONElement data;
    data.parse(_data);
    int windMultiplier;
    float windSpeed;
    bool rainFound = false;
    //auto weatherConditions = data["data"]["metar"]["conditions"]["weatherConditions"];
    //string conditions = data["data"]["metar"]["conditions"].asString();
    string precipType = data["data"]["processed"]["precip"]["type"].asString();
    string precipLevel = data["data"]["processed"]["precip"]["level"].asString();
    bool showers = false;
    bool rain = false;
    /*
    string precipType = "Moderate";
    if(conditions.find("SH") != string::npos){
        showers = true;
        if(conditions.find("RA") != string::npos){
            rain = true;
            if(conditions.find("-") != string::npos){
                precipType = "Light";
            }
            if(conditions.find("+") != string::npos){
                precipType = "Heavy";
            }
            rainFound = true;
            loadPreset("Rain:" + precipType);
        }
    }
    */
    cout << "setting" << precipType << "to level " << precipType << endl;
    if(precipType != "" && precipLevel != ""){
        loadPreset(precipType + ":" + precipLevel);
        if(sysInfo != nullptr){
            sysInfo->setPrecipitation(precipLevel + " " + precipType);
        }
    } else {
        loadPreset("Rain:None");
        if(sysInfo != nullptr){
            sysInfo->setPrecipitation("");
        }
    }
    /* Figure out how to display rain now.
    for(auto cIter = weatherConditions.begin(); cIter != weatherConditions.end(); cIter++){
        if((*cIter)["phenomenon"] == "Rain"){
            string intensity = (*cIter)["intensity"].asString();
            string preset = "Rain:" + intensity;
            loadPreset(preset);
            rainFound = true;
        }
        if((*cIter)["phenomenon"] == "Snow"){
            string intensity = (*cIter)["intensity"].asString();
            string preset = "Rain:" + intensity;
            loadPreset(preset);
            rainFound = true;
        }
    }
    */
    //int wind =
}