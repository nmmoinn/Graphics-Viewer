#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "shape.h"
#include "matrix.h"
#include "gcontext.h"
#include "viewcontext.h"
#include <iostream>

class Triangle : public Shape {

	public:
	
	    Triangle( );
	
		Triangle( unsigned int color, matrix& origin, matrix& midPoint, matrix& endPoint);
		
		Triangle( unsigned int color, matrix& points);
		
		Triangle( const Triangle& copyTriangle );
		
		~Triangle();

		void draw( GraphicsContext* gc, ViewContext* view = NULL);
		
        void out(std::ostream& os) const;
        
        void in(std::istream& is);
        
        Triangle* clone();
        
	protected:
	
	    matrix midPoint = matrix(1,4);
	    matrix endPoint = matrix(1,4);
	    
	    //speed up calculations by holding all points at once and precalculating transpose
	    matrix points = matrix(4,3);
		    
};


#endif
