#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	
	receiver = new ofxTSPSReceiver();
	receiver->setListener( this );
	receiver->connect(12000);
	
}

//--------------------------------------------------------------
void testApp::update(){
	triangle.clear();
	
//	if(receiver->totalPeople() > 0){
//		triangle.triangulate(receiver->personAtIndex(0)->denormalizedContour(ofGetWidth(), ofGetHeight()));
//	}
	
	for(int i = 0; i < receiver->totalPeople(); i++){
		ofxTSPSPerson* person = receiver->personAtIndex(i);
		ofxInteractiveSilhouette* silhouette = (ofxInteractiveSilhouette*)person->customAttributes;
		vector<ofPoint> contourToTriangulate = person->denormalizedContour(ofGetWidth(), ofGetHeight());
		if(contourToTriangulate.size() > 2){
			silhouette->triangle.clear();
			silhouette->triangle.triangulate( contourToTriangulate, 200 );
			silhouette->hasTriangle = true;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofBackground(255, 255, 255);

	for(int i = 0; i < receiver->totalPeople(); i++){
		ofxTSPSPerson* person = receiver->personAtIndex(i);
		ofxInteractiveSilhouette* silhouette = (ofxInteractiveSilhouette*)person->customAttributes;
		if(silhouette->hasTriangle){
			silhouette->triangle.draw();
		}
		
		ofSetColor(silhouette->color.x, silhouette->color.y, silhouette->color.z);
		for(int c = 1; c < person->contour.size(); c++){
			ofLine(person->contour[c-1].x*ofGetWidth(), person->contour[c-1].y*ofGetHeight(), 
				   person->contour[c  ].x*ofGetWidth(), person->contour[c  ].y*ofGetHeight());
		}
	}			
}

//called when the person enters the system
void testApp::personEntered( ofxTSPSPerson* person, ofxTSPSScene* scene )
{
	//put a color into the custom attributes field
	ofxInteractiveSilhouette* silhouette = new ofxInteractiveSilhouette();
	silhouette->color = ofPoint(ofRandom(0, 255),ofRandom(0, 255),ofRandom(0, 255));
	person->customAttributes = silhouette;
	
}

//called each time the centroid moves (a lot)
void testApp::personMoved( ofxTSPSPerson* person, ofxTSPSScene* scene )
{
}

//called one frame before the person is removed from the list to let you clean up
void testApp::personWillLeave( ofxTSPSPerson* person, ofxTSPSScene* scene )
{
	delete (ofxInteractiveSilhouette*)person->customAttributes;
}

//called every frame no matter what.
void testApp::personUpdated(ofxTSPSPerson* person, ofxTSPSScene* scene)
{
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

