//
//  SystemInfo.cpp
//  flight
//
//  Created by Paul Paradiso on 4/7/16.
//
//

#include "SystemInfo.h"

Time::Time(){
    
}

Time::~Time(){
    
}

void Time::setHours(int _h){
    hours = _h;
}

void Time::setMinutes(int _m){
    minutes = _m;
}

void Time::setUtcHours(int _h){
    utcHours = _h;
}

void Time::setUtcMinutes(int _m){
    utcMinutes = _m;
}

int Time::getHours(){
    return hours;
}
    
int Time::getMinutes(){
    return minutes;
}

int Time::getUtcHours(){
    return utcHours;
}

int Time::getUtcMinutes(){
    return utcMinutes;
}

SystemInfo::SystemInfo(){
    setPanelWidth(DEFAULT_PANEL_WIDTH);
    ofRegisterURLNotification(this);
    panelFont.load("fonts/frabk.ttf", 12);
    timeZones.open("animations/data/time_zones_simple.json");
    currentTimezone = "America/New_York";
    getGmtOffset();
    setupTexts();
}

SystemInfo::~SystemInfo(){
    
}

void SystemInfo::urlResponse(ofHttpResponse & response){
    if(response.status == 200){
        if(response.request.name == "airport"){
            cout << "Setting airport details." << endl;
            airportDetails.parse(response.data);
            getSunriseSunset();
            currentTimezone = airportDetails["data"]["timezone"].asString();
            ofLoadURLAsync(timezoneApiBaseUrl + currentTimezone, "gmt");
            cout << "Set timezone to " << currentTimezone << endl;
        }
        if(response.request.name == "sun"){
            sunDetails.parse(response.data);
            //cout << "SunDetails = " << sunDetails.getRawString() << endl;
            if(sunDetails["status"].asString() != "INVALID_REQUEST"){
                setSunrise();
                setSunset();
            }	
        }
        if(response.request.name == "gmt"){
            ofxJSONElement d;
            d.parse(response.data);
            cout << "Timezone data = " << d.getRawString() << endl;
            currentGmtOffset = ofToInt(d["zones"][0]["gmtOffset"].asString());
            cout << "set gmt offset to " << currentGmtOffset << " for " << currentTimezone << endl;
        }
    }
}

void SystemInfo::init(){
    getAirportDetails();
}

void SystemInfo::setBaseUrl(string _b){
    baseUrl = _b;
}

int SystemInfo::getTimeComponentFromString(string _t, string _c){
    auto sTokens = ofSplitString(_t, " ");
    string ampm = "am";
    int hours = 0;
    int minutes = 0;
    auto tTokens = ofSplitString(sTokens[0], ":");
    if(tTokens.size() > 2){
        hours = ofToInt(tTokens[0]);
        if(ampm == "PM"){
            hours += 12;
        }
        minutes = ofToInt(tTokens[1]);
    }
    if(_c == "hours"){
        return hours;
    } else {
        return minutes;
    }
}

string SystemInfo::getAMPMFromString(string _t){
    auto sTokens = ofSplitString(_t, " ");
    string ampm = "am";
    if(sTokens.size() > 1){
        cout << "Got token = " << sTokens[1] << endl;
        if(sTokens[1] == "PM"){
            ampm = "pm";
        }
    } else {
        cout << "Only 1 token." << endl;
    }
    return ampm;
}

void SystemInfo::setSunrise(){
    string rise = sunDetails["results"]["sunrise"].asString();
    //cout << "Sunrise - " << rise << endl;
    int hours = getTimeComponentFromString(rise, "hours");
    string ampm = getAMPMFromString(sunDetails["results"]["sunrise"].asString());
    if(ampm == "pm"){
        hours += 12;
    }
    if(ampm == "am" && hours == 12){
        hours = 0;
    }
    hours = hours + (currentGmtOffset / 3600);
    //hours = hours % 24;
    if(hours < 0){
        hours = 24 - hours;
    }
    int minutes = getTimeComponentFromString(rise, "minutes");
    sunrise.setHours(hours);
    sunrise.setMinutes(minutes);
    cout << "Sunrise - " << sunrise.getHours() << ":" << sunrise.getMinutes() << endl;
    bSunriseSet = true;
}

void SystemInfo::setSunset(){
    string set = sunDetails["results"]["sunset"].asString();
    int hours = getTimeComponentFromString(set, "hours");
    string ampm = getAMPMFromString(sunDetails["results"]["sunset"].asString());
    if(ampm == "pm"){
        hours += 12;
    }
    if(ampm == "am" && hours == 12){
        hours = 0;
    }
    hours = hours + (currentGmtOffset / 3600);
    //hours = hours % 24;
    if(hours < 0){
        hours = 24 - hours;
    }
    int minutes = getTimeComponentFromString(set, "minutes");
    sunset.setHours(hours);
    sunset.setMinutes(minutes);
    cout << "Sunset - " << sunset.getHours() << ":" << sunset.getMinutes() << endl;
    bSunsetSet = true;
}

Time SystemInfo::getSunrise(){
    return sunrise;
}

Time SystemInfo::getSunset(){
    return sunset;
}

void SystemInfo::getSunriseSunset(){
    cout << "Getting Sunrise/Sunset for " << airport;
    string latitude = airportDetails["data"]["latitude"].asString();
    string longitude = airportDetails["data"]["longitude"].asString();
    stringstream urlGen;
    urlGen << sunUrl;
    urlGen << "?lat=" << latitude;
    urlGen << "&lng=" << longitude;
    urlGen << "&date=today";
    cout << urlGen.str() << endl;
    ofLoadURLAsync(urlGen.str(), "sun");
}

void SystemInfo::calculateDay(){
    Time now = getCurrentTime();
    //cout << "Current time = " << now.getHours() << ":" << now.getMinutes() << endl;
    int nowMinutes = now.getHours() * 60 + now.getMinutes();
    int sunriseMinutes = sunrise.getHours() * 60 + sunrise.getMinutes();
    int sunsetMinutes = sunset.getHours() * 60 + now.getMinutes();
    //cout << "nowMinutes = " << nowMinutes << ", sunriseMinutes = " << sunriseMinutes << ", sunsetMinutes = " << sunsetMinutes << endl;
    if(nowMinutes > sunriseMinutes && nowMinutes < sunsetMinutes){
        dayPart = "day";
        int dayDiff = sunsetMinutes - sunriseMinutes;
        int dayProgress = nowMinutes - sunriseMinutes;
        dayPercent = (float)dayProgress / (float)dayDiff;
    } else {
        dayPart = "night";
        int nightDiff = sunriseMinutes + (24 * 60) - sunsetMinutes;
        if(nowMinutes < sunsetMinutes){
            nowMinutes += 24 * 60;
        }
        int nightProgress = nowMinutes - sunsetMinutes;
        dayPercent = (float)nightProgress / (float)nightDiff;
    }
    //cout << "Set daypart to " << dayPart << " nowMinutes = " << nowMinutes << ", sunriseMinutes = " << sunriseMinutes << ", sunsetMinutes = " << sunsetMinutes << endl;
    //cout << "Set day percent to " << dayPercent << endl;
}

void SystemInfo::setupTexts(){
    float scale = ofGetHeight() / 1080.f;
    int xPos = 20 * scale;
    
    airportText.setup("AIRPORT", xPos, 40 * scale);
    if(DISPLAY_TEST_TEXT){
        airportText.addBodyString("IST");
    }
    
    timeText.setup("TIME", xPos, 100 * scale);
    if(DISPLAY_TEST_TEXT){
        timeText.addBodyString("11:20");
    }
    
    weatherText.setup("WEATHER", xPos, 160 * scale);
    if(DISPLAY_TEST_TEXT){
        weatherText.addBodyString("PARTLY CLOUDY");
        weatherText.addBodyString("WIND: 3 KNOTS");
        weatherText.addBodyString("LIGHT RAIN");
    }
        
    departingText.setup("DEPARTING FLIGHTS", xPos, 282 * scale);
    if(DISPLAY_TEST_TEXT){
        departingText.addBodyString("BOS - 11:21");
        departingText.addBodyString("NIC - 11:22");
        departingText.addBodyString("JFK - 11:23");
        departingText.addBodyString("LHR - 11:27");
        departingText.addBodyString("ALG - 11:41");
    }
    
    arrivingText.setup("ARRIVING FLIGHTS", xPos, 470 * scale);
    if(DISPLAY_TEST_TEXT){
        arrivingText.addBodyString("BOS - 11:21");
        arrivingText.addBodyString("BOS - 11:23");
        arrivingText.addBodyString("BOS - 11:27");
        arrivingText.addBodyString("BOS - 11:28");
        arrivingText.addBodyString("BOS - 11:31");
    }
    
    satellitesText.setup("SATELLITES", xPos, 658 * scale);
    satellitesText.setLimit(2);
    if(DISPLAY_TEST_TEXT){
        satellitesText.addBodyString("Nikor-11");
        satellitesText.addBodyString("Boxian");
    }
    
    birdsInFlightText.setup("BIRD IN FLIGHT", xPos, 750 * scale);
    if(DISPLAY_TEST_TEXT){
        birdsInFlightText.addBodyString("Crested crane");
    }
    
    birdsSightedText.setup("RECENT BIRDS SIGHTED / (SIGHTINGS)", xPos, 810 * scale);
    birdsSightedText.setLimit(10);
    if(DISPLAY_TEST_TEXT){
        birdsSightedText.addBodyString("Crested crane (4)");
        birdsSightedText.addBodyString("Redwing blackbird (64)");
        birdsSightedText.addBodyString("Barn owl (1)");
        birdsSightedText.addBodyString("Magpie (3)");
        birdsSightedText.addBodyString("Flamingo (23)");
        birdsSightedText.addBodyString("Crested crane (4)");
        birdsSightedText.addBodyString("Redwing blackbird (64)");
        birdsSightedText.addBodyString("Barn owl (1)");
        birdsSightedText.addBodyString("Magpie (3)");
        birdsSightedText.addBodyString("Flamingo (23)");
    }
}

void SystemInfo::updateTexts(){
    timeText.clearBody();
    timeText.addBodyString(getCurrentTimeString()/* + " " + dayPart + " " + ofToString(dayPercent)*/);
    weatherText.clearBody();
    weatherText.addBodyString(clouds);
    weatherText.addBodyString(knots);
    weatherText.addBodyString(precipitation);
}

string SystemInfo::getDayPart(){
    if(bSunriseSet && bSunsetSet){
        calculateDay();
        return dayPart;
    } else {
        return "day";
    }
}

float SystemInfo::getDayPercent(){
    if(bSunriseSet && bSunsetSet){
        calculateDay();
        return dayPercent;
    } else {
        return 0.0;
    }
}

void SystemInfo::getAirportDetails(){
    cout << "Getting details for " << airport << endl;
    stringstream urlGen;
    urlGen << baseUrl;
    urlGen << "airport/";
    urlGen << airport;
    cout << urlGen.str() << endl;
    ofLoadURLAsync(urlGen.str(), "airport");
}

void SystemInfo::setAirport(string _a){
    airport = _a;
    getAirportDetails();
    airportText.clearBody();
    airportText.addBodyString(airport);
}

string SystemInfo::getAirport(){
    return airport;
}

void SystemInfo::getGmtOffset(){
    //ofLoadURLAsync(timezoneApiBaseUrl + airportDetails["timeZoneRegionName"].asString(), "gmt");
    currentGmtOffset = ofToInt(timeZones[currentTimezone].asString());
}

string SystemInfo::capitalizeFirstLetter(string _s){
    string s = ofToLower(_s);
    char l = s[0];
    string upper = ofToUpper(string(&l));
    upper.resize(1);
    s.replace(0,1, upper);
    return s;
}

string SystemInfo::getTimeString(string _t){
    auto tokens = ofSplitString(_t, " ");
    string outString = "00:00";
    if(tokens.size() > 1){
        auto tTokens = ofSplitString(tokens[1], ":");
        if(tTokens.size() > 1){
            int hours = ofToInt(tTokens[0]);
            int minutes = ofToInt(tTokens[1]);
            stringstream out;
            if(hours < 10){
                out << "0";
            }
            out << hours;
            out << ":";
            if(minutes < 10){
                out << "0";
            }
            out << minutes;
            outString = out.str();
        }
    }
    return outString;
}

void SystemInfo::setClouds(string _c){
    if(cloudMap.find(_c) != cloudMap.end()){
        clouds = cloudMap[_c];
    } else {
        clouds = "";
    }
    cout << "Setting SysInfo clouds to " << clouds << endl;
}

void SystemInfo::setKnots(string _k){
    knots = "WIND: " + _k + " KNOTS";
}

void SystemInfo::setPrecipitation(string _p){
    precipitation = _p;
}

void SystemInfo::setFlights(map<int, ofxJSONElement> _f){
    cout << "Adding flights." << endl;
    arrivingText.clearBody();
    departingText.clearBody();
    for(auto fIter = _f.begin(); fIter != _f.end(); fIter++){
        //cout << fIter->second["direction"].asString() << endl;
        if(fIter->second["remote_airport"].asString() == airport){
            //cout << "\tAddding " << fIter->second["remote_airport"].asString() << " to departures." << endl;
            departingText.addBodyString(fIter->second["remote_airport"].asString() + " - " + getTimeString(fIter->second["time"].asString()));
        } else {
            //cout << "\tAddding " << fIter->second["remote_airport"].asString() << " to arrivals." << endl;
            arrivingText.addBodyString(fIter->second["remote_airport"].asString() + " - " + getTimeString(fIter->second["time"].asString()));
        }
    }
}

void SystemInfo::addFlight(ofxJSONElement _f){
    Time t = getCurrentTime();
    stringstream tStream;
    if(t.getHours() < 10){
        tStream << "0";
    }
    tStream << t.getHours();
    tStream << ":";
    if(t.getMinutes() < 10){
        tStream << "0";
    }
    tStream << t.getMinutes();
    if(_f["direction"].asString() == "arrivals"){
        arrivingText.addBodyString(_f["remote"].asString() + " - " + tStream.str(), true);
    } else {
        departingText.addBodyString(_f["remote"].asString() + " - " + tStream.str(), true);
    }
}

void SystemInfo::addSatellite(string _sat){
    satellitesText.addBodyString(_sat);
}

void SystemInfo::clearSatellites(){
    satellitesText.clearBody();
}

void SystemInfo::setBirdInFlight(string _b){
    birdsInFlightText.clearBody();
    birdsInFlightText.addBodyString(capitalizeFirstLetter(_b));
}

void SystemInfo::draw(){
    if(!bVisible && ofGetElapsedTimeMillis() > visibleStopTime + FADE_IN){
        return;
    }
    float alpha = 255.f;
    if(bVisible && ofGetElapsedTimeMillis() < visibleStartTime + FADE_IN){
        alpha = 255.f * (ofGetElapsedTimeMillis() - visibleStartTime) / float(FADE_IN);
        //alpha = 100.f;
    }
    if(!bVisible && ofGetElapsedTimeMillis() < visibleStopTime + FADE_IN){
        alpha = 255.f * (1.0 - ((ofGetElapsedTimeMillis() - visibleStopTime) / float(FADE_IN)));
        //alpha = 100.f;
    }
    updateTexts();
    ofSetColor(255,255,255,128 * (alpha / 255.0));
    ofDrawRectangle(0, 0, panelWidth, ofGetHeight());
    ofSetColor(0,0,0,255);
    drawTexts(alpha);
    /*
    ofSetColor(255,255,255,128);
    ofDrawRectangle(0, 0, panelWidth, ofGetHeight());
    ofSetColor(0,0,0,255);
    stringstream output;
    output << "AIPORT:\n";
    output << airport << "\n";
    output << "\n";
    output << "CURRENT TIME:\n";
    output << getCurrentTimeString() << "\n";
    output << "\n";
    output << "CURRENT WEATHER:\n";
    output << weatherString << "\n";
    output << "\n";
    output << "ARRIVING:\n";
    int count = 0;
    for(auto aIter = arrivals.begin(); aIter != arrivals.end(); aIter++){
        if(count > 5){
            break;
        }
        output << "FLIGHT from ";
        output << aIter->second["remote_airport"].asString();
        output << " - " << getTimeString(aIter->second["time"].asString());
        output << "\n";
        count++;
    }
    output << "\n";
    output << "DEPARTING:\n";
    count = 0;
    for(auto dIter = departures.begin(); dIter != departures.end(); dIter++){
        if(count > 5){
            break;
        }
        output << "FLIGHT to ";
        output << dIter->second["remote_airport"].asString();
        output << " - " << getTimeString(dIter->second["time"].asString());
        output << "\n";
        count++;
    }
    output << "\n";
    output << "BIRDS:\n";
    for(auto bIter = activeBirds.begin(); bIter != activeBirds.end(); bIter++){
        output << (*bIter)["com_name"].asString() <<  " - " << (*bIter)["how_many"].asInt() << "\n";
    }
    output << "\n";
    panelFont.drawString(output.str(), 20, 40);
    ofSetColor(255, 255, 255);
    */
}

void SystemInfo::drawTexts(float _alpha){
    airportText.setBodyColor(ofColor(0,0,0,_alpha));
    airportText.setHeaderColor(ofColor(0,0,0,_alpha));
    airportText.draw();
    timeText.setBodyColor(ofColor(0,0,0,_alpha));
    timeText.setHeaderColor(ofColor(0,0,0,_alpha));
    timeText.draw();
    weatherText.setBodyColor(ofColor(0,0,0,_alpha));
    weatherText.setHeaderColor(ofColor(0,0,0,_alpha));
    weatherText.draw();
    departingText.setBodyColor(ofColor(0,0,0,_alpha));
    departingText.setHeaderColor(ofColor(0,0,0,_alpha));
    departingText.draw();
    arrivingText.setBodyColor(ofColor(0,0,0,_alpha));
    arrivingText.setHeaderColor(ofColor(0,0,0,_alpha));
    arrivingText.draw();
    satellitesText.setBodyColor(ofColor(0,0,0,_alpha));
    satellitesText.setHeaderColor(ofColor(0,0,0,_alpha));
    satellitesText.draw();
    birdsInFlightText.setBodyColor(ofColor(0,0,0,_alpha));
    birdsInFlightText.setHeaderColor(ofColor(0,0,0,_alpha));
    birdsInFlightText.draw();
    birdsSightedText.setBodyColor(ofColor(0,0,0,_alpha));
    birdsSightedText.setHeaderColor(ofColor(0,0,0,_alpha));
    birdsSightedText.draw();
}

Time SystemInfo::getCurrentTime(){
    Time currentTime;
    //currentTime.setHours(ofGetHours());
    //currentTime.setMinutes(ofGetMinutes());
    time_t t;
    time_t u;
    struct tm * localTime;
    struct tm * utcTime;
    //t = time(NULL) + currentGmtOffset;
    time(&u);
    //u -= 3600;
    //cout << "GMT OFFSET = " << currentGmtOffset << endl;
    utcTime = gmtime(&u);
    t = mktime(utcTime) + currentGmtOffset;
    localTime = localtime(&t);
    //cout << "ISDST = " << utcTime->tm_isdst << endl;
    //localTime = time(mktime(utcTime)) + currentGmtOffset;
    //localTime = utcTime;
    //localTime.tm_sec += currentGmtOffset;
    //cout << "UTC = " << utcTime->tm_hour << ":" << utcTime->tm_min << endl;
    currentTime.setHours(localTime->tm_hour);
    currentTime.setMinutes(localTime->tm_min);
    //currentTime.setHours(utcTime->tm_hour);
    //currentTime.setMinutes(utcTime->tm_min);
    currentTime.setUtcHours(utcTime->tm_hour);
    currentTime.setUtcMinutes(utcTime->tm_min);
    return currentTime;
}

int SystemInfo::daysSinceNewMoon(){
    
    struct tm newMoonTm = {0, 0, 0, 4, 5, 116};
    time_t now;
    time(&now);
    struct tm nowTm;
    nowTm = * gmtime(&now);
    
    /*
    time_t now;
    time(&now);
     
    struct tm newMoon;
    newMoon.tm_year = 116;
    newMoon.tm_mon = 6;
    newMoon.tm_mday = 4;
    newMoon.tm_hour = 6;
    newMoon.tm_min = 0;
    time_t diff = difftime(now, mktime(&newMoon));
    //cout << "Now = " << now << endl;
    //time_t difference = difftime(nowTm, newMoonTm) / (60 * 60 * 24);
    */
    
     //cout << "diff = " << diff << endl;
    double diff = difftime(now, mktime(&newMoonTm));
    return int(fmod(diff / 86400.f, 29.5));
    //return abs(fmod(difference, 29.5));
}

string SystemInfo::getCurrentTimeString(){
    Time currentTime = getCurrentTime();
    stringstream time;
    if(currentTime.getHours() < 10){
        time << "0";
    }
    time << currentTime.getHours();
    time << ":";
    if(currentTime.getMinutes() < 10){
        time << "0";
    }
    time << currentTime.getMinutes();
    return time.str();
}

int SystemInfo::getTimeDiffMillis(Time& _t, bool _utc){
    Time now = getCurrentTime();
    int hoursDiff;
    int minutesDiff;
    if(_utc){
        hoursDiff = _t.getHours() - now.getUtcHours();
        minutesDiff = _t.getMinutes() - now.getUtcMinutes();
    } else {
        hoursDiff = _t.getHours() - now.getHours();
        minutesDiff = _t.getMinutes() - now.getMinutes();
    }
    int totalMinutes = (hoursDiff * 60) + minutesDiff;
    return totalMinutes * 60000;
}

void SystemInfo::setPanelWidth(float _pctOfWidth){
    panelWidth = ofGetWidth() * _pctOfWidth;
}

void SystemInfo::addArrival(int _id, ofxJSONElement _a){
    arrivals[_id] = _a;
}

void SystemInfo::addDeparture(int _id, ofxJSONElement _d){
    departures[_id] = _d;
}

void SystemInfo::setActiveArrival(int _id){
    activeArrival = _id;
    cout << "Setting active arrival to " << activeArrival << endl;
}

void SystemInfo::setActiveDeparture(int _id){
    activeDeparture = _id;
}

void SystemInfo::clearArrivals(){
    arrivals.clear();
}

void SystemInfo::clearDepartures(){
    departures.clear();
}

void SystemInfo::removeArrival(int _id){
    /*
    for(auto aIter = arrivals.begin(); aIter != arrivals.end();){
        if(aIter->second["flightId"].asString() == _flightId){
            aIter = arrivals.erase(aIter);
        } else {
            aIter++;
        }
    }
    */
    if(arrivals.find(_id) != arrivals.end()){
        
    }
}

void SystemInfo::removeDeparture(int _id){
    /*
    for(auto dIter = departures.begin(); dIter != departures.end();){
        if(dIter->second["flightId"].asString() == _flightId){
            dIter = departures.erase(dIter);
        } else {
            dIter++;
        }
    }
    */
    if(departures.find(_id) != departures.end()){
        
    }
}

void SystemInfo::setWeatherString(string _w){
    weatherString = _w;
}

void SystemInfo::clearBirds(){
    /*
    if(activeBirds.size() > 0){
        activeBirds.clear();
    }
    */
    birdsSightedText.clearBody();
}

void SystemInfo::addBird(ofxJSONElement _b){
    //activeBirds.push_back(_b);
    string b = capitalizeFirstLetter(_b["com_name"].asString()) + " (" + _b["how_many"].asString() + ")";
    birdsSightedText.addBodyString(b);
    //cout << "SysInfo adding bird " << b << " from " << _b.getRawString() << endl;
}

void SystemInfo::toggleVisible(){
    if(bVisible){
        setVisible(false);
    } else {
        setVisible(true);
    }
}

void SystemInfo::setVisible(bool _v){
    bVisible = _v;
    if(bVisible){
        cout << "Setting visible start time." << endl;
        visibleStartTime = ofGetElapsedTimeMillis();
    } else {
        visibleStopTime = ofGetElapsedTimeMillis();
    }
}
