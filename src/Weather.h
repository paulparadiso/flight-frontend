//
//  Weather.h
//  flight
//
//  Created by Paul Paradiso on 3/13/16.
//
//

#ifndef __flight__Weather__
#define __flight__Weather__

#include <stdio.h>
#include "ofMain.h"
#include "DataClient.h"
//#include "WeatherType.h"
#include "WeatherRain.h"
#include "WeatherClouds.h"
#include "WeatherParticulate.h"
#include "WeatherBackground.h"
#include "WeatherMist.h"
#include "WeatherWind.h"
#include "WeatherStars.h"
#include "WeatherLightPollution.h"
#include "ofxDatGui.h"
#include "SystemInfo.h"
#include "SunMoon.h"
#include "Tower.h"

class Weather : public DataClient{
public:
    Weather();
    ~Weather();
    void init();
    void update();
    void update(string _data);
    void draw();
    void drawGUI();
    void toggleGUI();
    void setGui(bool _g){bGuiOn = _g;};
    void toggleWeather();
    void dataReceived(ofxJSONElement & _data);
    void onDropdownEvent(ofxDatGuiDropdownEvent _e);
    void setSysInfo(shared_ptr<SystemInfo> _info);
    void setAirport(string _a);
    void setWeatherActive(int _n);
private:
    WeatherBackground bg;
    WeatherStars stars;
    WeatherRain rain;
    WeatherClouds clouds;
    WeatherParticulate particulate;
    WeatherParticulate ozone;
    WeatherParticulate particles;
    WeatherWind wind;
    WeatherMist mist;
    WeatherLightPollution lightPollution;
    ofxDatGuiDropdown* guiOptions;
    SunMoon sunMoon;
    Tower tower;
    vector<string> options;
    int currentGui;
    shared_ptr<SystemInfo> sysInfo;
    string airport = "JFK";
    bool bGuiOn;
    bool bWeatherOn;
    map<string, bool> weatherActiveMap;
    map<int, string> keyWeatherMap;
};

#endif /* defined(__flight__Weather__) */
