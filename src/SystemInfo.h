//
//  SystemInfo.h
//  flight
//
//  Created by Paul Paradiso on 4/7/16.
//
//

#ifndef __flight__SystemInfo__
#define __flight__SystemInfo__

#include <stdio.h>
#include "ofMain.h"
#include "ofxJSON.h"
#include "TextBlock.h"

#define DEFAULT_PANEL_WIDTH 0.22
#define TIMEZONE_API_KEY "ROTQ83OH4VG4"
#define DISPLAY_TEST_TEXT false
#define FADE_IN 2000

using namespace std::chrono;

class Time{
public:
    Time();
    ~Time();
    void setHours(int _h);
    void setMinutes(int _m);
    void setUtcHours(int _h);
    void setUtcMinutes(int _m);
    int getHours();
    int getMinutes();
    int getUtcHours();
    int getUtcMinutes();
private:
    int hours;
    int minutes;
    int utcHours;
    int utcMinutes;
};

class SystemInfo{
public:
    SystemInfo();
    ~SystemInfo();
    void urlResponse(ofHttpResponse & response);
    void init();
    void setBaseUrl(string _b);
    void draw();
    Time getSunrise();
    Time getSunset();
    Time getCurrentTime();
    string getCurrentTimeString();
    string getAirport();
    void setAirport(string _a);
    int getTimeDiffMillis(Time& _t, bool _utc=false);
    void setPanelWidth(float _pctOfWidth); //0.0 to 1.0 percent of width
    void addArrival(int _id, ofxJSONElement _a);
    void addDeparture(int _id, ofxJSONElement _d);
    void setActiveArrival(int _id);
    void setActiveDeparture(int _id);
    void clearDepartures();
    void clearArrivals();
    void removeArrival(int _id);
    void removeDeparture(int _id);
    void setWeatherString(string _w);
    void setClouds(string _c);
    void setKnots(string _k);
    void setPrecipitation(string _p);
    void setFlights(map<int, ofxJSONElement> _f);
    void addFlight(ofxJSONElement _f);
    void clearSatellites();
    void addSatellite(string _sat);
    void setBirdInFlight(string _b);
    void clearBirds();
    void addBird(ofxJSONElement _b);
    string getDayPart();
    float getDayPercent();
    void toggleGui(){bGuiOn = !bGuiOn;}
    bool getGuiOn(){return bGuiOn;}
    void setGui(bool _g){bGuiOn = _g;}
    int daysSinceNewMoon();
    void setVisible(bool _v);
    void toggleVisible();
    bool getVisible(){return bVisible;}
private:
    void getSunriseSunset();
    void setSunrise();
    void setSunset();
    void getAirportDetails();
    int getTimeComponentFromString(string _t, string _c);
    string getTimeString(string _t);
    string getAMPMFromString(string _t);
    void getGmtOffset();
    void calculateDay();
    void setupTexts();
    void updateTexts();
    void drawTexts(float _alpha);
    string capitalizeFirstLetter(string _s);
    ofTrueTypeFont panelFont;
    int panelWidth;
    string sunUrl = "http://api.sunrise-sunset.org/json";
    string baseUrl;
    string airport;
    Time sunrise;
    Time sunset;
    ofxJSONElement airportDetails;
    string currentTimezone;
    ofxJSONElement sunDetails;
    map<int, ofxJSONElement> arrivals;
    map<int, ofxJSONElement> departures;
    vector<ofxJSONElement> activeBirds;
    vector<TextBlock> infoTexts;
    int activeArrival;
    int activeDeparture;
    string weatherString = "";
    string timezoneApiBaseUrl = "http://api.timezonedb.com/v2/list-time-zone?key=ROTQ83OH4VG4&format=json&by=zone&zone=";
    int currentGmtOffset = 0;
    string dayPart;
    float dayPercent;
    bool bSunriseSet = false;
    bool bSunsetSet = false;
    bool bGuiOn = false;
    ofxJSONElement timeZones;
    TextBlock airportText;
    TextBlock timeText;
    TextBlock weatherText;
    TextBlock departingText;
    TextBlock arrivingText;
    TextBlock satellitesText;
    TextBlock birdsInFlightText;
    TextBlock birdsSightedText;
    string clouds;
    string knots;
    string precipitation;
    map<int, ofxJSONElement> flights;
    map<string, string> cloudMap = {
        {"BKN", "BROKEN CLOUDS"},
        {"SCT", "SCATTERED CLOUDS"},
        {"FEW", "FEW CLOUDS"},
        {"OVC", "OVERCAST"}
    };
    bool bVisible = false;
    int visibleStartTime;
    int visibleStopTime;
};


#endif /* defined(__flight__SystemInfo__) */
