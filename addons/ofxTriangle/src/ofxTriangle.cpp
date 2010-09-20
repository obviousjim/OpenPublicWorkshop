#include "ofxTriangle.h"

#ifdef USE_OPENCV
void ofxTriangle::triangulate(ofxCvBlob &cvblob, int resolution)
{		
	triangulate(cvblob.pts, resolution);
}
#endif

void ofxTriangle::triangulate(vector<ofPoint> contour, int resolution)
{
	
    int bSize = contour.size();
    float maxi = min(resolution, bSize);
	
    Delaunay::Point tempP;
    vector< Delaunay::Point > v;
	
    for(int i = 0; i < maxi; i++) {
        int id = (int)( (float)i/maxi*bSize );
		
        tempP[0] = contour[id].x;
        tempP[1] = contour[id].y;
		
        v.push_back(tempP);
    }
	
    delobject = new Delaunay(v);
    delobject->Triangulate();

    //triangles.clear();
    //nTriangles = 0;

    Delaunay::fIterator fit;
    for ( fit = delobject->fbegin(); fit != delobject->fend(); ++fit )
    {
        int pta = delobject->Org(fit);
        int ptb = delobject->Dest(fit);
        int ptc = delobject->Apex(fit);

        int pta_id = (int)( ((float)pta/resolution)*bSize );
        int ptb_id = (int)( ((float)ptb/resolution)*bSize );
        int ptc_id = (int)( ((float)ptc/resolution)*bSize );

        ofPoint tr[3];
//        tr[0] = ofPoint(blob->pts[pta_id].x, blob->pts[pta_id].y);
//        tr[1] = ofPoint(blob->pts[ptb_id].x, blob->pts[ptb_id].y);
//        tr[2] = ofPoint(blob->pts[ptc_id].x, blob->pts[ptc_id].y);
        tr[0] = ofPoint(contour[pta_id].x, contour[pta_id].y);
        tr[1] = ofPoint(contour[ptb_id].x, contour[ptb_id].y);
        tr[2] = ofPoint(contour[ptc_id].x, contour[ptc_id].y);
		
        if( isPointInsidePolygon(&contour[0], contour.size(), getTriangleCenter(tr) ) )
        {
            ofxTriangleData td;
            td.a = ofPoint(tr[0].x, tr[0].y);
            td.b = ofPoint(tr[1].x, tr[1].y);
            td.c = ofPoint(tr[2].x, tr[2].y);

            td.area = delobject->area(fit);

            triangles.push_back(td);
			
            nTriangles++;
        }
    }
	
    delete delobject;
}

void ofxTriangle::clear()
{
    triangles.clear();
    nTriangles = 0;
}

ofPoint ofxTriangle::getTriangleCenter(ofPoint *tr)
{
    float c_x = (tr[0].x + tr[1].x + tr[2].x) / 3;
    float c_y = (tr[0].y + tr[1].y + tr[2].y) / 3;

    return ofPoint(c_x, c_y);
}

bool ofxTriangle::isPointInsidePolygon(ofPoint *polygon,int N, ofPoint p)
{
    int counter = 0;
    int i;
    double xinters;
    ofPoint p1,p2;

    p1 = polygon[0];

    for (i=1;i<=N;i++)
    {
        p2 = polygon[i % N];
        if (p.y > MIN(p1.y,p2.y)) {
            if (p.y <= MAX(p1.y,p2.y)) {
                if (p.x <= MAX(p1.x,p2.x)) {
                    if (p1.y != p2.y) {
                        xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
                        if (p1.x == p2.x || p.x <= xinters)
                            counter++;
                    }
                }
            }
        }
        p1 = p2;
    }

  if (counter % 2 == 0)
    return false;
  else
    return true;
}

void ofxTriangle::draw(float x, float y)
{
    ofPushMatrix();
    ofTranslate(x, y, 0);
        draw();
    ofPopMatrix();
}

void ofxTriangle::draw()
{
    ofFill();

    for (int i=0; i<nTriangles; i++)
    {
        ofSetColor(ofRandom(0, 0xffffff));
        ofTriangle( triangles[i].a.x, triangles[i].a.y,
                    triangles[i].b.x, triangles[i].b.y,
                    triangles[i].c.x, triangles[i].c.y);
    }

}
