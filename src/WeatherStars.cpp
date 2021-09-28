//
//  WeatherStars.cpp
//  flight
//
//  Created by Paul Paradiso on 8/17/16.
//
//

#include "WeatherStars.h"

WeatherStars::WeatherStars(){
    stars.load("animations/stars/stars.png");
}

void WeatherStars::update(){
    
}

void WeatherStars::draw(){
    float scale = 1080.0 / ofGetHeight();
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofScale(0.85,0.85,0.85);
    ofRotateZ(float(ofGetElapsedTimeMillis() % 360000) / 1000);
    ofTranslate(-(stars.getWidth() /2), -(stars.getHeight() / 2));
    ofSetColor(255,255,255,190);
    stars.draw(0, 0);
    ofSetColor(255,255,255,255);
    ofPopMatrix();  
}