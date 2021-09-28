//
//  WeatherType.h
//  flight
//
//  Created by Paul Paradiso on 3/13/16.
//
//

#ifndef __flight__WeatherType__
#define __flight__WeatherType__

#include <stdio.h>
#include "ofMain.h"
#include "ofxJSON.h"
#include "DataClient.h"
#include "ofxDatGui.h"
#include "SystemInfo.h"

class WeatherType{
public:
    WeatherType();
    WeatherType(string _shaderPath);
    ~WeatherType();
    virtual void update() = 0;
    virtual void updateGui();
    virtual void update(string _data){}
    virtual void draw();
    virtual void drawGUI();
    virtual void sliderUpdated(ofxDatGuiSliderEvent _e);
    virtual void setResolution(float _x, float _y);
    virtual void loadPresetsFromFile(string _presets);
    virtual void loadConfigFile(string _path);
    virtual void loadPreset(string _preset);
    virtual void loadShader(string _vert, string _frag);
    void setSysInfo(shared_ptr<SystemInfo> _s);
    virtual void setAirport(string _a);
protected:
    virtual void init();
    virtual void updateUniforms();
    //virtual void updatePresets();
    float shaderTime;
    ofShader shader;
    //map<string, float> uniforms;
    map<string, ofParameter<float> > uniforms;
    map<string, ofParameter<float> > uniformParams;
    ofVec2f resolution;
    ofxJSONElement presets;
    ofPlanePrimitive plane;
    vector<ofxDatGuiSlider *> sliders;
    shared_ptr<SystemInfo> sysInfo = nullptr;
    string airport = "JFK";
};

#endif /* defined(__flight__WeatherType__) */
