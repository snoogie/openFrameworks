#ifndef OF_SHAPE
#define OF_SHAPE

#include "ofConstants.h"

#ifdef TARGET_OSX
	#include <OpenGL/glu.h>
#endif

#ifdef TARGET_OPENGLES
	#include "glu.h"
#endif

#ifdef TARGET_LINUX
	#include "GL/glu.h"
#endif

#ifdef TARGET_WIN32
	#include "glu.h"
#endif

#ifndef TARGET_WIN32
    #define CALLBACK
#endif

//----------------------------------------------------------
// ofShape
//----------------------------------------------------------

// ofShape  tesselation callbacks



class ofShape{
public:
	ofShape(){
		tesselator = NULL;
		//id=lastid++;
		id = -1;
		//TODO: implement id in .cpp
		polyMode = OF_POLY_WINDING_ODD;
		drawMode = OF_OUTLINE;
		curveResolution = 20;
		noFill();
	}
	virtual ~ofShape(){}

	void setCurveResolution(float _curveResolution){
		curveResolution = _curveResolution;
	}

	void beginShape(int _polyMode = OF_POLY_WINDING_ODD){
		clearVertices();
		clearTessVertices();
		polyMode = _polyMode;
	}


	void vertex(const ofPoint & vertex){
		points.push_back(vertex);

		double* point = new double[3];
		point[0] = vertex.x;
		point[1] = vertex.y;
		point[2] = vertex.z;
		newVertices.push_back(point);


		clearCurveVertices();	// we drop any "curve calls"
								// once a vertex call has been made
								// ie,
								// you can't mix
								// ofCurveVertex();
								// ofCurveVertex();
								// ofVertex();
								// etc...
								// and you need 4 calls
								// to curve to see something...
	}

	void vertex(float x, float y, float z=0){

		vertex(ofPoint(x,y,z));
	}




	void curveVertex(const ofPoint & point){

		/*curveVertices.push_back(point);

		if (curveVertices.size() >= 4){

			int startPos = (int)curveVertices.size() - 4;

			ofPoint p0 = curveVertices[startPos + 0];
			ofPoint p1 = curveVertices[startPos + 1];
			ofPoint p2 = curveVertices[startPos + 2];
			ofPoint p3 = curveVertices[startPos + 3];


			float t,t2,t3;
			ofPoint vertex;

			for (int i = 0; i < curveResolution; i++){

				t 	=  (float)i / (float)(curveResolution-1);
				t2 	= t * t;
				t3 	= t2 * t;

				vertex.x = 0.5f * ( ( 2.0f * p1.x ) +
				( -p0.x + p2.x ) * t +
				( 2.0f * p0.x - 5.0f * p1.x + 4 * p2.x - p3.x ) * t2 +
				( -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x ) * t3 );

				vertex.y = 0.5f * ( ( 2.0f * p1.y ) +
				( -p0.y + p2.y ) * t +
				( 2.0f * p0.y - 5.0f * p1.y + 4 * p2.y - p3.y ) * t2 +
				( -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y ) * t3 );

				vertex.z = 0.5f * ( ( 2.0f * p1.z ) +
				( -p0.z + p2.z ) * t +
				( 2.0f * p0.z - 5.0f * p1.z + 4 * p2.z - p3.z ) * t2 +
				( -p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z ) * t3 );


				points.push_back(vertex);

				double* point = new double[3];
				point[0] = vertex.x;
				point[1] = vertex.y;
				point[2] = vertex.z;
				newVertices.push_back(point);
			}
		}*/
	}

	//TODO: 3d
	/*void curveVertex(float x, float y, float z=0){
		curveVertex(ofPoint(x,y,z));


	}*/
	void curveVertex(float x, float y, float z=0){

		double* point = new double[3];
	 	point[0] = x;
		point[1] = y;
		point[2] = 0;
	 	curveVertices.push_back(point);

	 	if (curveVertices.size() >= 4){

	 		int startPos = (int)curveVertices.size() - 4;

	 		float x0 = curveVertices[startPos + 0][0];
		   	float y0 = curveVertices[startPos + 0][1];
	 		float x1 = curveVertices[startPos + 1][0];
		   	float y1 = curveVertices[startPos + 1][1];
	 		float x2 = curveVertices[startPos + 2][0];
		   	float y2 = curveVertices[startPos + 2][1];
	 		float x3 = curveVertices[startPos + 3][0];
		   	float y3 = curveVertices[startPos + 3][1];

	 		int resolution = curveResolution;

			float t,t2,t3;
			float x,y;

			for (int i = 0; i < resolution; i++){

				t 	=  (float)i / (float)(resolution-1);
				t2 	= t * t;
				t3 	= t2 * t;

				x = 0.5f * ( ( 2.0f * x1 ) +
				( -x0 + x2 ) * t +
				( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
				( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

				y = 0.5f * ( ( 2.0f * y1 ) +
				( -y0 + y2 ) * t +
				( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
				( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

				/*double* newPoint = new double[3];
				newPoint[0] = x;
				newPoint[1] = y;
				newPoint[2] = 0;
				polyVertices.push_back(newPoint);*/

				points.push_back(ofPoint(x,y,0));

				double* newPoint = new double[3];
				newPoint[0] = x;
				newPoint[1] = y;
				newPoint[2] = 0;
				newVertices.push_back(newPoint);
			}
	 	}

	}
	//TODO: 3d and call with 3 points
	void bezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){


		clearCurveVertices();	// we drop any stored "curve calls"


		// if, and only if poly vertices has points, we can make a bezier
		// from the last point

		// the resolultion with which we computer this bezier
		// is arbitrary, can we possibly make it dynamic?

		if (newVertices.size() > 0){

			float x0 = newVertices[newVertices.size()-1][0];
			float y0 = newVertices[newVertices.size()-1][1];

			float   ax, bx, cx;
			float   ay, by, cy;
			float   t, t2, t3;
			float   x, y;

			// polynomial coefficients
			cx = 3.0f * (x1 - x0);
			bx = 3.0f * (x2 - x1) - cx;
			ax = x3 - x0 - cx - bx;

			cy = 3.0f * (y1 - y0);
			by = 3.0f * (y2 - y1) - cy;
			ay = y3 - y0 - cy - by;

			// arbitrary ! can we fix??
			int resolution = curveResolution;

			for (int i = 0; i < resolution; i++){
				t 	=  (float)i / (float)(resolution-1);
				t2 = t * t;
				t3 = t2 * t;
				x = (ax * t3) + (bx * t2) + (cx * t) + x0;
				y = (ay * t3) + (by * t2) + (cy * t) + y0;
				vertex(x,y);
			}


		}


	}

	void endShape(bool bClose){
		bIsConvex = isConvex();

		// (close -> add the first point to the end)
		// -----------------------------------------------
		if ((bClose == true)){
			//---------------------------
			if ((int)points.size() > 0 && points.back() != points[0]){
				vertex(points[0]);
			}
		}


		ofLog(OF_LOG_VERBOSE,"ending a shape of %i points",points.size());
		if(((polyMode == OF_POLY_WINDING_ODD) && (drawMode == OF_OUTLINE)) || bIsConvex){
			tesselation tess;
			tess.shapeType = (drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_STRIP;
			tessVertices.push_back(tess);
			for(unsigned i=0; i<points.size(); i++){
				ofLog(OF_LOG_VERBOSE,"generating point: %i: %.02f,%.02f",i,points[i].x,points[i].y);
				tessVertices[0].tessVertices.push_back( points[i].x );
				tessVertices[0].tessVertices.push_back( points[i].y );
				tessVertices[0].tessVertices.push_back( points[i].z );
			}
		}else{
			startTess();
			if ( tesselator != NULL){
				gluTessBeginContour( tesselator);
				for (unsigned i=0; i<points.size(); i++) {
					ofLog(OF_LOG_VERBOSE,"generating point: %i",i);
					gluTessVertex( tesselator, newVertices[i], newVertices[i]);
				}
				gluTessEndContour( tesselator);

			}
			endTess();

			//clearVertices();
		}

		uploadVBO();
	}

	//TODO: nextContour

	void draw(){
		//TODO: this should go in ofGraphics?? if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();

		//VBO
		for(unsigned i=0;i<tessVertices.size();i++){
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, tessVertices[i].vboId);
			glEnableClientState(GL_VERTEX_ARRAY);                 // activate vertex coords array
			glVertexPointer(3, GL_FLOAT, 0, 0);                   // last param is offset, not ptr
			glDrawArrays(tessVertices[i].shapeType, 0, tessVertices[i].tessVertices.size()/3);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}

		//Vertex array
		/*for(unsigned i=0;i<tessVertices.size();i++){
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, tessVertices[i].tessVertices.data());
			glDrawArrays(tessVertices[i].shapeType, 0, tessVertices[i].tessVertices.size()/3);
		}*/


		//if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();
	}

	void fill(){
		drawMode = OF_FILLED;
	}

	void noFill(){
		drawMode = OF_OUTLINE;
	}

	void translate(float x, float y, float z=0){
		translate(ofPoint(x,y,z));
	}

	void translate(const ofPoint & offset){
		for(unsigned i=0; i<tessVertices.size(); i++){
			for(unsigned j=0; j<tessVertices[i].tessVertices.size()/3; j++){
				tessVertices[i].tessVertices[j*3]+=offset.x;
				tessVertices[i].tessVertices[j*3+1]+=offset.y;
				tessVertices[i].tessVertices[j*3+2]+=offset.z;
			}
		}
		uploadVBO();
	}

	void scale(float x, float y, float z=1){
		scale(ofPoint(x,y,z));
	}

	void scale(const ofPoint & scale){
		for(unsigned i=0; i<tessVertices.size(); i++){
			for(unsigned j=0; j<tessVertices[i].tessVertices.size()/3; j++){
				tessVertices[i].tessVertices[j*3]*=scale.x;
				tessVertices[i].tessVertices[j*3+1]*=scale.y;
				tessVertices[i].tessVertices[j*3+2]*=scale.z;
			}
		}
		uploadVBO();
	}

	//TODO: rotate

	//TODO: check for 3d, polyhedra?
	bool isConvex()
	{
	  if (points.size() < 3) return false;

	  ofPoint p;
	  ofPoint v;
	  ofPoint u;
	  int res = 0;
	  for (unsigned i = 0; i < points.size(); i++)
	  {
		p = points.at(i);
		ofPoint tmp = points.at((i+1) % points.size());
		v.x = tmp.x - p.x;
		v.y = tmp.y - p.y;
		u = points.at((i+2) % points.size());

		if (i == 0) // in first loop direction is unknown, so save it in res
		  res = u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x;
		else
		{
		  int newres = u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x;
		  if ( (newres > 0 && res < 0) || (newres < 0 && res > 0) )
			return false;
		}
	  }
	  return true;
	}

	vector<ofPoint> points;
	int id;



private:
	struct tesselation{
		tesselation(){
			bIsVBOUploaded = false;
		}

		~tesselation(){
			if(bIsVBOUploaded)
				glDeleteBuffersARB(1, &vboId);
		}

		void uploadVBO(){
			if(bIsVBOUploaded)
				glDeleteBuffersARB(1, &vboId);
			glGenBuffersARB(1, &vboId);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*tessVertices.size(), tessVertices.data(), GL_STATIC_DRAW_ARB);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			bIsVBOUploaded = true;
		}

		vector<float> tessVertices;
		GLint shapeType;
		GLuint vboId;
		bool bIsVBOUploaded;
	};

	int drawMode;
	vector <double*> newVertices;
	vector <double*> curveVertices;
	vector < tesselation > tessVertices;
	bool bIsConvex;
	int polyMode;
	GLUtesselator * tesselator;
	int curveResolution;


	//static int lastid;

	void uploadVBO(){
		for(unsigned i=0; i<tessVertices.size(); i++){
			tessVertices[i].uploadVBO();
		}
	}

	void startTess(){

		// just clear the vertices, just to make sure that
		// someone didn't do something improper, like :
		// a) ofBeginShape()
		// b) ofVertex(), ofVertex(), ofVertex() ....
		// c) ofBeginShape()
		// etc...

		clearTessVertices();


		// now get the tesselator object up and ready:

		tesselator = gluNewTess();


		// --------------------------------------------------------
		// note: 	you could write your own begin and end callbacks
		// 			if you wanted to...
		// 			for example, to count triangles or know which
		// 			type of object tess is giving back, etc...
		// --------------------------------------------------------

		#if defined( TARGET_OSX)
			#ifndef MAC_OS_X_VERSION_10_5
				#define OF_NEED_GLU_FIX
			#endif
		#endif

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// MAC - XCODE USERS PLEASE NOTE - some machines will not be able to compile the code below
		// if this happens uncomment the "OF_NEED_GLU_FIX" line below and it
		// should compile also please post to the forums with the error message, you OS X version,
		// Xcode verison and the CPU type - PPC or Intel. Thanks!
		// (note: this is known problem based on different version of glu.h, we are working on a fix)
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		//#define OF_NEED_GLU_FIX

		#ifdef OF_NEED_GLU_FIX
			#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)(...)
		#else
			#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)()
		#endif

		gluTessCallback( tesselator, GLU_TESS_BEGIN_DATA, OF_GLU_CALLBACK_HACK)&tessBegin);
		gluTessCallback( tesselator, GLU_TESS_VERTEX_DATA, OF_GLU_CALLBACK_HACK)&tessVertex);
		gluTessCallback( tesselator, GLU_TESS_COMBINE_DATA, OF_GLU_CALLBACK_HACK)&tessCombine);
		gluTessCallback( tesselator, GLU_TESS_END_DATA, OF_GLU_CALLBACK_HACK)&tessEnd);
		gluTessCallback( tesselator, GLU_TESS_ERROR_DATA, OF_GLU_CALLBACK_HACK)&tessError);

		gluTessProperty( tesselator, GLU_TESS_WINDING_RULE, polyMode);
		if (drawMode == OF_OUTLINE){
			gluTessProperty( tesselator, GLU_TESS_BOUNDARY_ONLY, true);
		} else {
			gluTessProperty( tesselator, GLU_TESS_BOUNDARY_ONLY, false);
		}
		gluTessProperty( tesselator, GLU_TESS_TOLERANCE, 0);

		/* ------------------------------------------
		for 2d, this next call (normal) likely helps speed up ....
		quote : The computation of the normal represents about 10% of
		the computation time. For example, if all polygons lie in
		the x-y plane, you can provide the normal by using the
		-------------------------------------------  */

		gluTessNormal(tesselator, 0.0, 0.0, 1.0);
		gluTessBeginPolygon( tesselator, this);

	}

	void endTess(){
		if ( tesselator != NULL){
			// no matter what we did / do, we need to delete the tesselator object
			gluTessEndPolygon( tesselator);
			gluDeleteTess( tesselator);
			tesselator = NULL;
		}
	}

	void clearVertices(){
		points.clear();
		for(vector<double*>::iterator itr=newVertices.begin();
			itr!=newVertices.end();
			++itr){
			delete [] (*itr);
		}
		newVertices.clear();


		clearCurveVertices();

	}

	void clearTessVertices(){
	    tessVertices.clear();
	}

	void clearCurveVertices(){
		// combine callback also makes new vertices, let's clear them:
	   /* for(vector<double*>::iterator itr=curveVertices.begin();
	        itr!=curveVertices.end();
	        ++itr){
	        delete [] (*itr);
	    }*/
	    curveVertices.clear();
	}

	//----------------------------------------------------------
	static void CALLBACK tessError(GLenum errCode, void * shape_ptr){
		ofShape * shape = (ofShape*)shape_ptr;
		const GLubyte* estring;
		estring = gluErrorString( errCode);
		ofLog(OF_LOG_ERROR, "tessError: %s for shape: %i", estring, shape->id);
	}


	//----------------------------------------------------------
	static void CALLBACK tessBegin(GLint type, void * shape_ptr){
		ofShape * shape = (ofShape*)shape_ptr;
		tesselation tess;
		tess.shapeType = type;
		shape->tessVertices.push_back(tess);
	}

	//----------------------------------------------------------
	static void CALLBACK tessEnd(void * shape_ptr){
		ofShape * shape = (ofShape*)shape_ptr;
		ofLog(OF_LOG_VERBOSE, "tesselation generated " + ofToString((int)shape->tessVertices.size()) + " shapes");
	}


	//----------------------------------------------------------
	static void CALLBACK tessVertex( void* data, void * shape_ptr){
		ofShape * shape = (ofShape*)shape_ptr;
		shape->tessVertices.back().tessVertices.push_back( ( (double*)data )[0] );
		shape->tessVertices.back().tessVertices.push_back( ( (double*)data )[1] );
		shape->tessVertices.back().tessVertices.push_back( ( (double*)data )[2] );	//No need for z for now?
	}


	//----------------------------------------------------------
	static void CALLBACK tessCombine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData, void * shape_ptr){
		ofShape * shape = (ofShape*)shape_ptr;
	    double* vertex = new double[3];
	    vertex[0] = coords[0];
	    vertex[1] = coords[1];
	    vertex[2] = coords[2];
	    *outData = vertex;
	}

};

//int ofShape::lastid=0;

#endif
