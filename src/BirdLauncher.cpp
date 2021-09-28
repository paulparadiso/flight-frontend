//
//  BirdLauncher.cpp
//  flight
//
//  Created by Paul Paradiso on 4/4/16.
//
//

#include "BirdLauncher.h"

Bird::Bird(){
    bFlying = false;
}

Bird::Bird(ofxJSONElement _data){
    loadData(_data);
    bFlying = false;
}

Bird::~Bird(){
    
}

void Bird::update(){
    /*
    int now = ofGetElapsedTimeMillis();
    if(!bFlying){
        if(now - lastLaunch > displayDelay){
            bFlying = true;
        }
    } else {
        ao.update();
    }
    */
    ao.update();
    /*
    if(ao.getPosition().x > (ofGetWidth() + ao.getWidth())){
        cout << "Turning off bird." << endl;
        bFlying = false;
    }
    */
    if(ao.isDone()){
        cout << "Turning off bird." << endl;
        bFlying = false;
    }
}

void Bird::draw(){
    //cout << "drawing " << path << endl;
    ao.draw();
}

bool Bird::isFlying(){
    return bFlying;
}

void Bird::loadData(ofxJSONElement _data){
    //cout << "Loading " << _data << endl;
    //ofxJSONElement d;
    //d.parse(_data);
    //cout << d.getRawString() << endl;
    string aPath = _data["animation_path"].asString();
    path = aPath;
    //ao.loadFromDirectory(path);
    //string settingsPath = path + "/" + "settings.json";
    //settings.open(settingsPath);
    auto minSettingsNames = _data["settings"]["min"].getMemberNames();
    auto maxSettingsNames = _data["settings"]["max"].getMemberNames();
    for(auto sIter = minSettingsNames.begin(); sIter != minSettingsNames.end(); sIter++){
        if(*sIter == "mirrored"){
            ao.setMirrored(_data["settings"]["min"][*sIter].asBool());
        } else {
            ao.addSettings(*sIter, _data["settings"]["min"][*sIter].asFloat(), "min");
        }
    }
    for(auto sIter = maxSettingsNames.begin(); sIter != maxSettingsNames.end(); sIter++){
        if(*sIter == "mirrored"){
            ao.setMirrored(_data["settings"]["max"][*sIter].asBool());
        } else {
            ao.addSettings(*sIter, _data["settings"]["max"][*sIter].asFloat(), "max");
        }
    }
    //cout << "Path set to " << path << endl;
    //ao.reset();
    //bFlying = true;
}

void Bird::loadData(string _path){
    
}

void Bird::loadFromDirectory(string _dir){
    path = _dir;
    ao.loadFromDirectory(path, true);
}

void Bird::launch(){
    
    if(!ao.isLoaded()){
        if(!ofDirectory::doesDirectoryExist(getPath())) {
            bFlying = false;
            cout << "Path - " << getPath() << " does not exist." << endl;
            return;
        }
        if(getPath() == ""){
            cout << "Bad path." << endl;
            bFlying = false;
            return;
        }
        cout << "Loading AO from " << getPath() << "|" << " for " << getName() << "|" << endl;
        ao.loadFromDirectory(getPath());
    }
    ao.setDirection(OBJECT_DIRECTION_RIGHT);
    ao.reset();
    bFlying = true;
}

string Bird::getPath(){
    return path;
}

BirdLauncher::BirdLauncher(){
    ofRegisterURLNotification(this);
    setRandomLaunch();
    setUfoLaunch();
    loadCommonBirds();
    loadBats();
    loadUfos();
    birth.loadFromDirectory("animations/birth/images_0");
    death.loadFromDirectory("animations/death/images_0");
}

BirdLauncher::~BirdLauncher(){
    
}

void BirdLauncher::setRandomLaunch(){
    int r = ofRandom(MAX_RANDOM_TIME);
    randomLaunch = ofGetElapsedTimeMillis() + r;
    cout << "Launching random bird in " << r / 1000 << " seconds." << endl;
}

void BirdLauncher::setCommonLaunch(){
    int r = ofRandom(COMMON_BIRD_DELAY);
    commonLaunch = ofGetElapsedTimeMillis() + r;
    cout << "Launching common bird in " << r / 1000 << " seconds." << endl;
}

void BirdLauncher::setUfoLaunch(){
    int r = ofRandom(UFO_DELAY);
    ufoLaunch = ofGetElapsedTimeMillis() + r;
    cout << "Launching ufo in " << r / 1000 << " seconds." << endl;
}

void BirdLauncher::launchCommonBird(){
    cout << "Launching Common Bird." << endl;
    int random = commonBirds.size() * ofRandom(1.0);
    if(random < 0){
        random = 0;
    }
    if(random > commonBirds.size() - 1){
        random = commonBirds.size() - 1;
    }
    if(!commonBirds[random].isFlying()){
        cout << "Loading common bird #" << random << endl;
        commonBirds[random].launch();
    }
    //commonBirdActive = true;
}

void BirdLauncher::launchBat(){
    cout << "Launching Bat." << endl;
    int random = bats.size() - 1 * ofRandom(1.0);
    if(random < 0){
        random = 0;
    }
    if(random > bats.size() - 1){
        random = bats.size() - 1;
    }
    if(!bats[random].isFlying()){
        bats[random].launch();
    }
}

void BirdLauncher::launchUfo(){
    int random = ufos.size() - 1 * ofRandom(1.0);
    if(random < 0){
        random = 0;
    }
    if(random > ufos.size() - 1){
        random = ufos.size() - 1;
    }
    if(!ufos[random].isFlying()){
        cout << "Launching UFO - " << random << endl;
        ufos[random].launch();
    }
}

void BirdLauncher::launchBirth(){
    if(!birth.isFlying()){
        birth.launch();
    }
}

void BirdLauncher::launchDeath(){
    cout << "Launching Death." << endl;
    if(!death.isFlying()){
        death.launch();
    }
}

string BirdLauncher::urlEncode(const string &value){
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;
    
    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);
        
        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '&' || c == '?' || c == '=' || c == '/') {
            escaped << c;
            continue;
        }
        
        // Any other characters are percent-encoded
        escaped << '%' << setw(2) << int((unsigned char) c);
    }
    
    return escaped.str();
}

void BirdLauncher::urlResponse(ofHttpResponse & response){
    auto chunks = ofSplitString(response.request.name, ":");
    if(chunks[0] != "bird"){
        return;
    }
    string sciName = "";
    if(chunks.size() > 1){
        sciName = chunks[1];
    }
    if(sciName == ""){
        return;
    }
    //cout << "Received bird data:\n" << response.data << endl;
    if(response.status == 200){
        //cout << "received data: " << response.data << endl << endl;
        /*
        ofxJSONElement data;
        data.parse(response.data);
        cout << "\n Path = " << data["path"].asString();
        auto sNames = data["settings"]["max"].getMemberNames();
        for(auto sIter = sNames.begin(); sIter != sNames.end(); sIter++){
            cout << "Settings " << *sIter << " = " << data["settings"]["max"][*sIter].asString() << endl;
        }
        */
        //birds[sciName]->loadData(response.data);
    }
}

void BirdLauncher::update(string _data){
    //cout << "New Bird Data:\n" << _data << endl;
    //ofxJSONElement data;
    //data.parse(_data);
    //for(auto bIter = data.begin(); bIter != data.end(); bIter++){
        //birds[(*bIter)["sciName"].asString()] = make_shared<Bird>();
        //birdData[(*bIter)["sciName"].asString()] = (
        //ofLoadURLAsync("http://127.0.0.1:5000" + request, "bird:" + (*bIter)["sciName"].asString());
        //if((*bIter)["animation_path"].asString() != ""){
        //    cout << "New bird - " << (*bIter)["comName"].asString() << ". Animation path = " << (*bIter)["animation_path"].asString() << endl;
        //    birds.push_back(make_shared<Bird>(*bIter));
        //}
    //}
    //cout << "Received new birds.\n" << _data << endl;
    if(_data == lastUpdate){
        return;
    }
    newBirds = _data;
    bHaveNewBirds = true;
    lastUpdate = _data;
    //loadBirds(newBirds);
}

void BirdLauncher::update(){
    //cout << "Updating bird launcher." << endl;
    if(bHaveNewBirds){
        cout << "Loading birds." << endl;
        loadBirds(newBirds);
        bHaveNewBirds = false;
        /*
        for(auto bIter = birds.begin(); bIter != birds.end(); bIter++){
            cout << "Added bird with path " << bIter->getPath() << endl;
        }
        */
    }
    //if(!commonBirdActive){
    if(ofGetElapsedTimeMillis() > (lastCommonBird + COMMON_BIRD_DELAY)){
        if(sysInfo->getDayPart() == "day"){
            launchCommonBird();
        } else {
            launchBat();
        }
        lastCommonBird = ofGetElapsedTimeMillis();
    } else {
        //cout << "Not launching common bird." << endl;
    }
    if(ofGetElapsedTimeMillis() > (lastUfo + UFO_DELAY)){
        if(sysInfo->getDayPart() != "day"){
            launchUfo();
        }
        lastUfo = ofGetElapsedTimeMillis();
    } else {
        //cout << "Not launching common bird." << endl;
    }
    if(ofGetElapsedTimeMillis() > (lastBirth + BIRTH_DELAY)){
        launchBirth();
        lastBirth = ofGetElapsedTimeMillis();
    }
    if(ofGetElapsedTimeMillis() > (lastDeath + DEATH_DELAY)){
        launchDeath();
        lastDeath = ofGetElapsedTimeMillis();
    } else {
        //cout << "Not launching common bird." << endl;
    }
    
    for(auto cIter = commonBirds.begin(); cIter != commonBirds.end(); cIter++){
        if(cIter->isFlying()){
            cIter->update();
        }
    }
    for(auto bIter = bats.begin(); bIter != bats.end(); bIter++){
        if(bIter->isFlying()){
            //cout << "drawing bat." << endl;
            bIter->update();
        }
    }
    for(auto uIter = ufos.begin(); uIter != ufos.end(); uIter++){
        if(uIter->isFlying()){
            //cout << "drawing bat." << endl;
            uIter->update();
        }
    }
    if(birds.size() > 0){
        /*
        for(auto bIter = birds.begin(); bIter != birds.end(); bIter++){
            cout << "Added bird with path " << bIter->getPath() << endl;
        }
        */
        int now = ofGetElapsedTimeMillis();
        if(now > randomLaunch){
            if(!bLoading){
                //int randomBird = int(ofRandom(birds.size() - 1));
                //if(sysInfo->getDayPart() == "day"){
                    int randomBird = birdSelector[int(ofRandom(birdSelector.size() - 1))];
                    //cout << "Randoom bird num = " << randomBird << ": " << birds[randomBird].getPath() << endl;
                    if(randomBird < birds.size()){
                        birds[randomBird].launch();
                        cout << "Launching " << birds[randomBird].getName() << endl;
                        sysInfo->setBirdInFlight(birds[randomBird].getName());
                    }
                //} else {
                    //launchBat();
                //}
                setRandomLaunch();
            }
        }
        /*
        if(now > commonLaunch){
            if(!bLoading){
                int randomCommonBird = ofRandom(commonBirds.size()) - 1;
                if(randomCommonBird < commonBirds.size()){
                    commonBirds[randomCommonBird].launch();
                }
                setCommonLaunch();
            }
        }
        */
        for(auto bIter = birds.begin(); bIter != birds.end(); bIter++){
            if(bIter->isFlying()){
                bIter->update();
            }
        }
    }
    if(birth.isFlying()){
        cout << "Updating birth." << endl;
        birth.update();
    }
    if(death.isFlying()){
        death.update();
    }
}

void BirdLauncher::clear(){
    birds.clear();
    sysInfo->clearBirds();
}

void BirdLauncher::draw(){
    for(auto bIter = birds.begin(); bIter != birds.end(); bIter++){
        if(bIter->isFlying()){
            bIter->draw();
        }
    }
    for(auto cIter = commonBirds.begin(); cIter != commonBirds.end(); cIter++){
        if(cIter->isFlying()){
            //cout << "Drawing common bird." << endl;
            cIter->draw();
        }
    }
    for(auto bIter = bats.begin(); bIter != bats.end(); bIter++){
        if(bIter->isFlying()){
            bIter->draw();
        }
    }
    for(auto uIter = ufos.begin(); uIter != ufos.end(); uIter++){
        if(uIter->isFlying()){
            uIter->draw();
        }
    }
    if(birth.isFlying()){
        birth.draw();
    }
    if(death.isFlying()){
        death.draw();
    }
}

void BirdLauncher::loadFromFile(string _f){
    ofFile f;
    f.open(ofToDataPath(_f), ofFile::ReadWrite, false);
    ofBuffer buf = f.readToBuffer();
    loadBirds(buf.getText());
}

string BirdLauncher::resolvePath(string _com, string _sci, string _order, string _family, string _genus){
    string base = "animations/birds/";
    ofDirectory tDir;
    /*
    tDir.listDir(base + "_defaults");
    auto defaults = tDir.getFiles();
    for(auto dIter = defaults.begin(); dIter != defaults.end(); dIter++){
        if(_com.find(dIter->getBaseName()) != -1){
            vector<string> dKeepers;
            tDir.listDir(base + "_defaults/" + dIter->getBaseName());
            auto dPaths = tDir.getFiles();
            for(auto kIter = dPaths.begin(); kIter != dPaths.end(); kIter++){
                if(kIter->getBaseName().find("images") != -2){
                    dKeepers.push_back(kIter->getBaseName());
                }
            }
            if(dKeepers.size() > 0){
                string selection = dKeepers[int(dKeepers.size() -1)];
                return base + "_defaults/" + dIter->getBaseName() + "/" + selection;
            }
        }
    }
    */
    
    string test = _order + "/" + _family + "/" + _genus;
    //cout << "Testing path " << test << " for " << _com << "."  << endl;
    //tDir.close();
    if(ofDirectory::doesDirectoryExist(base + test)){
        tDir.listDir(base + test);
        auto paths = tDir.getFiles();
        vector<string> keepers;
        for(auto pIter = paths.begin(); pIter != paths.end(); pIter++){
            //cout << ">>> " << pIter->getBaseName() << endl;
            if(pIter->getBaseName().find("images") != -1){
                keepers.push_back(pIter->getBaseName());
            }
        }
        if(keepers.size() > 0){
            string selection = keepers[int(ofRandom(keepers.size() -1))];
            return base + test + "/" + selection;
        }
        tDir.close();
    }
    test = _order + "/" + _family;
    //cout << "Testing path " << test << " for " << _com << "."  << endl;
    //tDir.close();
    if(ofDirectory::doesDirectoryExist(base + test)){
        tDir.listDir(base + test);
        auto paths = tDir.getFiles();
        //keepers.clear();
        vector<string> keepers;
        for(auto pIter = paths.begin(); pIter != paths.end(); pIter++){
            //cout << ">>> " << pIter->getBaseName() << endl;
            if(pIter->getBaseName().find("images") != -1){
                keepers.push_back(pIter->getBaseName());
            }
        }
        if(keepers.size() > 0){
            string selection = keepers[int(ofRandom(keepers.size() -1))];
            return base + test + "/" + selection;
        }
    }
    test = _order;
    if(ofDirectory::doesDirectoryExist(base + test)){
    //cout << "Testing path " << test << endl;
        tDir.close();
        tDir.listDir(base + test);
        auto paths = tDir.getFiles();
        vector<string> keepers;
        for(auto pIter = paths.begin(); pIter != paths.end(); pIter++){
            //cout << ">>> " << pIter->getBaseName() << endl;
            if(pIter->getBaseName().find("images") != -1){
                keepers.push_back(pIter->getBaseName());
            }
        }
        if(keepers.size() > 0){
            string selection = keepers[int(ofRandom(keepers.size() -1))];
            return base + test + "/" + selection;
        }
        return "";
    }
}

void BirdLauncher::loadBirds(string _birds){
    ofxJSONElement data;
    bLoading = true   ;
    data.parse(_birds);
    sysInfo->clearBirds();
    //for(auto rIter = data.rbegin(); rIter != data.rend(); rIter++){
    //    sysInfo->addBird(*rIter);
    //}
    //int birdEnd = data.size() - 1;
    //int birdStop = data.size() - 6;
    //if(birdStop < 0){
    //    birdStop = 0;
    //}
    cout << "Got " << data.size() << " birds." << endl;
    //for(int i = birdEnd; i > birdStop; i--){
    //    sysInfo->addBird(data[i]);
    //}
    birds.clear();
    birdSelector.clear();
    numSightings = 0;
    sysInfo->clearBirds();
    for(auto bIter = data.begin(); bIter != data.end(); bIter++){
        //birds[(*bIter)["sciName"].asString()] = make_shared<Bird>();
        //birdData[(*bIter)["sciName"].asString()] = (
        //ofLoadURLAsync("http://127.0.0.1:5000" + request, "bird:" + (*bIter)["sciName"].asString());
        //cout << "Checking path for " << (*bIter)["com_name"].asString() << "|" << (*bIter)["sci_name"].asString() << "|" << (*bIter)["order"].asString() << "|" << (*bIter)["family"].asString() << "|" << (*bIter)["genus"].asString() << endl;
        string path = resolvePath((*bIter)["com_name"].asString(), (*bIter)["sci_name"].asString(), (*bIter)["order"].asString(), (*bIter)["family"].asString(), (*bIter)["genus"].asString());
        //cout << "Path = " << path << endl;
        (*bIter)["animation_path"] = path;
        if((*bIter)["animation_path"].asString() != ""){
            numSightings += (*bIter)["how_many"].asInt();
            int position = distance(data.begin(), bIter);
            //cout << "Have " << (*bIter)["how_many"].asInt() << " at position " << position << " " << numSightings << " sightings so far." << endl;
            ofDirectory tDir;
            tDir.listDir((*bIter)["animation_path"].asString());
            if(tDir.exists()){
                //cout << "New bird - " << (*bIter)["com_name"].asString() << ". Animation path = " << (*bIter)["animation_path"].asString() << endl;
                birds.push_back(Bird());
                birds.back().loadData(*bIter);
                birds.back().setName((*bIter)["com_name"].asString());
                for(int i = 0; i < numSightings; i++){
                    birdSelector.push_back(position);
                }
            }
            //if(position < 10){
            sysInfo->addBird(*bIter);
            //}
        } else {
            cout << "No path for " << (*bIter)["com_name"].asString() << "/" << (*bIter)["sci_name"].asString() << ", " << (*bIter)["order"].asString() << ", " << (*bIter)["family"].asString() << endl;
        }
    }
    bLoading = false;
}

void BirdLauncher::loadCommonBirds(){
    cout << "Loading Common birds." << endl;
    bLoading = true;
    commonBirds.clear();
    string baseDirName = "animations/birds/_commons";
    ofDirectory baseDir;
    baseDir.listDir(baseDirName);
    auto paths = baseDir.getFiles();
    vector<string> keepers;
    vector<string> commonPaths;
    for(auto pIter = paths.begin(); pIter != paths.end(); pIter++){
        ofDirectory commonDir;
        //cout << "Checking for animations in " << baseDirName + "/" + pIter->getBaseName() << endl;
        commonDir.listDir(baseDirName + "/" + pIter->getBaseName());
        auto commonPaths = commonDir.getFiles();
        for(auto cIter = commonPaths.begin(); cIter != commonPaths.end(); cIter++){
            cout << "Checking path " << baseDirName + "/" + pIter->getBaseName() + "/" + cIter->getBaseName() << endl;
            if(cIter->getBaseName().find("images") != -1){
                string animationPath = baseDirName + "/" + pIter->getBaseName() + "/" + cIter->getBaseName();
                //cout << "Adding animation path " << animationPath << endl;
                keepers.push_back(animationPath);
            }
        }
    }
    //cout << "Have " << keepers.size() << " common animations." << endl;
    for(auto kIter = keepers.begin(); kIter != keepers.end(); kIter++){
        //cout << "Common bird path - " << *kIter << endl;
        commonBirds.push_back(Bird());
        commonBirds.back().loadFromDirectory(*kIter);
        
    }
}

void BirdLauncher::loadBats(){
    cout << "Loading Bats." << endl;
    bLoading = true;
    bats.clear();
    string baseDirName = "animations/bats";
    ofDirectory baseDir;
    baseDir.listDir(baseDirName);
    auto paths = baseDir.getFiles();
    vector<string> keepers;
    for(auto pIter = paths.begin(); pIter != paths.end(); pIter++){
        cout << "Checking path " << baseDirName + "/" + pIter->getBaseName() << endl;
        if(pIter->getBaseName().find("images") != -1){
            string animationPath = baseDirName + "/" + pIter->getBaseName();
            //cout << "Adding animation path " << animationPath << endl;
            keepers.push_back(animationPath);
        }
    }
    //cout << "Have " << keepers.size() << " common animations." << endl;
    for(auto kIter = keepers.begin(); kIter != keepers.end(); kIter++){
        cout << "Bat path - " << *kIter << endl;
        bats.push_back(Bird());
        bats.back().loadFromDirectory(*kIter);
    }
}

void BirdLauncher::loadUfos(){
    cout << "Loading UFOs." << endl;
    bLoading = true;
    ufos.clear();
    string baseDirName = "animations/ufos";
    ofDirectory baseDir;
    baseDir.listDir(baseDirName);
    auto paths = baseDir.getFiles();
    vector<string> keepers;
    vector<string> commonPaths;
    for(auto pIter = paths.begin(); pIter != paths.end(); pIter++){
        ofDirectory commonDir;
        //cout << "Checking for animations in " << baseDirName + "/" + pIter->getBaseName() << endl;
        commonDir.listDir(baseDirName + "/" + pIter->getBaseName());
        auto commonPaths = commonDir.getFiles();
        for(auto cIter = commonPaths.begin(); cIter != commonPaths.end(); cIter++){
            cout << "Checking path " << baseDirName + "/" + pIter->getBaseName() + "/" + cIter->getBaseName() << endl;
            if(cIter->getBaseName().find("images") != -1){
                string animationPath = baseDirName + "/" + pIter->getBaseName() + "/" + cIter->getBaseName();
                cout << "Adding animation path " << animationPath << endl;
                keepers.push_back(animationPath);
            }
        }
    }
    //cout << "Have " << keepers.size() << " common animations." << endl;
    for(auto kIter = keepers.begin(); kIter != keepers.end(); kIter++){
        //cout << "Common bird path - " << *kIter << endl;
        ufos.push_back(Bird());
        ufos.back().loadFromDirectory(*kIter);
        
    }
}
