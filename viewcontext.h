#ifndef VIEWCONTEXT_H
#define VIEWCONTEXT_H


#include "gcontext.h"
#include "matrix.h"
#include <iostream>

class ViewContext {

	public:
	    
	    //constructors, destructor, and reset
        ViewContext( );
        ViewContext( matrix& origin, int xOrient = 1, int yOrient = 1, int zOrient = 1);
		ViewContext( const ViewContext& view );
		~ViewContext();
		void reset();

		//model transforms
		void setScale( double scale);
		void scale( double scaleChange );
		void translate( double x, double y, double z );
		void rotateX( double angle );
		void rotateY( double angle );
		void rotateZ( double angle );
		
		//camera/view transforms
		void zoom( double zoomValue );
		void rotateLookUp( double angle );
		void verticalOrbit( double angle );
		void horizontalOrbit( double angle );
		
		//transform functions
		matrix transform( matrix& points );
		matrix transformNM( matrix& points );
		matrix transformINV( matrix& points );
		
		//unfinished clipping clode
    	    //int clippingCode( double x, double y, double z );
        //matrix clip( matrix& points );
        //matrix test( matrix& points );
    	    
	protected:
	
	    //internal functions
	    void updatePerspective();
	    void updateAxes();
        matrix homogenize( matrix& points );
	    
	    //viewport axes
	    double xAxis[3];
	    double yAxis[3];
	    double zAxis[3];
	    
	    //viewport transformation matricies 
	    matrix mperspective = matrix(4,4);
	    matrix mviewCenter = matrix(4,1);
	    matrix lookUp = matrix(4,1);
	    matrix mproject = matrix(4,4);
	    
	    //model transformations
	    matrix mrotate = matrix::identity(4);
	    matrix mscale = matrix::identity(4);
	    matrix mtranslate = matrix::identity(4);
	    
	    //combined transforms, saved for speed purposes
	    matrix modelFinal = matrix::identity(4);
	    matrix mfinal = matrix::identity(4);
	    
	    //device transformation
	    matrix morigin = matrix::identity(4);
};

#endif
