#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxTSPSReceiver.h"
#include "ofxTriangle.h"
#include "ofxInteractiveSilhouette.h"
#include "ofxBox2d.h"

class testApp : public ofBaseApp, public ofxTSPSListener
{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxTSPSReceiver* receiver;
	
	ofxBox2d				 physics;
	vector <ofxBox2dCircle>  circles;
	vector <ofxBox2dPolygon> polys;
	
	
	//called when the person enters the system
	void personEntered( ofxTSPSPerson* person, ofxTSPSScene* scene );
	//called each time the centroid moves (a lot)
	void personMoved( ofxTSPSPerson* person, ofxTSPSScene* scene );
	//called one frame before the person is removed from the list to let you clean up
	void personWillLeave( ofxTSPSPerson* person, ofxTSPSScene* scene );
	//called every frame no matter what.
	void personUpdated(ofxTSPSPerson* person, ofxTSPSScene* scene);
	
};

#endif
