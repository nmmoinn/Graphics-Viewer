
#include "line.h"
#include "shape.h"
#include "gcontext.h"
#include "matrix.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

Line::Line( ) {}

Line::Line( unsigned int color, matrix& origin, matrix& endPoint) : Shape( color, origin )   {
    
     if( endPoint.getCols() != 4 ) {
        throw shapeException("endPoint for line object requires a 1x4 matrix of the form {x,y,z,1}");
    } else {
        this->endPoint = matrix( 4,1, endPoint[0] );
    }
}

Line::Line( unsigned int color, matrix& points ) : Shape( color, points )   {

    if( points.getCols() != 4 || points.getRows() != 2 ) {
        throw shapeException("points for line object requires an nx4 matrix of the form {x,y,z,1}");
    } else {
        this->endPoint = matrix( 4,1,points[1] );
        this->points = matrix( ~points );
    }
}

Line::Line( const Line& copyLine ) : Shape( copyLine ) {
    this->endPoint = matrix( copyLine.endPoint );
    this->points = matrix( copyLine.points );
}

Line::~Line() {
}


void Line::draw( GraphicsContext* gc, ViewContext* view ) {
	gc->setColor( color );
	
	if( view != NULL ) {
	
	    matrix points( view->transform( this->points ) );
	    gc->drawLine( points[0][0], points[1][0], points[0][1], points[1][1] );
	    
    } else {
        gc->drawLine( origin[0][0], origin[1][0], endPoint[0][0], endPoint[1][0] );
    }
}

void Line::drawNM( GraphicsContext* gc, ViewContext* view ) {
    gc->setColor( color );
    
    matrix points( view->transformNM( this->points ) );
    
    gc->drawLine( points[0][0], points[1][0], points[0][1], points[1][1] );

}

void Line::out(std::ostream& os) const {

    //print all line values formatted properly
    os << "line" << endl;
    this->Shape::out(os);
	os << "endPoint " << ~endPoint;
	os << "endline" << endl;
}

Line* Line::clone() { 
    return new Line( *this );
}


void Line::in( std::istream& inStream ) {
   
    if( inStream.fail() ) {
        throw shapeException("istream for file is NULL in Line");
    }
    
    string line;
    string word;
    vector<string> words;
    
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
            
            if( words[i] == "endPoint" ) {
                points[0][1] = atof( words[i+1].c_str() );
                points[1][1] = atof( words[i+2].c_str() );
                points[2][1] = atof( words[i+3].c_str() );
                points[3][1] = atof( words[i+4].c_str() );
                this->endPoint = matrix( 4,1,(~points)[1]);
            }
        }
    }
}  

















