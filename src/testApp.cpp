#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	//basic drawing stuff
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	
	//set up the receiver to listen for people
	receiver = new ofxTSPSReceiver();
	receiver->setListener( this );
	receiver->connect(12000);
	
	//set up the box2d environment
	physics.init();
	physics.createBounds();
	physics.setGravity(0, 10);
	physics.checkBounds(true);
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	for (int i = polys.size()-1; i >= 0; i--) {
		physics.world->DestroyBody(polys[i].body);
	}
	
	polys.clear();
	
	for(int i = 0; i < receiver->totalPeople(); i++){

		//get the person and their cv contour
		ofxTSPSPerson* person = receiver->personAtIndex(i);
		ofxInteractiveSilhouette* silhouette = (ofxInteractiveSilhouette*)person->customAttributes;
		vector<ofPoint> contourToTriangulate = person->denormalizedContour(ofGetWidth(), ofGetHeight());
		
		//if we have a valid poly...
		if(contourToTriangulate.size() > 2){
			//clear the existing triangle set for that person and create a new one
			silhouette->triangle.clear();
			silhouette->triangle.triangulate( contourToTriangulate, 50 );
			silhouette->hasTriangle = true;
			
			//for all the triangles add them to the physics world
			for(int t = silhouette->triangle.triangles.size()-1; t >= 0; t--) {
				ofxTriangleData& tData = silhouette->triangle.triangles[t];
				
				ofxBox2dPolygon poly;
				poly.addVertex(tData.a.x, tData.a.y);
				poly.addVertex(tData.b.x, tData.b.y);
				poly.addVertex(tData.c.x, tData.c.y);
				
				if(poly.validateShape()) {
					poly.createShape(physics.world, 0.0f, 0.0f);
					polys.push_back(poly);
				}
			}
		}
	}

	physics.update();
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
	
	for(int i = 0; i < circles.size(); i++){
		circles[i].draw();
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

	ofxBox2dCircle circle;
	circle.setPhysics(3.0, 0.53, 0.1);
	circle.setup(physics.getWorld(), mouseX, mouseY, 20);
	circles.push_back(circle);
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

