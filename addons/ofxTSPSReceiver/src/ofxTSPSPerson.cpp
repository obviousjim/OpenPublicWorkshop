/*
 *  ofxTSPSPerson.cpp
 *
 *  Adapted by James George on 11/21/09.
 *  Copyright 2009 (http://www.jamesgeorge.org) All rights reserved.
 *
 */
#include "ofxTSPSPerson.h"


ofxTSPSPerson::ofxTSPSPerson(int pid, int oid)
:	pid(pid),
	oid(oid),
	age(0),
	opticalFlowVectorAccumulation(ofPoint(0,0)),
	centroid(ofPoint(0,0)),
	customAttributes(NULL)
{};

ofxTSPSPerson::~ofxTSPSPerson()
{

}

void ofxTSPSPerson::update(ofxOscMessage* m)
{
	age = m->getArgAsInt32(1);
	centroid.x = m->getArgAsFloat(2);
	centroid.y = m->getArgAsFloat(3);
	velocity.x = m->getArgAsFloat(4);
	velocity.y = m->getArgAsFloat(5);
	boundingRect.x = m->getArgAsFloat(6);
	boundingRect.y = m->getArgAsFloat(7);
	boundingRect.width= m->getArgAsFloat(8);
	boundingRect.height = m->getArgAsFloat(9);
	opticalFlowVelocity.x = m->getArgAsFloat(10);
	opticalFlowVelocity.y = m->getArgAsFloat(11);	
	
	if (m->getNumArgs() > 12){
		contour.clear();
		for (int i = 12; i < m->getNumArgs(); i += 2){
			contour.push_back(ofPoint(m->getArgAsFloat(i), m->getArgAsFloat(i+1)));
		}
	}
	
	updatedAt = ofGetElapsedTimeMillis();
}

ofPoint ofxTSPSPerson::denormalizedCentroid(float width, float height)
{
	return ofPoint(centroid.x*width, centroid.y*height);
}

vector<ofPoint> ofxTSPSPerson::denormalizedContour(float width, float height)
{

	vector<ofPoint> denormalized;
	for( int i = 0; i < contour.size(); i++){
		ofPoint p;
		p.x = contour[i].x*width;
		p.y = contour[i].y*height;
		denormalized.push_back(p);
	}
	
	cout << "num points in contour " << denormalized.size() << endl;
	return denormalized;
}

