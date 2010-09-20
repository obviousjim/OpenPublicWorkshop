/*
 *  ofxInteractiveSilhouette.h
 *  PedestrianVisualizer
 *
 *  Created by James on 9/20/10.
 *  Copyright 2010 FlightPhase. All rights reserved.
 *
 */
#ifndef OFX_INTERACTIVE_SILHOUETTE
#define OFX_INTERACTIVE_SILHOUETTE

#include "ofMain.h"
#include "ofxTriangle.h"

class ofxInteractiveSilhouette
{
  public:
	ofxInteractiveSilhouette();
	~ofxInteractiveSilhouette();
	bool hasTriangle;
	ofxTriangle triangle;
	ofPoint color;
	
};

#endif