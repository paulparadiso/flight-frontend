#pragma once

#include "ofMain.h"
#include "sio_client.h"
#include "sio_message.h"
#include "sio_socket.h"
#include "DataGetter.h"
#include "Weather.h"
#include "BirdLauncher.h"
#include "FlightLauncher.h"
#include "AnimatedObject.h"
#include "ObjectLauncher.h"
#include "SystemInfo.h"
#include "NewsDisplay.h"

#define DISPLAY_CHANGE_DELAY 60000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        void initSockets();
    
        void setAirport(string _airport);
    
        void loadConfig();
    
        void setResolution(int _w, int _y, bool _fs=false);
    
        sio::client sioClient;
        sio::socket::ptr sioSocket;
        HttpDataGetter dataGetter;
        SioDataGetter sioGetter;
        UDPDataGetter udpGetter;
        Weather weather;
        AnimatedObject ao;
        BirdLauncher birdLauncher;
        FlightLauncher flightLauncher;
        NewsDisplay newsDisplay;
        shared_ptr<SystemInfo> sysInfo;
    
        string airport;
        string baseUrl;
    
        vector<string> processes;
    
        void reload();

        bool bDrawInfo = false;
        bool bDrawAirportUI = false;
    
        ofxDatGuiTextInput* input;
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
        string lastObjectLaunched = "";
    
        ObjectLauncher objectLauncher;
        bool bGuiOn = false;
        string dataMode = "sio";
        int timeOfLastDisplayChange;
        int currentDisplaySetting;
};
