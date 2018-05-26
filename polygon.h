#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"
#include "viewcontext.h"
#include "matrix.h"
#include "gcontext.h"
#include <iostream>

class Polygon : public Shape {

	public:
		
		Polygon( );
		
		Polygon( unsigned int color, matrix& points);
		
		Polygon( const Polygon& copyPolygon );
		
		~Polygon();
		
		void draw( GraphicsContext* gc, ViewContext* view = NULL);
		
        void out(std::ostream& os) const;
        
        void in(std::istream& is);
        
        Polygon* clone();
        
	protected:
	
	    matrix points = matrix(3,4);
	    int size = 3;
		    
};


#endif
