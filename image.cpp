#include "line.h"
#include "image.h"
#include "triangle.h"
#include "shape.h"
#include "viewcontext.h"
#include "gcontext.h"
#include <iostream>
#include <sstream>

/***********
*   Constructor, create and save origin lines to always view
***********/
Image::Image() {

    //create origin lines for world
    double l1[] = { 100,0,0,1,-100,0,0,1 };
    double l2[] = { 0,100,0,1,0,-100,0,1 };
    double l3[] = { 0,0,100,1,0,0,-100,1 };
    matrix ml1( 2,4,l1 );
    matrix ml2( 2,4,l2 );
    matrix ml3( 2,4,l3 );
    viewOrigin[0] = Line( GraphicsContext::RED, ml1 );
    viewOrigin[1] = Line( GraphicsContext::GREEN, ml2 );
    viewOrigin[2] = Line( GraphicsContext::BLUE, ml3 );

    //create origin lines for model
    double l4[] = { 30,0,0,1,-30,0,0,1 };
    double l5[] = { 0,30,0,1,0,-30,0,1 };
    double l6[] = { 0,0,30,1,0,0,-30,1 };
    matrix ml4( 2,4,l4 );
    matrix ml5( 2,4,l5 );
    matrix ml6( 2,4,l6 );
    modelOrigin[0] = Line( GraphicsContext::RED, ml4 );
    modelOrigin[1] = Line( GraphicsContext::GREEN, ml5 );
    modelOrigin[2] = Line( GraphicsContext::BLUE, ml6 );

}

/***********
*   Copy constructor, copy all shapes
***********/
Image::Image( const Image& image ) {

    shapes.clear();
    
    for( unsigned int i = 0; i < image.shapes.size(); i++ ) {
        shapes.push_back( image.shapes[i]->clone() );
    }    
    
}

/***********
*   Delete all shape pointers
************/
Image::~Image() {
    
    for( unsigned int i = 0; i < shapes.size(); i++ ) {
        delete shapes[i];
    } 
}

/***********
*   Draw origin lines and the shapes
***********/
void Image::draw( GraphicsContext* gc, ViewContext* view ) {
    	
    	//draw origin lines first thing so they're always seen
    viewOrigin[0].drawNM( gc,view );
    viewOrigin[1].drawNM( gc,view );
    viewOrigin[2].drawNM( gc,view );
    modelOrigin[0].draw( gc,view );
    modelOrigin[1].draw( gc,view );
    modelOrigin[2].draw( gc,view );
    	        
    //draw every shape
    for( unsigned int i = 0; i < shapes.size(); i++ ) {
        shapes[i]->draw( gc, view );
    }
}

/***********
*   Add a shape
***********/
void Image::add( Shape& shape ) {
    shapes.push_back( shape.clone() );
}


/***********
*   Copy all the shapes 
***********/
Image& Image::operator=(const Image& image) {

    //return if nothing to copy
	if( &image == this ) {    
	    return *this;
	}

    shapes.clear();
    
    for( unsigned int i = 0; i < image.shapes.size(); i++ ) {
        shapes.push_back( image.shapes[i]->clone() );
    } 
	
	return *this;
}

/***********
*   Output all shape information to output stream
***********/
void Image::out(std::ostream& os) const {

    for( unsigned int i = 0; i < shapes.size(); i++ ) {
        shapes[i]->out(os);
    } 
}

/***********
*   Get a bunch of shapes from input stream
***********/
void Image::in(std::istream& inStream) {

    if( inStream.fail() ) {
        throw shapeException("istream for file is NULL");
    }
    
    string line;
    string word;
    vector<string> words;
    stringstream shapeStream;
    
    //for the whole file
    while(!inStream.eof()) {
    
        //get the line
        getline(inStream,line);
        
        //split it usefully
        words = split( line, ' ' );
        words = removeGarbage(words);
   
        //for each word in the line
        for ( uint32_t i = 0; i < words.size(); i++ ) {
       
            //get line info if first word is line
            if( words[i] == "line" ) {
            
              getline(inStream,line);
                words = split( line, ' ' );
                words = removeGarbage(words);
                
                while( words[0] != "endline" ) {
                    shapeStream << line << endl;
                    getline(inStream,line);
                    words = split( line, ' ' );
                    words = removeGarbage(words);
                }
                Line* newLine = new Line();
                newLine->in(shapeStream);
                shapes.push_back(newLine);
                shapeStream.clear();
            }
            
            //get triangle info if first word is triangle
            if( words[i] == "triangle" ) {
            
                getline(inStream,line);
                words = split( line, ' ' );
                words = removeGarbage(words);
                
                while( words[0] != "endtriangle" ) {
                    shapeStream << line << endl;
                    getline(inStream,line);
                    words = split( line, ' ' );
                    words = removeGarbage(words);
                }
                Triangle* newTriangle = new Triangle();
                newTriangle->in(shapeStream);
                shapes.push_back(newTriangle);
                shapeStream.clear();
            }
        }
    }
}


/************
*   Get input from a stream in the form of an stl file
************/
void Image::stl_in(std::istream& inStream) {

    if( inStream.fail() ) {
        throw shapeException("istream for file is NULL");
    }
    
    string line;
    string word;
    vector<string> words;
    double points[12];
    int pointCount = 0;
    Triangle tri;
    matrix mpoints(3,4);
    
    //for the whole file
    while(!inStream.eof()) {
    
        getline(inStream,line);
        
        if( line.substr( line.length()-1, line.length() ) == "\r" ) {
            line = line.substr( 0, line.length()-1 );
        }
        words = split( line, ' ' );
        words = removeGarbage(words);
   
        for ( uint32_t i = 0; i < words.size(); i++ ) {
       
            if( words[i] == "solid" ) {
                //save name
                name = words[++i];        
                continue;
            }
            
            if( words[i] == "facet" ) {
                //reset point count
                pointCount = 0;
            }
            
            if( words[i] == "vertex" ) {
                //save following coordinates x y z 1
                points[pointCount++] = atof( words[++i].c_str() );
                points[pointCount++] = atof( words[++i].c_str() );    
                points[pointCount++] = atof( words[++i].c_str() );
                points[pointCount++] = 1;
                continue;
            }
            
            if( words[i] == "endloop" ) {
                //make triangle from points
                mpoints = matrix(3,4,points);
                tri = Triangle( GraphicsContext::WHITE, mpoints );
            }
            
            if( words[i] == "endfacet" ) {
                //save triangle
                add( tri );
            }
            
            if( words[i] == "endsolid" ) {
                //end import
                return;
            }
        }
    }
}

string Image::getName() {
    return name;
}

void Image::erase() {

    for( unsigned int i = 0; i < shapes.size(); i++ ) {
        delete shapes[i];
    } 
    
    shapes.clear();

}
