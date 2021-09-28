//
//  FlightLauncher.h
//  flight
//
//  Created by Paul Paradiso on 4/11/16.
//
//

#ifndef __flight__FlightLauncher__
#define __flight__FlightLauncher__

#include <stdio.h>
#include "ofMain.h"
#include "AnimatedObject.h"
#include "DataClient.h"
#include "ofxJSON.h"
#include "SystemInfo.h"

#define RANDOM_LAUNCH_MAX 20000
#define SATELLITE_DELAY 12 * 60 * 60 * 1000

class Airplane{
public:
    Airplane();
    Airplane(int _direction, string _type, bool _sat=false);
    Airplane(ofxJSONElement _data);
    void loadConfig(string _dir);
    ~Airplane();
    void update();
    void draw();
    bool isFlying();
    void launch();
    //void loadData(string _data);
    void loadData(ofxJSONElement _data);
    string getPath();
    string getId();
    void setId(string _id);
    int getDirection();
private:
    bool bFlying = false;
    string id;
    AnimatedObject ao;
    string path = "DEFAULT";
    int direction;
};

class FlightLauncher: public DataClient{
public:
    FlightLauncher();
    ~FlightLauncher();
    void urlResponse(ofHttpResponse & response);
    void update();
    void update(string _data);
    void draw();
    void clear();
    string urlEncode(const string &value);
    void setRandomLaunch();
    void setLive(bool _l);
    void launchFlight(ofxJSONElement _f);
private:
    Time stripFlightTime(string _time);
    void launchSatellite();
    void launchUfo();
    void checkRandomLaunch();
    void checkTimedLaunch();
    void loadSatellites(ofxJSONElement _data);
    vector<Airplane> airplanes;
    vector<Airplane> satellites;
    vector<Airplane> ufos;
    map<int, ofxJSONElement> timedLaunches;
    int randomLaunch;
    vector<string> airplaneTypes = {"707","747","767","777","787","CNA"};
    bool bLive = true;
    ofxJSONElement nextFlight;
    vector<string> satelliteNames;
    bool bHaveNewFlight;
    string lastArrival;
    string lastDeparture;
    int lastSatelliteLaunch = 0;
    Airplane ufo;
    string ufoPath = "animations/ufos/cigar/images_0";
};

#endif /* defined(__flight__FlightLauncher__) */
