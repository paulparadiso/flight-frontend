
//
//  FlightLauncher.cpp
//  flight
//
//  Created by Paul Paradiso on 4/11/16.
//
//

#include "FlightLauncher.h"

Airplane::Airplane(){
    
}

Airplane::Airplane(int _direction, string _type, bool _sat){
    stringstream path;
    if(!_sat){
        path << "animations/airplanes/";
        if(_type == "CNA"){
            path << "L/CNA/images_0";
        } else {
            path << "H/";
            path << _type;
            path << "/images_0";
        }
    } else {
        path << "animations/satellites/";
        path << _type;
        path << "/images_0";
    }
    direction = _direction;
    if(_sat){
        ao.loadFromDirectory(path.str(), true);
        ao.reset();
        ao.setStartY(0.2);
        ao.setEndY(0.2);
        ao.setSpeed(2.0);
    } else {
        ao.loadFromDirectory(path.str(), false);
        ao.setDirection(direction);
        float rScale = (ofRandom(1.0));
        //ao.addSettings("scale", rScale, "mix");
        //ao.setMirrored(true);
        ao.reset();
        ao.setMirrored(true);
        ao.setScale(rScale);
        float speed = 100.0 + ofRandom(100.0);
        if(direction == OBJECT_DIRECTION_LEFT){
            ao.setStartY(0.5);
            ao.setEndY(0.75);
            //ao.setSpeed(speed);
            //ao.setRotation(30.0);
        } else {
            ao.setStartY(0.5);
            ao.setEndY(0.75);
            ao.setSpeed(speed);
            //ao.setRotation(-30.0);
        }
        string d = "arrivals";
        if(_direction == OBJECT_DIRECTION_LEFT){
            d = "departures";
        }
        loadConfig(d);
    }
    bFlying = true;
}

void Airplane::loadConfig(string _dir){
    stringstream configPath;
    configPath << "animations/airplanes/config/" << _dir << ".json";
    ofxJSONElement config;
    config.open(configPath.str());
    int rotation = config["params"]["rotation"].asInt();
    cout << "Setting rotation for " << _dir << " to " << rotation << endl;
    ao.setRotation(float(rotation));
}

Airplane::Airplane(ofxJSONElement _data){
    
}

Airplane::~Airplane(){
    
}

void Airplane::update(){
    ao.update();
    //if((direction == OBJECT_DIRECTION_RIGHT && (ao.getPosition().x > (ofGetWidth() + ao.getWidth())))
    //   || (direction == OBJECT_DIRECTION_LEFT && (ao.getPosition().y < -(ao.getWidth())))
    //   ){
    ofVec2f p = ao.getPosition();
    //cout << "position = " << p.x << ", " << p.y << endl;
    if(ao.isDone()){
        //cout << "Turning off flight." << endl;
        bFlying = false;
    }
}

void Airplane::draw(){
    ao.draw();
}

bool Airplane::isFlying(){
    return bFlying;
}

void Airplane::launch(){
    
}

void Airplane::loadData(ofxJSONElement _data){
    
}

string Airplane::getPath(){
    
}

string Airplane::getId(){
    return id;
}

void Airplane::setId(string _id){
    id = _id;
}

int Airplane::getDirection(){
    return direction;
}

FlightLauncher::FlightLauncher(){
    setRandomLaunch();
}

FlightLauncher::~FlightLauncher(){
    
}

void FlightLauncher::urlResponse(ofHttpResponse & response){
    
}

void FlightLauncher::checkRandomLaunch(){
    int now = ofGetElapsedTimeMillis();
    if(now > randomLaunch){
        float d = ofRandom(1.0);
        int fDir = OBJECT_DIRECTION_RIGHT;
        if(d > 0.5){
            fDir = OBJECT_DIRECTION_LEFT;
        }
        string pType = "747";
        int pChooser = int(ofRandom(airplaneTypes.size())) - 1;
        if(pChooser < 0){
            pChooser = 0;
        }
        if(pChooser > (airplaneTypes.size() - 1)){
            pChooser = airplaneTypes.size() - 1;
        }
        string pTypeStr = airplaneTypes[pChooser];
        /*
         if(fDir == OBJECT_DIRECTION_RIGHT){
         cout << "Creating an arrival of type " << pTypeStr << endl;
         } else {
         cout << "Creating a departure of type " << pTypeStr << endl;
         }
         */
        airplanes.push_back(Airplane(fDir, pTypeStr));
        setRandomLaunch();
    }
}

void FlightLauncher::checkTimedLaunch(){
    int now = ofGetElapsedTimeMillis();
    for(auto tIter = timedLaunches.begin(); tIter != timedLaunches.end();){
        if(now > tIter->first){
            launchFlight(tIter->second);
            tIter = timedLaunches.erase(tIter);
            if(sysInfo != nullptr){
                sysInfo->setFlights(timedLaunches);
            }
        } else {
            tIter++;
        }
    }
}

void FlightLauncher::launchFlight(ofxJSONElement _f){
    cout << "Launching " << _f["direction"].asString() << " from " << _f["local"].asString() << " to " << _f["remote"].asString() << endl;
    string id = _f["faFlightID"].asString();
    bool bArrival = true;
    if(_f["direction"].asString() != "arrivals"){
        bArrival = false;
    }
    int fDir = OBJECT_DIRECTION_LEFT;
    if(bArrival){
        fDir = OBJECT_DIRECTION_RIGHT;
    }
    string pType = "747";
    airplanes.push_back(Airplane(fDir, pType));
    airplanes.back().setId(id);
}

void FlightLauncher::launchSatellite(){
    if(satelliteNames.size() > 0){
        cout << "Launching Satellite." << endl;
        int random = int(ofRandom(satelliteNames.size() - 1));
        if(random < 0){
            random = 0;
        }
        if(random > satelliteNames.size() - 1){
            random = satelliteNames.size() - 1;
        }
        string satellite = satelliteNames[random];
        stringstream satPath;
        satPath << "animations/satellites/" << satellite << "/images_0";
        ofDirectory tDir;
        tDir.listDir(satPath.str());
        if(tDir.exists()){
            float randomSel = ofRandom(1.0);
            int dir = OBJECT_DIRECTION_LEFT;
            if(randomSel > 0.5){
                dir = OBJECT_DIRECTION_RIGHT;
            }
            satellites.push_back(Airplane(dir, satellite, true));
        }
    }
}

void FlightLauncher::launchUfo(){
    if(ufos.size() > 0){
        cout << "Launching Ufo." << endl;
        int random = int(ofRandom(satelliteNames.size() - 1));
        if(random < 0){
            random = 0;
        }
        if(random > satelliteNames.size() - 1){
            random = satelliteNames.size() - 1;
        }
    }
}

void FlightLauncher::clear(){
    airplanes.clear();
    sysInfo->clearArrivals();
    sysInfo->clearDepartures();
}

void FlightLauncher::update(){
    for(auto aIter = airplanes.begin(); aIter != airplanes.end();){
        if(aIter->isFlying()){
            aIter->update();
            ++aIter;
        } else {
            if(aIter->getDirection() == OBJECT_DIRECTION_RIGHT){
                sysInfo->setActiveArrival(-1);
                sysInfo->removeArrival(0);
            } else {
                sysInfo->setActiveDeparture(-1);
                sysInfo->removeDeparture(0);
            }
            aIter = airplanes.erase(aIter);
        }
    }
    for(auto sIter = satellites.begin(); sIter != satellites.end();){
        if(sIter->isFlying()){
            sIter->update();
            ++sIter;
        } else {
            sIter = satellites.erase(sIter);
            if(satellites.size() == 0){
                lastSatelliteLaunch = ofGetElapsedTimeMillis();
            }
        }
    }
    if(bHaveNewFlight){
        launchFlight(nextFlight);
        bHaveNewFlight = false;
    }
    if(satellites.size() == 0){
        if(ofGetElapsedTimeMillis() > lastSatelliteLaunch + SATELLITE_DELAY){
            launchSatellite();
        }
    }
    if(bLive){
        checkTimedLaunch();
    } else {
        checkRandomLaunch();
    }
}

Time FlightLauncher::stripFlightTime(string _time){
    Time t;
    auto tokens = ofSplitString(_time, " ");
    if(tokens.size() > 1){
        auto timeTokens = ofSplitString(tokens[1], ":");
        if(timeTokens.size() > 1){
            t.setHours(ofToInt(timeTokens[0]));
            t.setMinutes(ofToInt(timeTokens[1]));
            t.setUtcHours(ofToInt(timeTokens[0]));
            t.setUtcMinutes(ofToInt(timeTokens[1]));
        }
    }
    return t;
}

void FlightLauncher::update(string _data){
    cout << "FlightLauncher received." << _data << endl;
    ofxJSONElement data;
    data.parse(_data);
    if(data.isArray()){
        auto names = data[0].getMemberNames();
        for (auto const& id : data[0].getMemberNames()) {
            if(id == "satellite"){
                loadSatellites(data);
                return;
            }
        }
    }
    if(data["remote"].asString() == ""){
        return;
    }
    if(data["direction"] == "arrivals"){
        if(_data == lastArrival){
            return;
        } else {
            lastArrival = _data;
        }
    } else {
        if(_data == lastDeparture){
            return;
        } else {
            lastDeparture = _data;
        }
    }
    //launchFlight(data);
    nextFlight = data;
    bHaveNewFlight = true;
    sysInfo->addFlight(data);
    return;
    Time currentTime = sysInfo->getCurrentTime();
    string direction = data["content"].asString();
    //cout << "Input direction = " << direction << endl;
    for(auto fIter = data["data"].begin(); fIter != data["data"].end(); fIter++){
        //cout << "adding flight - " << (*fIter)["flight_id"].asString() << endl;
        if(!(*fIter)["time"].isNull()){
            Time flightTime = stripFlightTime((*fIter)["time"].asString());
            //cout << "Current time = " << currentTime.getUtcHours() << ":" << currentTime.getUtcMinutes() << ". Flight arrives at " << flightTime.getHours() << ":" << flightTime.getMinutes() << "." << endl;
            int flightDelay = sysInfo->getTimeDiffMillis(flightTime, true);
            if(flightDelay > 0){
                bool bFlightDup = false;
                for(auto lIter = timedLaunches.begin(); lIter != timedLaunches.end(); lIter++){
                    if(lIter->second["flightId"] == (*fIter)["flight_id"]){
                        bFlightDup = true;
                    }
                }
                if(bFlightDup){
                    continue;
                } else {
                    bool bArrival = true;
                    if(direction == "departures"){
                        bArrival = false;
                    }
                    //cout << "Displaying flight " <<  (*fIter)["flight_id"].asString() << " in " << flightDelay / 60000 << " minutes." << endl;
                    int launchTime = ofGetElapsedTimeMillis() + flightDelay;
                    if(bArrival){
                        (*fIter)["direction"] = "arrival";
                        //sysInfo->addArrival(launchTime, *fIter);
                    } else {
                        (*fIter)["direction"] = "departure";
                        //sysInfo->addDeparture(launchTime, *fIter);
                    }
                    timedLaunches[launchTime] = *fIter;
                }
            } else {
                //cout << "Not displaying flight from " << flightDelay << " millis ago." << endl;
                //launchFlight(*fIter);
            }
        } else {
            
        }
        //cout << "adding flight " << (*fIter)["flightId"].asString() << endl;
    }
    if(sysInfo != nullptr){
        sysInfo->setFlights(timedLaunches);
    }
}

void FlightLauncher::draw(){
    for(auto aIter = airplanes.begin(); aIter != airplanes.end(); aIter++){
        if(aIter->isFlying()){
            aIter->draw();
        }
    }
    for(auto sIter = satellites.begin(); sIter != satellites.end(); sIter++){
        if(sIter->isFlying()){
            sIter->draw();
        }
    }
}

string FlightLauncher::urlEncode(const string &value){
    
}

void FlightLauncher::setRandomLaunch(){
    int r = ofRandom(RANDOM_LAUNCH_MAX);
    randomLaunch = ofGetElapsedTimeMillis() + r;
    //cout << "Launching random flight in " << r / 1000 << " seconds." << endl;
}

void FlightLauncher::setLive(bool _l){
    bLive = _l;
}

void FlightLauncher::loadSatellites(ofxJSONElement _data){
    sysInfo->clearSatellites();
    for(auto dIter = _data.begin(); dIter != _data.end(); dIter++){
        cout << "Loading satellite - " << (*dIter)["satellite"].asString() << endl;
        sysInfo->addSatellite((*dIter)["satellite"].asString());
        string capitalized = (*dIter)["satellite"].asString();
        ofStringReplace(capitalized, " ", "");
        satelliteNames.push_back(ofToLower(capitalized));
    }
}