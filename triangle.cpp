
#include "triangle.h"
#include "shape.h"
#include "gcontext.h"
#include "matrix.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "x11context.h"

using namespace std;

Triangle::Triangle( ) {}

Triangle::Triangle( unsigned int color, matrix& origin, matrix& midPoint, matrix& endPoint) : Shape( color, origin ) {
    
    if( midPoint.getCols() != 4 ) {
        throw shapeException("mid/endPoint for triangle object requires a 1x4 matrix of the form {x,y,z,1}");
    } else {
        this->midPoint = matrix( midPoint );
    }
    
    if( endPoint.getCols() != 4 ) {
        throw shapeException("mid/endPoint for triangle object requires a 1x4 matrix of the form {x,y,z,1}");
    } else {
        this->endPoint = matrix(4,1,endPoint[0]);
    }    
}

Triangle::Triangle( unsigned int color, matrix& points ) : Shape( color, points )   {

    if( points.getSize() != 12 ) {
        throw shapeException("points for triangle object requires an 3x4 matrix of the form {x,y,z,1}");
    } else {
        this->points = matrix( ~points );
        this->midPoint = matrix(4,1,points[1]);
        this->endPoint = matrix(4,1,points[2]);  
    }
    
}

Triangle::Triangle( const Triangle& copyTriangle ) : Shape( copyTriangle ) {
    this->midPoint = matrix( copyTriangle.midPoint );
    this->endPoint = matrix( copyTriangle.endPoint );
    this->points = matrix( copyTriangle.points );
}

Triangle::~Triangle() {
}


void Triangle::draw( GraphicsContext* gc, ViewContext* view ) {

	gc->setColor( color );
	
	if( view != NULL ) {
        
        matrix points( view->transform( this->points ) );    
        
        gc->drawLine( points[0][0], points[1][0], points[0][1], points[1][1] );  
        gc->drawLine( points[0][1], points[1][1], points[0][2], points[1][2] );  
        gc->drawLine( points[0][2], points[1][2], points[0][0], points[1][0] );  
	
	} else {

        gc->drawLine( origin[0][0], origin[0][1], midPoint[0][0], midPoint[0][1] );
        gc->drawLine( midPoint[0][0], midPoint[0][1], endPoint[0][0], endPoint[0][1] );
        gc->drawLine( endPoint[0][0], endPoint[0][1], origin[0][0], origin[0][1] );
    }
}

void Triangle::out(std::ostream& os) const {

    //print all line values formatted properly
    os << "triangle" << endl;
    this->Shape::out(os);
	os << "midPoint " << ~midPoint;
	os << "endPoint " << ~endPoint;
	os << "endtriangle" << endl;
}

Triangle* Triangle::clone() {
    return new Triangle( *this );
}

void Triangle::in( std::istream& inStream ) {
   
    /*if( inStream.fail() ) {
        throw shapeException("istream for file is NULL in Triangle");
    }*/
    
    string line;
    string word;
    vector<string> words;
    std::istringstream wordStream(line);
    
    while(!inStream.eof()) {
    
        getline(inStream,line);
        
        words = split( line, ' ' );
        words = removeGarbage(words);
   
        for ( uint32_t i = 0; i < words.size(); i++ ) {
       
            if( words[i] == "color" ) {
                color = (unsigned int)atoi( words[i+1].c_str() );
            }
            
            if( words[i] == "origin" ) {
                points[0][0] = atof( words[i+1].c_str() );
                points[1][0] = atof( words[i+2].c_str() );
                points[2][0] = atof( words[i+3].c_str() );
                points[3][0] = atof( words[i+4].c_str() );
                this->origin = matrix( 4,1, (~points)[0] );
            }
            
            if( words[i] == "midPoint" ) {
                points[0][1] = atof( words[i+1].c_str() );
                points[1][1] = atof( words[i+2].c_str() );
                points[2][1] = atof( words[i+3].c_str() );
                points[3][1] = atof( words[i+4].c_str() );
                this->midPoint = matrix( 4,1,(~points)[1]);
            }
            
            if( words[i] == "endPoint" ) {
                points[0][2] = atof( words[i+1].c_str() );
                points[1][2] = atof( words[i+2].c_str() );
                points[2][2] = atof( words[i+3].c_str() );
                points[3][2] = atof( words[i+4].c_str() );
                this->endPoint = matrix( 4,1,(~points)[2]);
            }
        }
    }
}  

















