//
//  NewsDisplay.cpp
//  flight
//
//  Created by Paul Paradiso on 12/30/17.
//
//

#include "NewsDisplay.h"

NewsDisplay::NewsDisplay(){
    float scale = ofGetHeight() / 1080.f;
    labelFont.load("animations/fonts/roboto/RobotoCondensed-Regular.ttf", 20 * scale);
}

void NewsDisplay::update(){
    if(bHaveNewNews){
        loadNews();
        bHaveNewNews = false;
    }
}

void NewsDisplay::update(string _data){
    //cout << "News display received new news." << endl;
    //cout << _data << endl;
    news = _data;
    bHaveNewNews = true;
}

void NewsDisplay::loadNews(){
    clear();
    ofxJSONElement data;
    data.parse(news);
    for(auto nIter = data["data"].begin(); nIter != data["data"].end(); nIter++){
        //cout << "..." << (*nIter)["summary"].asString() << "..." << endl;
        addSummary("..." + (*nIter)["summary"].asString() + "...");
    }
}

void NewsDisplay::addSummary(string _summary){
    float scale = ofGetHeight() / 1080.f;
    float drawPosY = 50.0;
    if(newsItems.size() > 0){
        float height = newsItems.back().getBodyHeight();
        float prevY = newsItems.back().getDrawPos().y;
        ofVec2f newDrawPos;
        drawPosY = SUMMARY_Y_START + prevY + height;
    }
    newsItems.push_back(TextBlock());
    newsItems.back().setBodyFont("animations/fonts/roboto/RobotoCondensed-Regular.ttf", 16);
    //newsItems.back().setup("", 20, (200 * scale) * newsItems.size());
    //newsItems.back().setDrawPos(20, 180 * scale * newsItems.size());
    newsItems.back().addBlock(_summary, (ofGetWidth() * NEWS_PANEL_WIDTH - 60.0) * scale );
    newsItems.back().setDrawPos(20.0, drawPosY);
}

void NewsDisplay::clear(){
    newsItems.clear();
}

void NewsDisplay::draw(){
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
    ofSetColor(255,255,255,128 * (alpha / 255.f));
    ofDrawRectangle(0, 0, ofGetWidth() * NEWS_PANEL_WIDTH, ofGetHeight());
    ofSetColor(127,127,127,alpha);
    labelFont.drawString("STAR TRIBUNE", 20.0, 20.0);
    ofSetColor(0,0,0,10);
    for(auto nIter = newsItems.begin(); nIter != newsItems.end(); nIter++){
        if(nIter > newsItems.begin()){
            auto preNIter = nIter;
            preNIter--;
            if(preNIter->getDrawPos().y + preNIter->getBodyHeight() + SUMMARY_Y_START + nIter->getBodyHeight() < ofGetHeight()){
                nIter->setBodyColor(ofColor(0,0,0,alpha));
                nIter->draw();
            }
        } else {
            nIter->setBodyColor(0,0,0,alpha);
            nIter->draw();
        }
    }
    ofSetColor(255,255,255,255);
}

void NewsDisplay::toggleVisible(){
    if(bVisible){
        setVisible(false);
    } else {
        setVisible(true);
    }
}

void NewsDisplay::setVisible(bool _v){
    bVisible = _v;
    if(bVisible){
        cout << "Setting visible start time." << endl;
        visibleStartTime = ofGetElapsedTimeMillis();
    } else {
        visibleStopTime = ofGetElapsedTimeMillis();
    }
}
