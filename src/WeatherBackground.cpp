//
//  WeatherBackground.cpp
//  flight
//
//  Created by Paul Paradiso on 4/1/16.
//
//

#include "WeatherBackground.h"

WeatherBackground::WeatherBackground(){
    
}

void WeatherBackground::update(){
    string part = sysInfo->getDayPart();
    float pct = sysInfo->getDayPercent();
    float startROut = 0.0;
    float startGOut = 0.0;
    float startBOut = 0.0;
    float endROut = 0.0;
    float endGOut = 0.0;
    float endBOut = 0.0;
    float startRDiff = dayStartR - nightStartR;
    float startGDiff = dayStartG - nightStartG;
    float startBDiff = dayStartB - nightStartB;
    float endRDiff = dayEndR - nightEndR;
    float endGDiff = dayEndG - nightEndG;
    float endBDiff = dayEndB - nightEndB;
    /*
    float distance = 0.0;
    if(pct > 0.5){
        distance = pct - 0.5;
    } else {
        distance = pct + 0.5;
    }
    */
    float distance = 1.0;
    float nightDistance = 0.0;
    if(pct < 0.05){
        distance = 1.0 - ((0.05 + pct) / 0.1);
        nightDistance = distance;
    } else if(pct > 0.95){
        distance = (0.05 - (1.0 - pct)) / 0.1;
        nightDistance = distance;
    }
    //cout << "distance = " << distance << endl;
    if(part == "day"){
        startROut = dayStartR - (startRDiff * (1.0 - distance));
        startGOut = dayStartG - (startGDiff * (1.0 - distance));
        startBOut = dayStartB - (startBDiff * (1.0 - distance));
        endROut = dayEndR - (endRDiff * (1.0 - distance));
        endGOut = dayEndG - (endGDiff * (1.0 - distance));
        endBOut = dayEndB - (endBDiff * (1.0 - distance));
    } else {
        startROut = nightStartR + (startRDiff * nightDistance);
        startGOut = nightStartG + (startGDiff * nightDistance);
        startBOut = nightStartB + (startBDiff * nightDistance);
        endROut = nightEndR + (endRDiff  * nightDistance);
        endGOut = nightEndG + (endGDiff * nightDistance);
        endBOut = nightEndB + (endBDiff * nightDistance);
    }
    uniforms["u_start_r"].set(startROut);
    uniforms["u_start_g"].set(startGOut);
    uniforms["u_start_b"].set(startBOut);
    uniforms["u_end_r"].set(endROut);
    uniforms["u_end_g"].set(endGOut);
    uniforms["u_end_b"].set(endBOut);
}