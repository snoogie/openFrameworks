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
		currentStartVertex=0;
		polyMode = OF_POLY_WINDING_ODD;
		drawMode = OF_OUTLINE;
		noFill();
	}
	virtual ~ofShape(){}


	void beginShape(int _polyMode = OF_POLY_WINDING_ODD){
		clearVertices();
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

	void endShape(bool bClose){
		bIsConvex = isConvex();

		// TODO: Not working! (close -> add the first point to the end)
		// -----------------------------------------------
		if ((bClose == true)){
			//---------------------------
			if ((int)points.size() > currentStartVertex && points.back() != points[currentStartVertex]){
				vertex(points[currentStartVertex]);
			}
		}


		ofLog(OF_LOG_VERBOSE,"ending a shape of %i points",points.size());
		if(((polyMode == OF_POLY_WINDING_ODD) && (drawMode == OF_OUTLINE)) || bIsConvex){
			tesselation tess;
			tess.shapeType = (drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_STRIP;;
			tessVertices.push_back(tess);
			for(unsigned i=0; i<points.size(); i++){
				ofPoint point;
				tessVertices[0].tessVertices[i*3]   = point.x;
				tessVertices[0].tessVertices[i*3+1] = point.y;
				tessVertices[0].tessVertices[i*3+2] = point.z;
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

			clearVertices();
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
			glDrawArrays(tessVertices[i].shapeType, 0, tessVertices[i].tessVertices.size());
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}

		//Vertex array
		/*for(unsigned i=0;i<tessVertices.size();i++){
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, &(tessVertices[i].tessVertices[0]));
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
		uploadVBO();
	}

	void translate(const ofPoint & offset){
		for(unsigned i=0; i<points.size(); i++){
			points[i] += offset;
		}
		uploadVBO();
	}

	void scale(float x, float y, float z=1){
		scale(ofPoint(x,y,z));
		uploadVBO();
	}

	void scale(const ofPoint & scale){
		for(unsigned i=0; i<points.size(); i++){
			points[i] *= scale;
			newVertices[i][0] *= scale.x;
			newVertices[i][1] *= scale.y;
			newVertices[i][2] *= scale.z;
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
		vector<float> tessVertices;
		GLint shapeType;
		GLuint vboId;
	};
	int drawMode;
	vector <double*> newVertices;
	vector <double*> curveVertices;
	vector < tesselation > tessVertices;
	int currentStartVertex;
	bool bIsConvex;
	GLint shapeType;
	int polyMode;
	GLUtesselator * tesselator;
	GLuint vboId;
	bool bIsVBOUploaded;


	//static int lastid;

	void uploadVBO(){
		for(int i=0; i<tessVertices.size(); i++){
			if(bIsVBOUploaded){
				glDeleteBuffersARB(1, &(tessVertices[i].vboId));
			}
			glGenBuffersARB(1, &(tessVertices[i].vboId));
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, tessVertices[i].vboId);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*tessVertices[i].tessVertices.size(), &(tessVertices[i].tessVertices[0]), GL_STATIC_DRAW_ARB);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}
		bIsVBOUploaded = true;
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

		// combine callback also makes new vertices, let's clear them:
		for(vector<double*>::iterator itr=newVertices.begin();
			itr!=newVertices.end();
			++itr){
			delete [] (*itr);
		}
		newVertices.clear();

		clearCurveVertices();

		currentStartVertex = 0;
	}

	void clearTessVertices(){
	    tessVertices.clear();

	}

	void clearCurveVertices(){
		// combine callback also makes new vertices, let's clear them:
	    for(vector<double*>::iterator itr=curveVertices.begin();
	        itr!=curveVertices.end();
	        ++itr){
	        delete [] (*itr);
	    }
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
