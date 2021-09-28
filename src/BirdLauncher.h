//
//  BirdLauncher.h
//  flight
//
//  Created by Paul Paradiso on 4/4/16.
//
//

#ifndef __flight__BirdLauncher__
#define __flight__BirdLauncher__

#include <stdio.h>
#include "ofMain.h"
#include "AnimatedObject.h"
#include "DataClient.h"
#include "ofxJSON.h"
#include "SystemInfo.h"

#define MAX_RANDOM_TIME 60000

#define RANDOM_LAUNCH_BASE 1000
#define RANDOM_LAUNCH_ADD 1000

#define COMMON_BIRD_DELAY 600000
#define BAT_DELAY 5000
#define UFO_DELAY 2 * 60 * 60 * 1000

//70,000 babies born in MN in 2015. ~8 per hour.
#define BIRTH_DELAY 450000
//define BIRTH_DELAY 10000

//43,050 babies botn in MN in 2016. ~5 per hour.
#define DEATH_DELAY 720000

class Bird{
public:
    Bird();
    Bird(ofxJSONElement _data);
    ~Bird();
    void update();
    void draw();
    bool isFlying();
    void launch();
    //void loadData(string _data);
    void loadData(ofxJSONElement _data);
    void loadData(string _path);
    void loadFromDirectory(string _path);
    string getPath();
    void setName(string _n){name = _n;}
    string getName(){return name;}
private:
    int displayDelay;
    int lastLaunch;
    bool bFlying = false;
    AnimatedObject ao;
    string path = "DEFAULT";
    ofxJSONElement settings;
    string name;
};

class BirdLauncher: public DataClient{
public:
    BirdLauncher();
    ~BirdLauncher();
    void urlResponse(ofHttpResponse & response);
    void update();
    void update(string _data);
    void draw();
    void clear();
    void loadFromFile(string _f);
    void loadBirds(string _birds);
    string urlEncode(const string &value);
private:
    string resolvePath(string _com, string _sci, string _order, string _family, string _genus);
    void loadBats();
    void loadCommonBirds();
    void loadUfos();
    void setRandomLaunch();
    void setCommonLaunch();
    void setUfoLaunch();
    void launchCommonBird();
    void launchBat();
    void launchUfo();
    void launchBirth();
    void launchDeath();
    vector<Bird> birds;
    vector<Bird> commonBirds;
    vector<Bird> bats;
    vector<Bird> ufos;
    Bird birth;
    Bird death;
    bool bHaveNewBirds = false;
    string newBirds;
    bool bLoading = false;
    int randomLaunch;
    int commonLaunch;
    int ufoLaunch;
    vector<string> errors;
    int numSightings;
    vector<int> birdSelector;
    string lastUpdate;
    int lastCommonBird = 0;
    int lastUfo = 0;
    int lastBirth = 0;
    int lastDeath = 0;
    //bool commonBirdActive = false;
    //ofVideoPlayer vid;
};

#endif /* defined(__flight__BirdLauncher__) */
