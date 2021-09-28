//
//  WeatherLightPollution.cpp
//  flight
//
//  Created by Paul Paradiso on 12/4/17.
//
//

#include "WeatherLightPollution.h"

WeatherLightPollution::WeatherLightPollution(){
    img.allocate(1920, 270, OF_IMAGE_COLOR_ALPHA);
    pix.allocate(1920, 270, OF_IMAGE_COLOR_ALPHA);
}

/*
void WeatherLightPollution::update(){
    ofPixels pix;
    cout << "starting traffic render." << endl;
    for(int y = 0; y < 270; y += 100){
        for(int x = 0; x < 1920; x += 100){
            ofColor color(0,0,0,0);
            for(int i = 0; i < numCars; i++){
                float mult = sin(ofGetElapsedTimef() * (0.3 / i));
                float lightX = 1920.0 * mult;
                ofVec2f lightPos(lightX, -520.0);
                ofVec2f redLightPos = lightPos;
                redLightPos.x -= 20.0;
                float d = abs(x - lightPos.x);
                float dy = 0.0;
                float dr = abs(x - redLightPos.x);
                if(d < maxDist){
                    dy = 1.0 - y / 270.0;
                    color += ofColor(255, 241, 224, ((0.5 - 0.75 * (d / maxDist)) * dy));
                }
                if(dr < maxDist){
                    dy = 1.0 - y / 270.0;
                    color += ofColor(140, 0, 0, ((0.5 - 0.75 * (dr / maxDist)) * dy));
                    color /= 2;
                }
            }
            //pix.setColor(x, y, color);
            //cout << "Setting color for " << x << ", " << y << endl;
            img.setColor(x, y, color);
        }
        
    }
    cout << "ending traffic render." << endl;
    img.update();
    //img.setFromPixels(pix);
}
*/
 
void WeatherLightPollution::update(string _data){
    
}

/*
void WeatherLightPollution::draw(){
    float scale = ofGetHeight() / 1080.0;
    img.draw(0, 180 * scale, 1920.0 * scale, 270 * scale);
}
*/
