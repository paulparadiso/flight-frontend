//
//  AnimatedObject.h
//  flight
//
//  Created by Paul Paradiso on 4/4/16.
//
//

#ifndef __flight__AnimatedObject__
#define __flight__AnimatedObject__

#include <stdio.h>
#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxJSONElement.h"
#include "SystemInfo.h"
#include "ofxDatGui.h"

#define ELEMENT_TYPE_IMAGE 0
#define ELEMENT_TYPE_SVG 1
#define ELEMENT_TYPE_VIDEO 2

#define SETTING_MIN 0
#define SETTING_MAX 1
#define SETTING_MIX 2

#define DEFAULT_FPS 50

#define DEFAULT_SPEED 300
#define DEFAULT_START_Y (ofGetHeight() / 3 * 2)
#define DEFAULT_END_Y (ofGetHeight() / 3 * 2)
#define DEFAULT_SCALE 0.75
#define DEFAULT_LOOP_LENGTH 0.2
#define GUI_ELEMENT_HEIGHT 20

#define SPEED_MIN 0.0
#define SPEED_MAX 4000.0
#define RATE_MIN 0.0
#define RATE_MAX 400.0
#define SCALE_MIN 0.0
#define SCALE_MAX 10.0

enum objectDirection{
    OBJECT_DIRECTION_RIGHT,
    OBJECT_DIRECTION_LEFT
};

class AnimatedObjectElement{
public:
    AnimatedObjectElement();
    AnimatedObjectElement(string _type, string _path);
    ~AnimatedObjectElement();
    void load(string _type, string _path);
    void load(string _path);
    void update();
    void draw();
    void reset();
    bool isDone();
    void draw(int _x, int _y, int _w, int _h);
    int getWidth();
    int getHeight();
    int getType();
private:
    void drawGui();
    int elementType;
    ofImage img;
    ofxSVG svg;
    ofVideoPlayer vid;
};

class AnimatedObject{
public:
    AnimatedObject();
    AnimatedObject(string _path, bool _random = true, shared_ptr<SystemInfo> _info=nullptr);
    ~AnimatedObject();
    void loadFromDirectory(string _path, bool _bLoadConfig=true);
    void reset();
    void update();
    void draw();
    void setRate(int _fps);
    void setMirrored(bool _m);
    void addSettings(string _label, float _val, string _set);
    bool isLoaded();
    int getWidth();
    int getHeight();
    ofVec2f getPosition();
    string getPath();
    void setDirection(int _d);
    bool isDone();
    void loadSettings(string _path);
    void sliderUpdated(ofxDatGuiSliderEvent _e);
    void buttonUpdated(ofxDatGuiButtonEvent _e);
    void setLooping(bool _l){bLooping = _l;}
    void setScale(float _s){scale = _s;}
    void setSpeed(float _s){speed = _s;}
    void setStartY(float _sY){startY = _sY;}
    void setEndY(float _eY){endY = _eY;}
    void setStaticPosition(ofVec2f _pos);
    void setRotation(float _r){rotation = _r;}
    //void setMirrored(bool _m){bMirrored = _m;}
private:
    void drawGui();
    void createGui();
    void refreshSettings(string _s);
    void saveSettings();
    vector<AnimatedObjectElement> elements;
    int currentElement = 0;
    int timeOfLastChange;
    int timeOfLastUpdate = 0;
    int startTime;
    int rate;
    int displayTime;
    ofVec2f position;
    ofVec2f dimensions;
    bool bMirrored;
    float getSetting(string _setting, int _type = SETTING_MIX);
    float getSetting(string _setting, string _type = "min");
    map<string, map<string, float> > settings;
    map<string, float> minSettings;
    map<string, float> maxSettings;
    bool bLoaded;
    float speed;
    float scale;
    float startY;
    float startX;
    float endY;
    float endX;
    string path;
    int direction = OBJECT_DIRECTION_RIGHT;
    bool bDone;
    bool bRandom = true;
    string jsonFile;
    ofxJSONElement details;
    ofxDatGuiButton* minButton;
    ofxDatGuiButton* maxButton;
    ofxDatGuiSlider* speedSlider;
    ofxDatGuiSlider* rateSlider;
    ofxDatGuiSlider* scaleSlider;
    ofxDatGuiSlider* startYSlider;
    ofxDatGuiSlider* endYSlider;
    ofxDatGuiToggle* minMaxToggle;
    ofxDatGuiButton* saveButton;
    ofxDatGuiSlider* rotationSlider;
    shared_ptr<SystemInfo> sysInfo = nullptr;
    bool bSetMin = false;
    bool bLooping = false;
    bool bSettingsChanged;
    bool bStatic = false;
    bool bFitToScreen = false;
    float rotation = 0.0;
    bool bHaveSettings = false;
};

#endif /* defined(__flight__AnimatedObject__) */
