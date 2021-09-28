//
//  DataClient.h
//  flight
//
//  Created by Paul Paradiso on 3/11/16.
//
//

#ifndef __flight__DataClient__
#define __flight__DataClient__

#include <stdio.h>
#include <ofxJSON.h>
#include "SystemInfo.h"

class DataClient{
public:
    DataClient(){}
    virtual void update(string _data){}
    void setSysInfo(shared_ptr<SystemInfo> _info);
    virtual void dataReceived(ofxJSONElement & data){cout << "bang" << endl;}
protected:
    shared_ptr<SystemInfo> sysInfo;
};

#endif /* defined(__flight__DataClient__) */
