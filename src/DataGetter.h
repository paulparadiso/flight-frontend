//
//  DataGetter.h
//  flight
//
//  Created by Paul Paradiso on 3/11/16.
//
//

#ifndef __flight__DataGetter__
#define __flight__DataGetter__

#include <stdio.h>
#include "ofMain.h"
#include "DataClient.h"
#include "ofxJSON.h"
#include "sio_client.h"
#include "sio_message.h"
#include "sio_socket.h"
#include "ofxNetwork.h"
#include "SystemInfo.h"

#define DEFAULT_UDP_PORT 9009

typedef struct _processPoll{
    int pollRate;
    int lastPoll = 0;
    string process;
} processPoll;

class Poller{
public:
    Poller(){}
    Poller(string _process, int _pollRate);
    string getProcess();
    int getPollRate();
    int getLastPoll();
    void setLastPoll(int _lp);
    void updateClients(ofBuffer & _data);
    void addClient(DataClient* _client);
private:
    int pollRate;
    int lastPoll;
    string process;
    vector<DataClient*> clients;
    ofxJSONElement json;
};

class HttpDataGetter{
public:
    HttpDataGetter();
    ~HttpDataGetter();
    void startSync();
    void stopSync();
    void addProcess(string _process, int _minutes);
    void urlResponse(ofHttpResponse & response);
    void update(ofEventArgs &e);
    void makeRequest(string _process);
    vector<string> getProcesses();
    void addClient(string _process, DataClient* client);
    void setBaseUrl(string _url);
    void setPollRate(int _minutes);
    void addAirport(string _airport);
private:
    void updateClients(string _process);
    void updateProcess(string _process);
    void updateAllProcesses();
    //vector<processPoll> processes;
    map<string, Poller> pollers;
    vector<string> airports;
    map<string, vector<DataClient*> > clients;
    string baseUrl;
    int pollRate;
    int timeOfLastQuery;
};

class SioDataGetter{
public:
    SioDataGetter();
    SioDataGetter(string _baseUrl);
    ~SioDataGetter();
    void connect();
    void connect(string _baseUrl);
    void addProcess(string _process);
    void removeProcess(string _process);
    void joinRooms();
    void leaveRooms();
    void setAirport(string _airport);
    void addClient(string _process, DataClient* _client);
    void onUpdate(sio::event & _e);
    void update();
private:
    void setupSocket();
    void updateClients(string _process, string _data);
    string baseUrl;
    map<string, string> dataStore;
    string airport;
    vector<string> processes;
    vector<string> activeProcesses;
    sio::client sioClient;
    sio::socket::ptr sioSocket;
    std::function<void(sio::event&)> onUpdateFunc;
    //std::function<void(sio::event&)> onFunc;
    map<string, vector<DataClient*> > clients;
};

class UDPDataGetter{
public:
    UDPDataGetter(int _port=DEFAULT_UDP_PORT);
    ~UDPDataGetter();
    void setAirport(string _airport);
    void addClient(string _process, DataClient* _client);
    void setSysInfo(shared_ptr<SystemInfo> _info);
    void update();
private:
    void init();
    void startUpdate();
    void endUpdate();
    void updateClients();
    ofxUDPManager udpManager;
    string airport;
    string currentUpdate;
    bool bReceivingUpdate;
    map<string, vector<DataClient*> > clients;
    shared_ptr<SystemInfo> sysInfo;
};
#endif /* defined(__flight__DataGetter__) */
