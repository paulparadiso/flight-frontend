//
//  SunMoon.cpp
//  flight
//
//  Created by Paul Paradiso on 4/7/16.
//
//

#include "SunMoon.h"

SunMoon::SunMoon(){
    moon.load("animations/sun_moon/moon-01.png");
    sun.load("animations/sun_moon/sun.png");
    moonPos.set(ofGetWidth() / 3, ofGetHeight() / 2);
    sunPos.set((ofGetWidth() / 3) * 2, ofGetHeight() / 2);
    moonScale = 0.1;
    sunScale = 0.3;
}

void SunMoon::update(string _data){
    
}

void SunMoon::update(){
    /*
    Time now = sysInfo->getCurrentTime();
    Time rise = sysInfo->getSunrise();
    Time set = sysInfo->getSunset();
    */
    /*
    float pct = 0.0;
    if(sysInfo != nullptr){
        Time now = sysInfo->getCurrentTime();
        int totalMinutes = now.getHours() * 60;
        totalMinutes += now.getMinutes();
        pct = totalMinutes / float(24 * 60);
        //cout << "PCT = " << pct << endl;
    }
    */
    int daysSinceNewMoon = sysInfo->daysSinceNewMoon();
    moonChunk = int(20.0 * (float(daysSinceNewMoon) / 29.5));
    //cout << "Days since new moon = " << daysSinceNewMoon << ". Moon Chunk = " << moonChunk << endl;
    if(moonChunk > 19){
        moonChunk = 19;
    }
    if(moonChunk < 0){
        moonChunk = 0;
    }
    string part = sysInfo->getDayPart();
    float pct = sysInfo->getDayPercent();
    //cout << pct << " through the " << part << endl;
    if(part == "day"){
        bDaytime = true;
    } else {
        bDaytime = false;
    }
    int arcStartY = ofGetHeight() / 3;
    int arcEndY = ofGetHeight() / 5;
    int arcDiff = arcStartY - arcEndY;
    sunPos.set(ofGetWidth() * pct, arcStartY - (arcDiff * pct));
    moonPos.set(ofGetWidth() * pct, arcStartY - (arcDiff * pct));
    //cout << "arcStartY = " << arcStartY << ", arcEndY = " << arcEndY << ", arcDiff = " << arcDiff << ", moonPos = " << moonPos.x << ", " << moonPos.y << endl;
    /*
    if(bDaytime){
        ofVec2f sunStartPoint(ofGetWidth() / 2 - (ofGetHeight() * .7), ofGetHeight());
        sunPos = rotatePoint(ofGetWidth() / 2, ofGetHeight(), PI * pct, sunStartPoint);
    } else {
        ofVec2f moonStartPoint(ofGetWidth() / 2 - (ofGetHeight() * .7), ofGetHeight());
        moonPos = rotatePoint(ofGetWidth() / 2, ofGetHeight(), PI * pct, moonStartPoint);
    }
    */
}

void SunMoon::draw(){
    if(bDaytime){
        ofVec2f sunDrawPos;
        sunDrawPos.x = sunPos.x - (sun.getWidth() / 2) * sunScale;
        sunDrawPos.y = sunPos.y - (sun.getHeight() / 2) * sunScale;
        sun.draw(sunDrawPos.x, sunDrawPos.y, sun.getWidth() * sunScale, sun.getHeight() * sunScale);
        //cout << "Drawing sun at " << sunDrawPos.x << ", " << sunDrawPos.y << endl;
    } else {
        ofVec2f moonDrawPos;
        int subX = moonChunk % 4;
        int subY = moonChunk / 4;
        moonDrawPos.x = moonPos.x - (200 / 2) * moonScale;
        moonDrawPos.y = moonPos.y - (200 / 2) * moonScale;
        //cout << "Drawing moon subsection " << subX << ", " << subY << endl;
        //cout << "Drawing moon at " << moonDrawPos.x << ", " << moonDrawPos.y << endl;
        //moon.draw(moonDrawPos.x, moonDrawPos.y, moon.getWidth() * moonScale, moon.getHeight() * moonScale);
        moon.drawSubsection(moonDrawPos.x - 100 * moonScale, moonDrawPos.y - 100 * moonScale, 200, 200, subX * 200, subY * 200);
        //sun.draw(moonDrawPos.x, moonDrawPos.y, sun.getWidth() * sunScale, sun.getHeight() * sunScale);
    }
}

ofVec2f SunMoon::rotatePoint(float _cx, float _cy, float _a, ofVec2f _p){
    float s = sin(_a);
    float c = cos(_a);
    _p.x -= _cx;
    _p.y -= _cy;
    float xnew = _p.x * c - _p.y * s;
    float ynew = _p.x * s + _p.y * c;
    _p.x = xnew + _cx;
    _p.y = ynew + _cy;
    return _p;
}
