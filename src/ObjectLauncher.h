//
//  ObjectLauncher.h
//  flight
//
//  Created by Paul Paradiso on 6/20/16.
//
//

#ifndef __flight__ObjectLauncher__
#define __flight__ObjectLauncher__

#include <stdio.h>
#include "ofMain.h"
#include "AnimatedObject.h"
#include "DataClient.h"
#include "ofxJSON.h"
#include "SystemInfo.h"

class ObjectLauncher{
public:
    ObjectLauncher();
    ~ObjectLauncher();
    void load(string _path);
    void update();
    void draw();
    void clear();
    void setSysInfo(shared_ptr<SystemInfo> _info){sysInfo = _info;}
    void stop();
private:
    vector<AnimatedObject> objects;
    AnimatedObject* object = nullptr;
    shared_ptr<SystemInfo> sysInfo = nullptr;
};

#endif /* defined(__flight__ObjectLauncher__) */
