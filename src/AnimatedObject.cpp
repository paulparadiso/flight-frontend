//
//  AnimatedObject.cpp
//  flight
//
//  Created by Paul Paradiso on 4/4/16.
//
//

#include "AnimatedObject.h"

AnimatedObjectElement::AnimatedObjectElement(){
    
}

AnimatedObjectElement::AnimatedObjectElement(string _type, string _path){
    load(_type, _path);
}

AnimatedObjectElement::~AnimatedObjectElement(){
    
}

void AnimatedObjectElement::load(string _type, string _path){
    if(_type == "png"){
        //cout << "Loading png." << endl;
        elementType = ELEMENT_TYPE_IMAGE;
        img.setUseTexture(false);
        img.load(_path);
    }
    if(_type == "svg"){
        //cout << "Loading svg." << endl;
        elementType = ELEMENT_TYPE_SVG;
        svg.load(_path);
    }
    if(_type == "mov"){
        elementType = ELEMENT_TYPE_VIDEO;
        vid.load(_path);
    }
}

void AnimatedObjectElement::load(string _path){
    
}

void AnimatedObjectElement::update(){
    if(elementType == ELEMENT_TYPE_VIDEO){
        vid.update();
    }
}

void AnimatedObjectElement::draw(){
    if(elementType == ELEMENT_TYPE_IMAGE){
        if(!img.isUsingTexture()) {
            //img.getTexture().loadData(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);
            img.getTexture().loadData(img.getPixels(), GL_RGBA);
            img.setUseTexture(true);
        }
        img.draw(-(img.getWidth() / 2),-(img.getHeight() / 2));
    }
    if(elementType == ELEMENT_TYPE_SVG){
        ofPushMatrix();
        ofTranslate(-(svg.getWidth() /2), -(svg.getHeight() / 2));
        svg.draw();
        ofPopMatrix();
    }
    if(elementType == ELEMENT_TYPE_VIDEO){
        //cout << "Drawing video." << endl;
        if(!vid.isPlaying()){
            vid.play();
        }
        vid.draw(0,0,ofGetWidth(), ofGetHeight());
    }
}

void AnimatedObjectElement::draw(int _x, int _y, int _w, int _h){
    if(elementType == ELEMENT_TYPE_IMAGE){
        if(!img.isUsingTexture()) {
            //img.getTexture().loadData(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGBA);
            img.getTexture().loadData(img.getPixels(), GL_RGBA);
            img.setUseTexture(true);
        }
        cout << "Drawing image at " << _x << ", " << _y << endl;
        img.draw(_x,_y,_w,_h);
    }
}

int AnimatedObjectElement::getWidth(){
    if(elementType == ELEMENT_TYPE_IMAGE){
        return img.getWidth();
    } else {
        return svg.getWidth();
    }
}

int AnimatedObjectElement::getHeight(){
    if(elementType == ELEMENT_TYPE_IMAGE){
        return img.getHeight();
    } else {
        return svg.getHeight();
    }
}

void AnimatedObjectElement::reset(){
    if(elementType == ELEMENT_TYPE_VIDEO){
        vid.play();
        vid.setPosition(0.0);
        vid.setLoopState(OF_LOOP_NONE);
    }
}

bool AnimatedObjectElement::isDone(){
    if(elementType == ELEMENT_TYPE_VIDEO){
        return !vid.isPlaying();
        //return false;
    } else {
        return false;
    }
}

int AnimatedObjectElement::getType(){
    return elementType;
}

AnimatedObject::AnimatedObject(){
    setRate(DEFAULT_FPS);
    bLoaded = false;
    //createGui();
}

AnimatedObject::AnimatedObject(string _path, bool _random, shared_ptr<SystemInfo> _info){
    setRate(DEFAULT_FPS);
    bLoaded = false;
    bRandom = _random;
    loadFromDirectory(_path);
    sysInfo = _info;
    setRate(rate);
    currentElement = 0;
    //createGui();
}

AnimatedObject::~AnimatedObject(){
    
}

void AnimatedObject::createGui(){
    minButton = new ofxDatGuiButton("MIN");
    minButton->setEnabled(false);
    minButton->setPosition(0, 0);
    minButton->setWidth(200);
    minButton->onButtonEvent(this, &AnimatedObject::buttonUpdated);
    
    maxButton = new ofxDatGuiButton("MAX");
    maxButton->setEnabled(false);
    maxButton->setPosition(200, 0);
    maxButton->setWidth(200);
    maxButton->onButtonEvent(this, &AnimatedObject::buttonUpdated);
    
    rateSlider = new ofxDatGuiSlider("RATE", RATE_MIN, RATE_MAX, rate);
    rateSlider->setPosition(0, 25);
    rateSlider->setPrecision(4);
    rateSlider->setWidth(400, 0.3);
    rateSlider->onSliderEvent(this, &AnimatedObject::sliderUpdated);
    
    speedSlider = new ofxDatGuiSlider("SPEED", SPEED_MIN, SPEED_MAX, speed);
    speedSlider->setPosition(0, 50);
    speedSlider->setPrecision(4);
    speedSlider->setWidth(400, 0.3);
    speedSlider->onSliderEvent(this, &AnimatedObject::sliderUpdated);

    scaleSlider = new ofxDatGuiSlider("SCALE", SCALE_MIN, SCALE_MAX, scale);
    scaleSlider->setPosition(0, 75);
    scaleSlider->setPrecision(4);
    scaleSlider->setWidth(400, 0.3);
    scaleSlider->onSliderEvent(this, &AnimatedObject::sliderUpdated);
    
    startYSlider = new ofxDatGuiSlider("STARTY", 0.0, 1.0, startY);
    startYSlider->setPosition(0, 100);
    startYSlider->setPrecision(4);
    startYSlider->setWidth(400, 0.3);
    startYSlider->onSliderEvent(this, &AnimatedObject::sliderUpdated);
    
    endYSlider = new ofxDatGuiSlider("ENDY", 0.0, 1.0, endY);
    endYSlider->setPosition(0, 125);
    endYSlider->setPrecision(4);
    endYSlider->setWidth(400, 0.3);
    endYSlider->onSliderEvent(this, &AnimatedObject::sliderUpdated);

    minMaxToggle = new ofxDatGuiToggle("MIRROR", bMirrored);
    minMaxToggle->setEnabled(bMirrored);
    minMaxToggle->setWidth(200);
    minMaxToggle->setPosition(0, 150);
    minMaxToggle->onButtonEvent(this, &AnimatedObject::buttonUpdated);
    
    saveButton = new ofxDatGuiButton("SAVE");
    saveButton->setEnabled(false);
    saveButton->setPosition(200, 150);
    saveButton->setWidth(200);
    saveButton->onButtonEvent(this, &AnimatedObject::buttonUpdated);
    
    rotationSlider = new ofxDatGuiSlider("ROTATION", 0.0, 90.0, startY);
    rotationSlider->setPosition(0, 175);
    rotationSlider->setPrecision(4);
    rotationSlider->setWidth(400, 0.3);
    rotationSlider->onSliderEvent(this, &AnimatedObject::sliderUpdated);
}

void AnimatedObject::reset(){
    if(elements[0].getType() == ELEMENT_TYPE_VIDEO){
        elements[0].reset();
        bDone = false;
        return;
    }
    string which = "min";
    if(!bSetMin){
        which = "max";
    }
    currentElement = 0;
    scale = getSetting("scale", which);
    if(scale < 0.0){
        scale = DEFAULT_SCALE;
    }
    speed = getSetting("speed", which);
    if(speed < 0.0){
        speed = DEFAULT_SPEED;
    }
    /*
    startY = getSetting("startY");
    if(startY < 0.0){
        startY = DEFAULT_START_Y;
    }
    endY = getSetting("endY");
    if(endY < 0.0){
        endY = DEFAULT_END_Y;
    }
    */
    if(bRandom){
        //startY = ofRandom(ofGetHeight());
        //endY = ofRandom(ofGetHeight());
        startY = ofRandom(1.0);
        endY = ofRandom(1.0);
    } else {
        //startY = ofGetHeight() / 3;
        //endY = startY;
        startY = getSetting("startY", which);
        endY = getSetting("endY", which);
        rate = getSetting("frameRate", which);
        setRate(rate);
    }
    if(direction == OBJECT_DIRECTION_RIGHT){
        position.x = -getWidth();
    } else {
        position.x = ofGetWidth() + getWidth();
    }
    position.y = startY;
    /*
    float loopLength = getSetting("loopLength", which) * 1000.0;
    if(loopLength < 0.0){
        loopLength = DEFAULT_LOOP_LENGTH;
    }
    loopLength *= 1000.0;
    if(loopLength < elements.size()){
        loopLength = elements.size();
    }
    */
    /*
    if(loopLength > 0.0){
        setRate(int(loopLength / elements.size()));
    }
    */
    timeOfLastUpdate = ofGetElapsedTimeMillis();
    bDone = false;
    timeOfLastChange = ofGetElapsedTimeMillis();
    //cout << "Position = " << position.x << ", " << position.y << endl;
}

void AnimatedObject::loadFromDirectory(string _path, bool _bLoadConfig){
    //cout << "Loading object from " << _path << endl;
    ofDirectory dir;
    dir.allowExt("svg");
    dir.allowExt("png");
    dir.allowExt("mov");
    dir.listDir(_path);
    dir.sort();
    string fileType = "";
    auto files = dir.getFiles();
    for(auto fIter = files.begin(); fIter != files.end(); fIter++){
        //cout << "loading " << fIter->getExtension() << " from " << fIter->getEnclosingDirectory() << endl;
        fileType = fIter->getExtension();
        elements.push_back(AnimatedObjectElement(fIter->getExtension(), fIter->getEnclosingDirectory() + fIter->getFileName()));
    }
    bLoaded = true;
    if(_bLoadConfig && fileType != "mov"){
        loadSettings(_path);
    }
    createGui();
    refreshSettings("min");
    //reset();
};

void AnimatedObject::loadSettings(string _path){
    //ofDirectory dir;
    //dir.listDir(path);
    //ofxJSONElement details;
    jsonFile = _path + "/settings.json";
    cout << "Checking " << jsonFile << endl;
    if(details.open(jsonFile) > 0){
        //cout << "Loaded from path = " << jsonFile << endl;
        //cout << details.getRawString() << endl;
        auto minSettingsNames = details["min"].getMemberNames();
        auto maxSettingsNames = details["max"].getMemberNames();
        for(auto sIter = minSettingsNames.begin(); sIter != minSettingsNames.end(); sIter++){
            if(*sIter == "mirrored"){
                setMirrored(details[*sIter].asBool());
            } else {
                addSettings(*sIter, details["min"][*sIter].asFloat(), "min");
            }
        }
        for(auto sIter = maxSettingsNames.begin(); sIter != maxSettingsNames.end(); sIter++){
            if(*sIter == "mirrored"){
                setMirrored(details[*sIter].asBool());
            } else {
                addSettings(*sIter, details["max"][*sIter].asFloat(), "max");
            }
        }
        setMirrored(details["mirrored"].asBool());
        bHaveSettings = true;
        if(details["static"].asBool()){
            bStatic = true;
            if(details["fit"].asBool()){
                bFitToScreen = true;
            }
        }
        if(details["fit"].asBool()){
            setStaticPosition(ofVec2f(0,0));
            bFitToScreen = true;
        }
    }
    /*
    if(!bRandom){
        string settingsKey = "min";
        if(!bSetMin){
            settingsKey = "max";
        }
        scale = settings[settingsKey]["scale"];
        rate = settings[settingsKey]["loopLength"];
        speed = settings[settingsKey]["speed"];
        if(startY > 1.0){
            startY = ofGetHeight() * (settings[settingsKey]["startY"] / ofGetHeight());
            bSettingsChanged = true;
        } else {
            startY = ofGetHeight() * settings[settingsKey]["startY"];
        }
        if(endY > 1.0){
            endY = ofGetHeight() * (settings[settingsKey]["endY"] / ofGetHeight());
            bSettingsChanged = true;
        } else {
            endY = ofGetHeight() * settings[settingsKey]["endY"]; 
        }
    }
    */
}

void AnimatedObject::setStaticPosition(ofVec2f _pos){
    position = _pos;
    cout << "Set static position to " << position.x << ", " << position.y << endl;
    bStatic = true;
}

bool AnimatedObject::isLoaded(){
    if(bLoaded){
        return true;
    } else {
        return false;
    }
}

string AnimatedObject::getPath(){
    return path;
}

void AnimatedObject::update(){
    if(elements.size() < 1){
        return;
    }
    int now = ofGetElapsedTimeMillis();
    if(now - timeOfLastChange > displayTime){
        currentElement = (currentElement + 1) % elements.size();
        timeOfLastChange = now;
        //cout << "switched elements" << endl;
    }
    int timeDiff = now - timeOfLastUpdate;
    //int pixelsMoved = (speed * ((float)timeDiff / 1024.0));
    //float velocity = speed / 1000.0;
    //float pixelsMoved = speed * (float(timeDiff) / 2000.0);
    if(!bStatic){
        float pixelsMoved = speed * (float(timeDiff) / 1000.0);
        //cout << "timeDiff = " << timeDiff << endl;
        //cout << "pixelsMoved = " << pixelsMoved << endl;
        if(direction == OBJECT_DIRECTION_RIGHT){
            position.x += pixelsMoved;
            if(position.x > ofGetWidth() + getWidth()){
                //position.x = 0;
                if(bLooping){
                    position.x = -getWidth();
                } else {
                    bDone = true;
                }
            }
        } else {
            position.x -= pixelsMoved;
            if(position.x < -getWidth()){
                //position.x = ofGetWidth();
                if(bLooping){
                    position.x = ofGetWidth();
                } else {
                    bDone = true;
                }
            }
        }
    }
    //cout << "Flight position set to " << position.x << endl;
    float xPct = position.x / ofGetWidth();
    float yDiff = (ofGetHeight() * endY) - (ofGetHeight() * startY);
    if(!bStatic){
        //cout << "xPct = " << xPct << ". yDiff = " << yDiff << endl;
        //cout << "endY = " << endY << ". startY = " << startY << endl;
        position.y = (ofGetHeight() * startY) + (yDiff * xPct);
    }
    timeOfLastUpdate = now;
    //cout << "AO position set to " << position.x << ", " << position.y << endl;
    if(sysInfo != nullptr){
        if(sysInfo->getGuiOn()){
            minButton->update();
            maxButton->update();
            speedSlider->update();
            rateSlider->update();
            scaleSlider->update();
            startYSlider->update();
            endYSlider->update();
            minMaxToggle->update();
            saveButton->update();
            rotationSlider->update();
        }
    }
    elements[currentElement].update();
    if(elements[currentElement].isDone()){
        bDone = true;
    }
}

void AnimatedObject::draw(){
    if(elements.size() < 1){
        cout << "Empty item." << endl;
        return;
    }
    if(elements[currentElement].getType() == ELEMENT_TYPE_VIDEO){
        elements[currentElement].draw();
        return;
    }
    //elements[currentElement]->draw();
    float xPos = position.x - (getWidth() / 2);
    float yPos = position.y - (getHeight() / 2);
    //cout << "Drawing at " << position.x << ", " << position.y << endl;
    ofPushMatrix();
    //ofTranslate(xPos, yPos);
    //ofScale(scale, scale);
    ofTranslate(position.x, position.y);
    if((bMirrored && (direction == OBJECT_DIRECTION_RIGHT)) || (!bMirrored && (direction == OBJECT_DIRECTION_LEFT))){
        ofRotateY(180);
    }
    ofRotateZ(rotation);
    //ofTranslate(-getWidth() / 2, -getHeight() / 2);
    ofScale(scale, scale);
    //ofRotateY(180);
    elements[currentElement].draw();
    ofPopMatrix();
    if(sysInfo != nullptr){
        if(sysInfo->getGuiOn()){
            drawGui();
        }
    }
}

void AnimatedObject::drawGui(){
    minButton->draw();
    maxButton->draw();
    speedSlider->draw();
    rateSlider->draw();
    scaleSlider->draw();
    startYSlider->draw();
    endYSlider->draw();
    minMaxToggle->draw();
    saveButton->draw();
    rotationSlider->draw();
}

void AnimatedObject::setRate(int _fps){
    rate = _fps;
    if(rate > 0.0){
        displayTime = 1000 / rate;
    }   
    //cout << "Display time for object set to - " << displayTime << endl;
}

void AnimatedObject::setMirrored(bool _m){
    bMirrored = _m;
}

void AnimatedObject::addSettings(string _label, float _val, string _set){
    if(_set == "max"){
        //maxSettings[_label] = _val;
        //cout << "Setting max " << _label << " to " << _val << endl;
        settings[_set][_label] = _val;
    } else if(_set == "min"){
        //minSettings[_label] = _val;
        //cout << "Setting min " << _label << " to " << _val << endl;
        settings[_set][_label] = _val;
    } else {
        //maxSettings[_label] = _val;
        //minSettings[_label] = _val;
        settings["min"][_label] = _val;
        settings["max"][_label] = _val;
    }
}

float AnimatedObject::getSetting(string _setting, int _type){
    float ret = -1.0;
    switch(_type){
        case SETTING_MIN: {
            if(settings["min"].find(_setting) != settings["min"].end()){
                ret = settings["min"][_setting];
                break;
            }
        }
        case SETTING_MAX: {
            if(settings["max"].find(_setting) != settings["max"].end()){
                ret = settings["max"][_setting];
                break;
            }
        }
        case SETTING_MIX: {
            if(settings["min"].find(_setting) != settings["min"].end() && settings["max"].find(_setting) != settings["max"].end()){
                float min = settings["max"][_setting];
                float max = settings["max"][_setting];
                float diff = max - min;
                ret = min + (diff * ofRandom(1.0));
                break;
            } else {
                //cout << "Couldn't find setting - " << _setting << endl;
            }
        }
        default:{
            break;
        }
    }
    return ret;
}

float AnimatedObject::getSetting(string _setting, string _type){
    float ret = -1.0;
    if(_type == "min"){
        if(settings["min"].find(_setting) != settings["min"].end()){
            ret = settings["min"][_setting];
        }
    }
    if(_type == "max"){
        if(settings["max"].find(_setting) != settings["max"].end()){
            ret = settings["max"][_setting];
        }
    }
    if(_type == "mix"){
        if(settings["min"].find(_setting) != settings["min"].end() && settings["max"].find(_setting) != settings["max"].end()){
            float min = settings["max"][_setting];
            float max = settings["max"][_setting];
            float diff = max - min;
            ret = min + (diff * ofRandom(1.0));
        } else {
            //cout << "Couldn't find setting - " << _setting << endl;
        }
    }
    return ret;
}

int AnimatedObject::getWidth(){
    if(elements.size() > 0){
        return elements[0].getWidth();
    } else {
        return 0;
    }
}

int AnimatedObject::getHeight(){
    if(elements.size() > 0){
        return elements[0].getHeight();
    } else {
        return 0;
    }
}

ofVec2f AnimatedObject::getPosition(){
    return position;  
}

void AnimatedObject::setDirection(int _d){
    direction = _d;
}

bool AnimatedObject::isDone(){
    return bDone;
}

void AnimatedObject::refreshSettings(string _s){
    string selector = "min";
    if(_s == "max"){
        selector = "max";
    }
    //scale = getSetting("scale", selector);
    //speed = getSetting("speed", selector);
    //startY = getSetting("startY", selector);
    //endY = getSetting("endY", selector);
    //rate = getSetting("frameRate", selector);
    scale = details[selector]["scale"].asFloat();
    speed = details[selector]["speed"].asFloat();
    rate = details[selector]["frameRate"].asFloat();
    startY = details[selector]["startY"].asFloat();
    endY = details[selector]["endY"].asFloat();
    speedSlider->setValue(speed);
    rateSlider->setValue(rate);
    scaleSlider->setValue(scale);
    startYSlider->setValue(startY);
    endYSlider->setValue(endY);
}

void AnimatedObject::sliderUpdated(ofxDatGuiSliderEvent _e){
    string name = _e.target->getName();
    string which = "max";
    if(bSetMin){
        which = "min";
    }
    if(name == "SPEED"){
        speed = _e.value;
        details[which]["speed"] = speed;
    }
    if(name == "SCALE"){
        scale = _e.value;
        details[which]["scale"] = scale;
    }
    if(name == "STARTY"){
        startY = _e.value;
        details[which]["startY"] = _e.value;
    }
    if(name == "ENDY"){
        endY = _e.value;
        details[which]["endY"] = _e.value;
    }
    if(name == "RATE"){
        rate = _e.value;
        details[which]["frameRate"] = rate;
        setRate(rate);
    }
    if(name == "ROTATION"){
        rotation = _e.value;
    }
}

void AnimatedObject::buttonUpdated(ofxDatGuiButtonEvent _e){
    string name = _e.target->getName();
    cout << name << " pushed." << endl;
    if(name == "MIN"){
        //if(_e.enabled){
            bSetMin = true;
            refreshSettings("min");
            //_e.enabled = false;
        //}
    }
    if(name == "MAX"){
        //if(_e.enabled){
            bSetMin = false;
            refreshSettings("max");
            //_e.enabled = false;
        //}
    }
    if(name == "MIRROR"){
        cout << "MIRROR toggled." << endl;
        bMirrored = _e.enabled;
        details["mirrored"] = bMirrored;
    }
    if(name == "SAVE"){
        //if(_e.enabled){
        saveSettings();
        //}
    }
}

void AnimatedObject::saveSettings(){
    cout << "Saving settings." << endl;
    details.save(jsonFile);
    cout << details.getRawString() << endl;
}
