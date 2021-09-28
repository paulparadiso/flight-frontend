//
//  ObjectLauncher.cpp
//  flight
//
//  Created by Paul Paradiso on 6/20/16.
//
//

#include "ObjectLauncher.h"

ObjectLauncher::ObjectLauncher(){
    
}

ObjectLauncher::~ObjectLauncher(){
    
}

void ObjectLauncher::load(string _path){
    cout << "Loading " << _path << endl;
    
    /*
    objects.push_back(AnimatedObject(_path, false, sysInfo));
    objects.back().reset();
    */
    
    if(object != nullptr){
        stop();
    }
    object = new AnimatedObject(_path, false, sysInfo);
    object->reset();
    object->setLooping(true);
}

void ObjectLauncher::update(){
    
    /*
    for(auto oIter = objects.begin(); oIter != objects.end();){
        oIter->update();
        if(oIter->isDone()){
            oIter = objects.erase(oIter);
        } else {
            ++oIter;
        }
    }
    */
    
    if(object != nullptr){
        object->update();
        if(object->isDone()){
            stop();
        }
    }
}

void ObjectLauncher::draw(){
    
    /*
    for(auto oIter = objects.begin(); oIter != objects.end(); oIter++){
        oIter->draw();
    }
    */
    
    if(object != nullptr){
        object->draw();
    }
}

void ObjectLauncher::clear(){
    objects.clear();
}

void ObjectLauncher::stop(){
    delete object;
    object = nullptr;
}