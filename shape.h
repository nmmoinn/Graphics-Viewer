#ifndef SHAPE_H
#define SHAPE_H

#include "matrix.h"
#include "gcontext.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include "viewcontext.h"
#include <string>

using namespace std;

class shapeException:public std::runtime_error {
	public:
		shapeException(std::string message):
		      std::runtime_error((std::string("Shape Exception: ") + 
		               message).c_str()) {}
};

class Shape {

	public:

		static const unsigned int BLACK = 0x000000;
		static const unsigned int BLUE = 0x0000FF;
		static const unsigned int GREEN = 0x00FF00;
		static const unsigned int RED = 0xFF0000;
		static const unsigned int CYAN = 0x00FFFF;
		static const unsigned int MAGENTA = 0xFF00FF;
		static const unsigned int YELLOW = 0xFFFF00;
		static const unsigned int GRAY = 0x808080;
		static const unsigned int WHITE = 0xFFFFFF;
		
		Shape( );
		
    		Shape( unsigned int color, matrix& origin );
    		
    		Shape( const Shape& shape );
    		
		virtual ~Shape();
		
		void setColor( unsigned int color );

		virtual void draw( GraphicsContext* gc, ViewContext* view = NULL) = 0;
        
        virtual void out(std::ostream& os) const;
        
        virtual void in(std::istream& is) = 0;
        
        virtual Shape* clone() = 0;
        
	protected:
		
		unsigned int color = WHITE;
		matrix origin = matrix(4,1);
		    
};

vector<string> split(const string& s, char delimiter);

vector<string> removeGarbage( vector<string> trim );

#endif
