//
//  WeatherType.cpp
//  flight
//
//  Created by Paul Paradiso on 3/13/16.
//
//

#include "WeatherType.h"

WeatherType::WeatherType(){
    int planeWidth = ofGetWidth();
    int planeHeight = ofGetHeight();
    int planeGridSize = 20;
    int planeColums = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    plane.set(planeWidth, planeHeight, planeColums, planeRows, OF_PRIMITIVE_TRIANGLES);
    shaderTime = 0.0;
}

WeatherType::WeatherType(string _shaderPath){
    
}

WeatherType::~WeatherType(){
    
}

void WeatherType::updateGui(){
    //cout << "updating sliders" << endl;
    for(auto sIter = sliders.begin(); sIter != sliders.end(); sIter++){
        (*sIter)->update();
    }
}

void WeatherType::draw(){
    //uniforms["u_time"].set(uniforms["u_time"].get() + 0.005);
    //cout << "u_time = " << uniforms["u_time"] << endl;
    shaderTime += 0.005;
    //cout << "shaderTime = " << shaderTime << endl;
    shader.begin();
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shader.setUniform1f("u_time", shaderTime);
    for(auto uIter = uniforms.begin(); uIter != uniforms.end(); uIter++){
        //cout << "Setting uniform " << uIter->first << " to " << uIter->second.get() << endl;
        shader.setUniform1f(uIter->first, uIter->second.get());
    }
    //plane.drawWireframe();
    ofFill();
    //ofRectangle(0,0,ofGetWidth(), ofGetHeight());
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
}

void WeatherType::drawGUI(){
    for(auto sIter = sliders.begin(); sIter != sliders.end(); sIter++){
        (*sIter)->draw();
    }
}

void WeatherType::loadShader(string _vert, string _frag){
    shader.load(_vert, _frag);
}

void WeatherType::init(){
    
}

void WeatherType::updateUniforms(){
    
}

void WeatherType::setResolution(float _x, float _y){
    
}

void WeatherType::loadPresetsFromFile(string _file){
    ofFile file;
    file.open(ofToDataPath(_file));
    string buf = file.readToBuffer().getText();
    //cout << "Loading preset string. - " << buf << endl;
    presets.parse(buf);
    //cout << presets.getRawString() << endl;
    /*
    auto presetNames = presets.getMemberNames();
    for(auto pIter = presetNames.begin(); pIter != presetNames.end(); pIter++){
        cout << "Preset = " << *pIter << endl;
        auto names = presets[*pIter].getMemberNames();
        for(auto nIter = names.begin(); nIter != names.end(); nIter++){
            cout << "\t" << *nIter << " = " << presets[*pIter][*nIter].asFloat() << endl;
        }
    }
    */
}

void WeatherType::loadConfigFile(string _path){
    ofFile file;
    file.open(ofToDataPath(_path));
    string buf = file.readToBuffer().getText();
    ofxJSONElement config;
    config.parse(buf);
    loadShader(config["shaders"]["vert"].asString(), config["shaders"]["frag"].asString());
    auto cUniforms = config["uniforms"];
    float guiX = ofGetWidth() - 400;
    float guiY = 20;
    float sliderHeight = 20;
    for(auto uIter = cUniforms.begin(); uIter != cUniforms.end(); uIter++){
        string uName = (*uIter)["name"].asString();
        float min = (*uIter)["min"].asFloat();
        float max = (*uIter)["max"].asFloat();
        float iv = (*uIter)["initialValue"].asFloat();
        //cout << "adding uniform " << uName << ", " << min << ", " << max << ", " << iv << endl;
        ofParameter<float> p(uName, iv, min, max);
        uniforms.insert(make_pair(uName, p));
        //cout << "Created a parameter named - " << p.getName() << endl;
        //cout << "Creating slider named - " << uniforms[uName].getName() << endl;
        sliders.push_back(new ofxDatGuiSlider(uniforms[uName]));
        sliders.back()->setPrecision(4);
        sliders.back()->setWidth(400, 0.4);
        sliders.back()->setPosition(guiX, guiY);
        sliders.back()->onSliderEvent(this, &WeatherType::sliderUpdated);
        guiY += sliders.back()->getHeight();
    }
}

void WeatherType::loadPreset(string _preset){
    cout << "Loading preset " << _preset << endl;
    auto levels = ofSplitString(_preset, ":");
    auto presetLevel = presets[levels[0]];
    if(levels.size() > 1){
        for(int i = 1; i < levels.size(); i++){
            presetLevel = presetLevel[levels[i]];
        }
    }
    auto uniformNames = presetLevel.getMemberNames();
    for(auto uIter = uniformNames.begin(); uIter != uniformNames.end(); uIter++){
        string uniformName = *uIter;
        //float value = presets[_preset][*uIter].asFloat();
        float value = presetLevel[*uIter].asFloat();
        //cout << "Adding " << uniformName << " to uniforms, with initial value " << value << endl;
        //uniforms[uniformName] = value;
        if(uniforms.find(uniformName) != uniforms.end()){
            uniforms[uniformName].set(value);
            //cout << "Setting uniform - " << uniformName << " to " << value << endl;
        }
    }
    //uniforms["u_time"].set(0.0);
}

void WeatherType::sliderUpdated(ofxDatGuiSliderEvent _e){
    //cout << "Slider " << _e.target->getName() << " = " << _e.value << endl;
    uniforms[_e.target->getName()].set(_e.value);
}

void WeatherType::setSysInfo(shared_ptr<SystemInfo> _s){
    sysInfo = _s;
}

void WeatherType::setAirport(string _a){
    airport = _a;
}