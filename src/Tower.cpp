//
//  Tower.cpp
//  flight
//
//  Created by Paul Paradiso on 4/8/16.
//
//

#include "Tower.h"

Tower::Tower(){
    
}

void Tower::update(){
    if(bAnimated){
        if(ao != nullptr){
            //cout << "Tower updating elements." << endl;
            ao->update();
        } else {
            //cout << "AO = nullptr." << endl;
        }
    }
}

void Tower::update(string _data){
    
}

void Tower::draw(){
    if(!bAnimated){
        towerImg.draw(0, 0, towerImg.getWidth() * scale, towerImg.getHeight() * scale);
    } else {
        //cout << "Drawing AO" << endl;
        ofPushMatrix();
        ofTranslate(-50, 0);
        ao->draw();
        ofPopMatrix();
    }
}

void Tower::setAirport(string _a){
    airport = _a;
    stringstream imgPath;
    stringstream jsonPath;
    stringstream aoPath;
    imgPath << "animations/towers/" << airport << ".png";
    jsonPath << "animations/towers/" << airport << ".json";
    aoPath << "animations/towers/" << airport << "/images_0";
    //ofxJSONElement d;
    //d.open(jsonPath.str());
    //towerPos.x = ofGetWidth() * d["params"]["x"].asFloat();
    //towerPos.y = ofGetHeight() * d["params"]["y"].asFloat();
    //scale = d["params"]["scale"].asFloat();
    scale = ofGetWidth() / 1920.0;
    towerPos.set(ofGetWidth()/2,ofGetHeight()/2);
    //cout << "Checking directory - " << aoPath.str() << endl;
    ofDirectory aoDir(aoPath.str());
    bool loadedAO = false;
    if(aoDir.exists()){
        aoDir.allowExt("png");
        aoDir.allowExt("svg");
        aoDir.listDir();
        if(aoDir.size() > 0){
            cout << "Loading animated tower." << endl;
            if(ao != nullptr){
                delete ao;
                ao = nullptr;
            }
            ao = new AnimatedObject(aoPath.str(), false);
            ao->setStaticPosition(towerPos);
            ao->setScale(scale);
            ao->setRate(4);
            bAnimated = true;
            loadedAO = true;
        }
    }
    if(!loadedAO){
        cout << "Loading static tower." << endl;
        towerImg.load(imgPath.str());
        bAnimated = false;
    }
}
