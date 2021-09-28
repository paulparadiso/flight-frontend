//
//  NewsDisplay.hpp
//  flight
//
//  Created by Paul Paradiso on 12/30/17.
//
//

#ifndef NewsDisplay_h
#define NewsDisplay_h

#include <stdio.h>
#include "TextBlock.h"
#include "DataClient.h"

#define NEWS_PANEL_WIDTH 0.25
#define SUMMARY_Y_START 50.0
#define FADE_IN 2000

class NewsDisplay : public DataClient{
public:
    NewsDisplay();
    ~NewsDisplay(){}
    void update();
    void update(string _data);
    void loadNews();
    void addSummary(string _summary);
    void clear();
    void draw();
    void toggleVisible();
    void setVisible(bool _v);
    bool getVisible(){return bVisible;}
private:
    vector<TextBlock> newsItems;
    bool bVisible = false;
    string news;
    bool bHaveNewNews = false;
    ofTrueTypeFont labelFont;
    int visibleStartTime;
    int visibleStopTime;
};

#endif /* NewsDisplay_hpp */
