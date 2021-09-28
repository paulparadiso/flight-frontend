//
//  DataGetter.cpp
//  flight
//
//  Created by Paul Paradiso on 3/11/16.
//
//

#include "DataGetter.h"

Poller::Poller(string _process, int _pollRate){
    
}

string Poller::getProcess(){
    
}

int Poller::getPollRate(){
    
}

int Poller::getLastPoll(){
    
}

void Poller::setLastPoll(int _lp){
    lastPoll = _lp;
}

void Poller::updateClients(ofBuffer & _data){
    json.parse(_data);
    for(auto cIter = clients.begin(); cIter != clients.end(); cIter++){
        cout << "Updating Client." << endl;
        (*cIter)->dataReceived(json);
    }
}

void Poller::addClient(DataClient* _client){
    clients.push_back(_client);
}

HttpDataGetter::HttpDataGetter(){
    
}

HttpDataGetter::~HttpDataGetter(){
    
}

void HttpDataGetter::startSync(){
    ofAddListener(ofEvents().update, this, &HttpDataGetter::update);
    timeOfLastQuery = ofGetElapsedTimeMillis();
    ofRegisterURLNotification(this);
    for(auto pIter = pollers.begin(); pIter != pollers.end(); pIter++){
        makeRequest(pIter->first);
        pIter->second.setLastPoll(ofGetElapsedTimeMillis());
    }
}

void HttpDataGetter::stopSync(){
    ofRemoveListener(ofEvents().update, this, &HttpDataGetter::update);
}

void HttpDataGetter::addProcess(string _process, int _minutes){
    if(pollers.find(_process) == pollers.end()){
        cout << "Adding " << _process << " to processes." << endl;
        pollers[_process] = Poller(_process, _minutes * 1000);
        //makeRequest(_process);
    }
}

void HttpDataGetter::update(ofEventArgs &e){
    //cout << "Updating DataGetter." << endl;
    int now = ofGetElapsedTimeMillis();
    for(auto pIter = pollers.begin(); pIter != pollers.end(); pIter++){
        if(now - pIter->second.getLastPoll() >  pIter->second.getPollRate()){
            makeRequest(pIter->first);
        }
    }
}

void HttpDataGetter::makeRequest(string _process){
    for(auto aIter = airports.begin(); aIter != airports.end(); aIter++){
        stringstream urlGen;
        urlGen << baseUrl << "/" << _process << "/" << *aIter;
        stringstream requestName;
        requestName << _process << "/" << *aIter;
        cout << "Making request to " << urlGen.str() << endl;
        ofLoadURLAsync(urlGen.str(), requestName.str());
    }
}

void HttpDataGetter::urlResponse(ofHttpResponse & response){
    if(response.status == 200){
        cout << "Got Response." << endl;
        auto nameTokens = ofSplitString(response.request.name, "/");
        if(nameTokens.size() > 1){
            pollers[nameTokens[0]].updateClients(response.data);
        }
    }
}

void HttpDataGetter::addClient(string _process, DataClient* client){
    if(pollers.find(_process) != pollers.end()){
        pollers[_process].addClient(client);
    }
}

void HttpDataGetter::setBaseUrl(string _url){
    baseUrl = _url;
}

void HttpDataGetter::addAirport(string _airport){
    airports.push_back(_airport);
}

SioDataGetter::SioDataGetter(){
    setupSocket();
}

SioDataGetter::SioDataGetter(string _baseUrl){
    setupSocket();
    baseUrl = _baseUrl;
}

SioDataGetter::~SioDataGetter(){
    //leaveRooms();
    //sioClient.close();
}

void SioDataGetter::setupSocket(){
    sioSocket = sioClient.socket();
    onUpdateFunc = std::bind(&SioDataGetter::onUpdate, this, std::placeholders::_1);
    sioSocket->on("update", onUpdateFunc);
    
}

void SioDataGetter::connect(){
    
}

void SioDataGetter::update(){
    /*
    if(!sioClient.opened()){
        cout << "SIO closed. Re-opening." << endl;
        connect(baseUrl);
        joinRooms();
    }
    */
}

void SioDataGetter::connect(string _baseUrl){
    baseUrl = _baseUrl;
    sioClient.connect(baseUrl);
}

void SioDataGetter::addProcess(string _process){
    processes.push_back(_process);
}

void SioDataGetter::removeProcess(string _process){
    for(auto pIter = processes.begin(); pIter != processes.end();){
        if(*pIter != _process){
            pIter = processes.erase(pIter);
        } else {
            ++pIter;
        }
    }
}

void SioDataGetter::setAirport(string _airport){
    airport = _airport;
}

void SioDataGetter::joinRooms(){
    activeProcesses = processes;
    for(auto pIter = activeProcesses.begin(); pIter != activeProcesses.end(); pIter++){
        sio::message::list li;
        stringstream msg;
        msg << "{\"process\": \"" << *pIter << "\", \"airport\": \"" << airport << "\"}" << endl;
        li.push(msg.str());
        sioSocket->emit("join", li);
    }
}

void SioDataGetter::leaveRooms(){
    for(auto pIter = activeProcesses.begin(); pIter != activeProcesses.end(); pIter++){
        sio::message::list li;
        stringstream msg;
        msg << "{\"process\": \"" << *pIter << "\", \"airport\": \"" << airport << "\"}" << endl;
        li.push(msg.str());
        sioSocket->emit("leave", li);
    }
}

void SioDataGetter::addClient(string _process, DataClient* _client){
    if(clients.find(_process) != clients.end()){
        clients[_process].push_back(_client);
    } else {
        clients[_process] = {_client};
    }
}

void SioDataGetter::updateClients(string _process, string _data){
    for(auto cIter = clients[_process].begin(); cIter != clients[_process].end(); cIter++){
        //cout << "updateing client of " << _process << ".\n" << _data << endl;
        if(_data.size() > 10){
            (*cIter)->update(_data);
        }
    }
}

void SioDataGetter::onUpdate(sio::event & _e){
    //cout << "update received: name = " << _e.get_name() << ", nsp = " << _e.get_nsp() << endl;
    auto msg = _e.get_message();
    auto msgMap = msg->get_map();
    //cout << "\tprocess = " << msgMap["process"]->get_string() << endl;
    //cout << "\tdata = " << msgMap["data"]->get_string() << endl;;
    updateClients(msgMap["process"]->get_string(), msgMap["data"]->get_string());
}

UDPDataGetter::UDPDataGetter(int _port){
    cout << "Creating UDP receiver on port - " << _port << endl;
    udpManager.Create();
    udpManager.Bind(_port);
    udpManager.SetNonBlocking(true);
}

UDPDataGetter::~UDPDataGetter(){
    
}

void UDPDataGetter::setAirport(string _airport){
    airport = _airport;
}

void UDPDataGetter::addClient(string _process, DataClient* _client){
    if(clients.find(_process) != clients.end()){
        clients[_process].push_back(_client);
    } else {
        clients[_process] = {_client};
    }
}

void UDPDataGetter::update(){
    char udpMessage[100000];
    udpManager.Receive(udpMessage,100000);
    string msg = udpMessage;
    /*
    if(msg != ""){
        cout << "RECEIVED MESSAGE - " << msg << endl;
    }
    */
    if(msg == ""){
        return;
    }
    if(bReceivingUpdate){
        if(msg == "END_UPDATE"){
            endUpdate();
        } else {
            currentUpdate += msg;
        }
    } else if(msg == "START_UPDATE"){
        startUpdate();
    } else {
        ofxJSONElement data;
        data.parse(msg);
        if(data["message"] == "info_on" ){
            //cout << "Received command." << endl;
            bool bTargetted = false;
            for(auto tIter = data["targets"].begin(); tIter != data["targets"].end(); tIter++){
                //cout << "Target = " << *tIter << endl;
                if(*tIter == airport){
                    bTargetted = true;
                }
            }
            if(bTargetted){
                sysInfo->setVisible(true);
            } else {
                sysInfo->setVisible(false);
            }
        }
    }
    
}

void UDPDataGetter::startUpdate(){
    bReceivingUpdate = true;
    currentUpdate = "";
}

void UDPDataGetter::endUpdate(){
    bReceivingUpdate = false;
    updateClients();
    //cout << "update complete." << endl;
    //cout << currentUpdate << endl;
}

void UDPDataGetter::updateClients(){
    ofxJSONElement data;
    data.parse(currentUpdate);
    //cout << "Sending " << currentUpdate << endl;
    for(auto cIter = clients.begin(); cIter != clients.end(); cIter++){
        //cout << "Have " << data.getRawString() << endl;
        string process = cIter->first;
        cout << "Sending to " << process << endl;
        //cIter->second->update(data[airport][process]);
        for(auto pIter = clients[process].begin(); pIter != clients[process].end(); pIter++){
            //cout << "Writing " << data[airport][process] << endl;
            //data[airport][process]["type"] = process;
            Json::FastWriter writer;
            string output = writer.write(data[airport][process]);
            //cout << "Sending " << output << endl;
            if(output.size() > 10){
                (*pIter)->update(output);
            }
        }
    }
}

void UDPDataGetter::setSysInfo(shared_ptr<SystemInfo> _info){
    sysInfo = _info;
}
