//
//  Weather.cpp
//  flight
//
//  Created by Paul Paradiso on 3/13/16.
//
//

#include "Weather.h"

Weather::Weather(){
    ofxDatGui::setAssetPath(ofToDataPath(""));
    bg.loadConfigFile("weather/configs/background_config.json");
    bg.loadPresetsFromFile("weather/presets/background/b1.json");
    bg.loadPreset("one");
    weatherActiveMap["bg"] = true;
    keyWeatherMap[48] = "bg";
    
    //rain.loadShader("weather/default.vert", "weather-shaders/glsl/rain.frag");
    rain.loadConfigFile("weather/configs/rain_config.json");
    rain.loadPresetsFromFile("weather/presets/rain/r1.json");
    rain.loadPreset("Rain:None");
    weatherActiveMap["rain"] = true;
    keyWeatherMap[49] = "rain";
    
    //clouds.loadShader("weather/default.vert", "weather-shaders/glsl/clouds.frag");
    clouds.loadConfigFile("weather/configs/clouds_config.json");
    clouds.loadPresetsFromFile("weather/presets/clouds/c1.json");
    clouds.loadPreset("BKN");
    weatherActiveMap["clouds"] = true;
    keyWeatherMap[50] = "clouds";
    
    particulate.loadConfigFile("weather/configs/particulates_config.json");
    particulate.loadPresetsFromFile("weather/presets/particulate/p1.json");
    particulate.loadPreset("one");
    weatherActiveMap["particulates"] = false;
    keyWeatherMap[51] = "particulates";
    
    wind.loadConfigFile("weather/configs/wind_config.json");
    wind.loadPresetsFromFile("weather/presets/wind/w1.json");
    wind.loadPreset("one");
    weatherActiveMap["wind"] = true;
    keyWeatherMap[52] = "wind";
    
    mist.loadConfigFile("weather/configs/mist_config.json");
    mist.loadPresetsFromFile("weather/presets/mist/m1.json");
    mist.loadPreset("one");
    weatherActiveMap["mist"] = true;
    keyWeatherMap[53] = "mist";
    
    lightPollution.loadConfigFile("weather/configs/light_config.json");
    weatherActiveMap["lightPollution"] = true;
    keyWeatherMap[54] = "lightPollution";
    
    ozone.loadConfigFile("weather/configs/ozone_config.json");
    weatherActiveMap["ozone"] = true;
    keyWeatherMap[55] = "ozone";
    
    particles.loadConfigFile("weather/configs/particles_config.json");
    weatherActiveMap["particles"] = true;
    keyWeatherMap[56] = "particles";
    
    currentGui = 0;
    options = {"Background", "Rain", "Clouds", "Particulates", "Wind", "Mist", "Light Pollution", "Ozone", "Particles"};
    currentGui = 0;
    guiOptions = new ofxDatGuiDropdown("SELECT WEATHER GUI", options);
    guiOptions->setPosition(ofGetWidth() - 400, 0);
    guiOptions->onDropdownEvent(this, &Weather::onDropdownEvent);
    bGuiOn = false;
    bWeatherOn = true;
}

Weather::~Weather(){
    
}

void Weather::toggleGUI(){
    bGuiOn = !bGuiOn;
    //cout << "GuiOn = " << bGuiOn << endl;
}

void Weather::toggleWeather(){
    bWeatherOn = !bWeatherOn;
}

void Weather::dataReceived(ofxJSONElement & _data){
    cout << "Data Received." << endl;
}

void Weather::update(){
    if(bGuiOn){
        guiOptions->update();
        if(options[currentGui] == "Background"){
            bg.updateGui();
        }
        if(options[currentGui] == "Rain"){
            rain.updateGui();
        }
        if(options[currentGui] == "Clouds"){
            clouds.updateGui();
        }
        if(options[currentGui] == "Particulates"){
            particulate.updateGui();
        }
        if(options[currentGui] == "Wind"){
            wind.updateGui();
        }
        if(options[currentGui] == "Mist"){
            mist.updateGui();
        }
        if(options[currentGui] == "Light Pollution"){
            lightPollution.updateGui();
        }
        if(options[currentGui] == "Ozone"){
            ozone.updateGui();
        }
        if(options[currentGui] == "Particles"){
            particles.updateGui();
        }
    }
    //rain.updateGui();
    bg.update();
    stars.update();
    tower.update();
    rain.update();
    clouds.update();
    //clouds.updateGui();
    particulate.update();
    wind.update();
    mist.update();
    sunMoon.update();
    lightPollution.update();
    ozone.update();
    particles.update();
    //particulate.updateGui();
}

void Weather::update(string _data){
    ofxJSONElement data;
    data.parse(_data);
    //cout << "Rain data: " << data.getRawString() << endl;
    if(data["content"] == "weather"){
        rain.update(_data);
        //cout << "Updating clouds data - " << _data << endl;
        clouds.update(_data);
        wind.update(_data);
        mist.update(_data);
    }
    if(data["content"] == "traffic"){
        lightPollution.update(_data);
    }
    if(data["content"] == "airquality"){
        cout << "Received airquality.\n\t" << _data << endl;
        particles.update(_data);
        ozone.update(_data);
    }
    string weatherString = data["data"]["metar"]["cloud_friendlt"].asString();
    sysInfo->setWeatherString(weatherString);
}

void Weather::draw(){
    if(weatherActiveMap["bg"]){
        bg.draw();
    }
    tower.draw();
    if(bWeatherOn){
        if(weatherActiveMap["rain"]){
            rain.draw();
        }
        if(weatherActiveMap["clouds"]){
            clouds.draw();
        }
        if(weatherActiveMap["particulates"]){
            particulate.draw();
        }
        if(weatherActiveMap["mist"]){
            mist.draw();
        }
        if(weatherActiveMap["wind"]){
            wind.draw();
        }
        if(weatherActiveMap["lightPollution"]){
            lightPollution.draw();
        }
        if(weatherActiveMap["ozone"]){
            ozone.draw();
        }
        if(weatherActiveMap["particles"]){
            particles.draw();
        }
    }
    if(sysInfo->getDayPart() != "day"){
        stars.draw();
    }
    sunMoon.draw();
    //rain.drawGUI();
    //particulate.drawGUI();
}

void Weather::drawGUI(){
    if(bGuiOn){
        if(options[currentGui] == "Background"){
            bg.drawGUI();
        }
        if(options[currentGui] == "Rain"){
            rain.drawGUI();
        }
        if(options[currentGui] == "Clouds"){
            clouds.drawGUI();
        }
        if(options[currentGui] == "Particulates"){
            particulate.drawGUI();
        }
        if(options[currentGui] == "Wind"){
            wind.drawGUI();
        }
        if(options[currentGui] == "Mist"){
            mist.drawGUI();
        }
        if(options[currentGui] == "Light Pollution"){
            lightPollution.drawGUI();
        }
        if(options[currentGui] == "Ozone"){
            ozone.drawGUI();
        }
        if(options[currentGui] == "Particles"){
            particles.drawGUI();
        }
        guiOptions->draw();
    }
}

void Weather::onDropdownEvent(ofxDatGuiDropdownEvent _e){
    //currentGui = _e.target->getName();
    //cout << _e.child << endl;
    currentGui = _e.child;
}

void Weather::setSysInfo(shared_ptr<SystemInfo> _info){
    sysInfo = _info;
    bg.setSysInfo(sysInfo);
    sunMoon.setSysInfo(sysInfo);
    tower.setSysInfo(sysInfo);  
    rain.setSysInfo(sysInfo);
    clouds.setSysInfo(sysInfo);
    wind.setSysInfo(sysInfo);
}

void Weather::setAirport(string _a){
    airport = _a;
    tower.setAirport(airport);
}

void Weather::setWeatherActive(int _n){
    weatherActiveMap[keyWeatherMap[_n]] = !weatherActiveMap[keyWeatherMap[_n]];
    string status = " off.";
    if(weatherActiveMap[keyWeatherMap[_n]]){
        status = " on.";
    }
    cout << "Turned " << keyWeatherMap[_n] << status << endl;
}
