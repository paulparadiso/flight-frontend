#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxWatchdog.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context
    //ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    //ofSetupOpenGL(1280,720, OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofRunApp(new ofApp());
    
    ofGLWindowSettings settings;
    settings.width = 1280;
    settings.height = 720;
    
    settings.setGLVersion(3,3); /// < select your GL Version here
    ofCreateWindow(settings); ///< create your window here
    
    ofxWatchdog::watch(10000, true, true, true);
    
    ofRunApp(new ofApp());
}
