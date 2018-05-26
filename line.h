#ifndef LINE_H
#define LINE_H


#include "shape.h"
#include "matrix.h"
#include "gcontext.h"
#include "viewcontext.h"
#include <iostream>

class Line : public Shape {

	public:
	
	    Line( );
	
		Line( unsigned int color, matrix& origin, matrix& endPoint);
		
		Line( unsigned int color, matrix& points);
		
		Line( const Line& line );
		
		~Line();

		void draw( GraphicsContext* gc, ViewContext* view = NULL);
		void drawNM( GraphicsContext* gc, ViewContext* view = NULL);
		
        void out(std::ostream& os) const;
        
        void in(std::istream& is);
        
        Line* clone();
        
	protected:
	
	    matrix endPoint = matrix(4,1);
	    matrix points = matrix(4,2);
		    
};

#endif
