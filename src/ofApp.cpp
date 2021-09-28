#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    airport = "MSP";
    loadConfig();
    for(auto pIter = processes.begin(); pIter != processes.end(); pIter++){
        cout << "Process ==> " << *pIter << endl;
    }
    ofBackground(204,204,255);
    //ofBackground(0,0,0);
    //ofBackground(255,255,255);
    ofSetVerticalSync(false);
    ofEnableAlphaBlending();
    input = new ofxDatGuiTextInput("AIRPORT INPUT", "Airport Code");
    input->onTextInputEvent(this, &ofApp::onTextInputEvent);
    input->setWidth(300, .5);
    input->setPosition(ofGetWidth()/2 - input->getWidth()/2, 240);
    //airport = "JFK";
    //processes = {"birds", "arrivals", "departures","weather"};
    //processes = {"weather"};
    //processes = {"weather","birds","arrivals","departures"};
    //processes = {"weather","birds", "arrivals"};
    //ofSetFrameRate(12);
    //initSockets();
    //dataGetter.setBaseUrl("http://127.0.0.1:5000/get");
    //dataGetter.addProcess("weather", 1);
    //dataGetter.addClient("weather", make_shared<Weather>(weather));
    //dataGetter.addAirport("JFK");
    //dataGetter.startSync();
    //birdLauncher = make_shared<BirdLauncher>();
    sysInfo = make_shared<SystemInfo>();
    //sysInfo->setBaseUrl("http://127.0.0.1:5000/");
    sysInfo->setBaseUrl(baseUrl);
    //sysInfo->setBaseUrl("http://127.0.0.1:8000/");
    sysInfo->setAirport(airport);
    sysInfo->init();
    //sioGetter.connect("http://127.0.0.1:8000/");
    if(dataMode == "sio"){
        sioGetter.connect(baseUrl);
        sioGetter.setAirport(airport);
    } else {
        udpGetter.setAirport(airport);
        udpGetter.setSysInfo(sysInfo);
    }
    //sioGetter.addProcess("weather");
    //sioGetter.addProcess("birds");
    for(auto pIter = processes.begin(); pIter != processes.end(); pIter++){
        if(dataMode == "sio"){
            sioGetter.addProcess(*pIter);
        }
    }
    if(find(processes.begin(), processes.end(), "flights") != processes.end()){
        if(dataMode == "sio"){
            sioGetter.addClient("flights", &flightLauncher);
        } else {
            udpGetter.addClient("arrivals", &flightLauncher);
            udpGetter.addClient("departures", &flightLauncher);
        }
    }
    /*
    if(find(processes.begin(), processes.end(), "departures") != processes.end()){
        sioGetter.addClient("departures", &flightLauncher);
    }
    */
    if(find(processes.begin(), processes.end(), "weather") != processes.end()){
        if(dataMode == "sio"){
            sioGetter.addClient("weather", &weather);
        } else {
            udpGetter.addClient("weather", &weather);
        }
    }
    if(find(processes.begin(), processes.end(), "birds") != processes.end()){
        if(dataMode == "sio"){
            sioGetter.addClient("birds", &birdLauncher);
        } else {
            udpGetter.addClient("birds", &birdLauncher);
        }
    }
    if(find(processes.begin(), processes.end(), "satellites") != processes.end()){
        if(dataMode == "sio"){
            sioGetter.addClient("satellites", &flightLauncher);
        } else {
            udpGetter.addClient("satellites", &flightLauncher);
        }
    }
    if(find(processes.begin(), processes.end(), "airquality") != processes.end()){
        if(dataMode == "sio"){
            sioGetter.addClient("airquality", &weather);
        } else {
            udpGetter.addClient("airquality", &weather);
        }
    }
    if(find(processes.begin(), processes.end(), "news") != processes.end()){
        if(dataMode == "sio"){
            sioGetter.addClient("news", &newsDisplay);
        } else {
            udpGetter.addClient("news", &newsDisplay);
        }
    }
    weather.setSysInfo(sysInfo);
    weather.setAirport(airport);
    flightLauncher.setSysInfo(sysInfo);
    flightLauncher.setLive(true);
    //birdLauncher.loadFromFile("birds.json");
    birdLauncher.setSysInfo(sysInfo);
    //sioGetter.addClient("birds", &birdLauncher);
    if(dataMode == "sio"){
        sioGetter.joinRooms();
    }
    objectLauncher.setSysInfo(sysInfo);
    //ofTrueTypeFont::setGlobalDpi(300);
    //ao.loadFromDirectory("animations/birds/accipitriformes/accipitridae/images_0");
    cout << "It is currently " << sysInfo->getDayPart() << "time." << " " << sysInfo->getDayPercent() << " through." << endl;
    timeOfLastDisplayChange = ofGetElapsedTimeMillis();
    currentDisplaySetting = 0;
}

void ofApp::setResolution(int _w, int _h, bool _fs){
    ofGLWindowSettings settings;
    settings.width = _w;
    settings.height = _h;
    ofSetFullscreen(_fs);
}

void ofApp::loadConfig(){
    ofxJSONElement config;
    config.open("config/config.json");
    bool bFullscreen = config["params"]["fullscreen"].asBool();
    string resolution = config["params"]["resolution"].asString();
    auto resTokens = ofSplitString(resolution, "x");
    if(resTokens.size() > 1){
        setResolution(ofToInt(resTokens[0]), ofToInt(resTokens[1]), bFullscreen);
    } else {
        setResolution(1920, 1080, bFullscreen);
    }
    if(bFullscreen){
        ofHideCursor();
    }
    airport = config["params"]["airport"].asString();
    baseUrl = config["params"]["base_url"].asString();
    dataMode = config["params"]["data_mode"].asString();
    string rawProcs = config["params"]["processes"].asString();
    auto procTokens = ofSplitString(rawProcs, ",");
    if(procTokens.size() > 0){
        for(auto pIter = procTokens.begin(); pIter != procTokens.end(); pIter++){
            processes.push_back(*pIter);
        }
    }
}

void onConnect(sio::event & _e){
    cout << _e.get_name() << endl;
}

void onUpdate(sio::event & _e){
    cout << _e.get_name() << endl;
}

void ofApp::initSockets(){
    cout << "initing sockets..." << endl;
    sioSocket = sioClient.socket();
    sioSocket->on("update", sio::socket::event_listener_aux([&](string const& name, sio::message::ptr const& data, bool isAck,sio::message::list &ack_resp){
        cout << name << endl;
    }));
    sioSocket->on("connect", &onConnect);
    sioSocket->on("update", &onUpdate);
    sioClient.connect("http://127.0.0.1:5000/");
    sio::message::list li;
    li.push("{\"process\": \"weather\", \"airport\": \"JFK\"}");
    sioSocket->emit("join", li);
}

//--------------------------------------------------------------
void ofApp::update(){
    weather.update();
    birdLauncher.update();
    flightLauncher.update();
    newsDisplay.update();
    if(bDrawAirportUI){
        input->update();
    }
    objectLauncher.update();
    if(dataMode == "sio"){
        sioGetter.update();
    } else {
        udpGetter.update();
    }
    if(ofGetElapsedTimeMillis() - timeOfLastDisplayChange > DISPLAY_CHANGE_DELAY){
        currentDisplaySetting = (currentDisplaySetting + 1) % 4;
        if(currentDisplaySetting == 0){
            newsDisplay.setVisible(false);
        }
        if(currentDisplaySetting == 1){
            sysInfo->setVisible(true);
        }
        if(currentDisplaySetting == 2){
            sysInfo->setVisible(false);
        }
        if(currentDisplaySetting == 3){
            newsDisplay.setVisible(true);
        }
        /*
        if(newsDisplay.getVisible()){
            newsDisplay.setVisible(false);
            sysInfo->setVisible(false);
        } else if(sysInfo->getVisible()){
            sysInfo->setVisible(false);
            newsDisplay.setVisible(true);
        } else {
            sysInfo->setVisible(true);
            newsDisplay.setVisible(false);
        }
        */
        timeOfLastDisplayChange = ofGetElapsedTimeMillis();
    }
    //ao.update();
    //cout << "It is currently " << sysInfo->getDayPart() << "time." << " " << sysInfo->getDayPercent() << " through." << endl;
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    // text input events carry the text of the input field //
    //cout << "From Event Object: " << e.text << endl;
    // although you can also retrieve it from the event target //
    //cout << "From Event Target: " << e.target->getText() << endl;
    cout << "Setting airport to " << e.text << endl;
    setAirport(e.text);
    bDrawAirportUI = false;
}

void ofApp::setAirport(string _airport){
    airport = _airport;
    if(dataMode == "sio"){
        sioGetter.leaveRooms();
        sioGetter.setAirport(_airport);
    }
    sysInfo->setAirport(_airport);
    flightLauncher.clear();
    birdLauncher.clear();
    if(dataMode == "sio"){
        sioGetter.joinRooms();
    } else {
        udpGetter.setAirport(_airport);
    }
    weather.setAirport(_airport);
}

//--------------------------------------------------------------
void ofApp::draw(){
    weather.draw();
    birdLauncher.draw();
    flightLauncher.draw();
    weather.drawGUI();
    /*
    if(bDrawInfo){
        sysInfo->draw();
    }
    */
    sysInfo->draw();
    newsDisplay.draw();
    if(bDrawAirportUI){
        input->draw();
    }
    objectLauncher.draw();
    //ao.draw();
}

void ofApp::reload(){
    setAirport(airport);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    /*
    if(key == 'i'){
        initSockets();
    }
    */
    if(key >= 48 && key <= 57){
        weather.setWeatherActive(key);
        return;
    }
    if(!bDrawAirportUI){
        if(key == 'r'){		
            reload();
        }
        if(key == 'g'){
            //weather.toggleGUI();
            bGuiOn = !bGuiOn;
            weather.setGui(bGuiOn);
            sysInfo->setGui(bGuiOn);
        }
        if(key == 'w'){
            weather.toggleWeather();
        }
        if(key == 'f'){
            ofToggleFullscreen();
        }
        if(key == 'i'){
            //bDrawInfo = !bDrawInfo;
            sysInfo->toggleVisible();
        }
        if(key == 'n'){
            newsDisplay.toggleVisible();
        }
        if(key == 'a'){
            bDrawAirportUI = true;
        }
        if(key == 'l'){
            ofFileDialogResult selection = ofSystemLoadDialog("Select a folder.", true);
            //cout << "Selected folder - " << selection.getPath() << endl;
            objectLauncher.load(selection.getPath());
            lastObjectLaunched = selection.getPath();
        }
        if(key == 'p'){
            if(lastObjectLaunched != ""){
                objectLauncher.load(lastObjectLaunched);
            }
        }
        if(key == 's'){
            objectLauncher.stop();
        }
    }
}
	
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
