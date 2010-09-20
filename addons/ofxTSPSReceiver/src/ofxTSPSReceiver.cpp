/*
 *  ofxTSPSReceiver.cpp
 *  Hopscotch Confetti
 *
 *  Created by Brett Renfer on 3/8/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 */

#include "ofxTSPSReceiver.h"

ofxTSPSReceiver::ofxTSPSReceiver(){
	bSetup = false;
	eventListener = NULL;
	personTimeout = 3.5 * 1000; // in millis
}

void ofxTSPSReceiver::connect( int port ){
	setup(port);
	bSetup = true;
}

void ofxTSPSReceiver::setListener(ofxTSPSListener* listener)
{
	eventListener = listener;
	ofAddListener(ofEvents.update, this, &ofxTSPSReceiver::update);
}

void ofxTSPSReceiver::draw(int width, int height){
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(.5);
	for(int i = 0; i < totalPeople(); i++){
		ofxTSPSPerson* p = personAtIndex(i);
		ofSetColor(0xffffff);
		ofRect(p->boundingRect.x*width, p->boundingRect.y*height, p->boundingRect.width*width, p->boundingRect.height*height);
		ofSetColor(0xff0000);
		ofCircle((p->centroid.x*width)-2, (p->centroid.y*height)-2, 4);
		ofSetColor(0xffff00);
		ofBeginShape();
		for (int j=0; j<p->contour.size(); j++){
			ofVertex(p->contour[j].x*width, p->contour[j].y*height);
		}
		ofEndShape();
	}
	ofPopStyle();
}

ofxTSPSScene* ofxTSPSReceiver::getScene()
{
	return &scene;
}

void ofxTSPSReceiver::update( ofEventArgs &e ){
	
	if(hasWaitingMessages()){
		// check for waiting messages
		while( hasWaitingMessages() ){
			// get the next message
			ofxOscMessage* m = new ofxOscMessage();
			getNextMessage( m );
			
			if(m->getAddress() == "TSPS/scene/") {
				scene.idleTime = m->getArgAsFloat(0);
				scene.percentCovered = m->getArgAsFloat(1);
			}
			
			
			if (m->getAddress() == "TSPS/personEntered/" || 
				m->getAddress() == "TSPS/personMoved/" || 
				m->getAddress() == "TSPS/personUpdated/" || 
				m->getAddress() == "TSPS/personWillLeave/"){
				
				int id = m->getArgAsInt32(0);
				bool personIsNew = false;
				ofxTSPSPerson* person = personWithID( id );
				if(person == NULL){
					person = new ofxTSPSPerson(id, trackedPeople.size());
					trackedPeople.push_back(person);
					personIsNew = true;
				}
				person->update( m );
				
				if(eventListener != NULL){
					if (m->getAddress() == "TSPS/personEntered/" || personIsNew){
						eventListener->personEntered(person, &scene);
					}
					else if (m->getAddress() == "TSPS/personMoved/"){
						eventListener->personMoved(person, &scene);
					}
					else if (m->getAddress() == "TSPS/personUpdated/"){
						eventListener->personUpdated(person, &scene);
						
					}
					else if (m->getAddress() == "TSPS/personWillLeave/"){
						eventListener->personWillLeave(person, &scene);					
					}
				}
				
				if(m->getAddress() == "TSPS/personWillLeave/"){
					for (int i = trackedPeople.size() - 1; i >= 0; i--){
						if (trackedPeople[i]->pid == person->pid){
							trackedPeople.erase(trackedPeople.begin() + i);
							break;
						}
					}
					delete person;				
				}
				delete m;
			}		
		}
	}

	
	//clear orphaned blobs
	for(int i = trackedPeople.size()-1; i >= 0; i--) {
		if(ofGetElapsedTimeMillis()-trackedPeople[i]->updatedAt > personTimeout) {
			trackedPeople.erase(trackedPeople.begin() + i);
			eventListener->personWillLeave(trackedPeople[i], &scene);
		}
	}	
}


//callbacks to get people
ofxTSPSPerson* ofxTSPSReceiver::personAtIndex(int i)
{
	return trackedPeople[i];
}

ofxTSPSPerson* ofxTSPSReceiver::personWithID(int pid)
{
	for (int i = 0; i < trackedPeople.size(); i++){
		if (trackedPeople[i]->pid == pid){
			return trackedPeople[i];
		}
	}
	return NULL;
}	

int ofxTSPSReceiver::totalPeople()
{
	return trackedPeople.size();
}

