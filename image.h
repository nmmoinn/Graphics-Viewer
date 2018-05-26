#ifndef IMAGE_H
#define IMAGE_H


#include "shape.h"
#include "line.h"
#include "gcontext.h"
#include "matrix.h"
#include <iostream>
#include <string>

class Image {

	public:
	
		Image();
		
		Image( const Image& image );
		
		~Image();

		void draw( GraphicsContext* gc, ViewContext* view = NULL );
		
        void add( Shape& shape );
        
        Image& operator=(const Image& rhs);
        
        void out(std::ostream& os) const;
        
        void in(std::istream& is);
        
        void erase();
        
        void stl_in(std::istream& is);
        
        string getName();
        
	protected:
	
	    vector<Shape*> shapes;
	    string name = "Image";
        Line viewOrigin[3];
        Line modelOrigin[3];
		    
};

std::ostream& operator<<(std::ostream& os, const Image& image);

void drawThreading(Shape*,GraphicsContext* gc, ViewContext* view);

#endif
