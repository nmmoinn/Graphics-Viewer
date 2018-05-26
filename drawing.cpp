 
#include "drawing.h"
#include "viewcontext.h"
#include "gcontext.h"
#include <math.h> 
#include "matrix.h"
#include "line.h"
#include "image.h"
#include "triangle.h"
#include <time.h>
#include <ios>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>

using namespace std;

// Constructor
Drawing::Drawing( ) {

    //set intial values
    x0 = x1 = y0 = y1 = 0;
    
    //save new image
    images.push_back( new Image() );
    views.push_back(new ViewContext( ));
    
    //create and save file name for image
    time_t timer;
    timer = time(NULL);
    stringstream fileName;
    fileName << "Image" << images.size() << timer << ".txt";
    fileNames.push_back( fileName.rdbuf()->str() );
    
    return;
}

// Constructor
Drawing::Drawing( matrix& origin ) {

    //set intial values
    x0 = x1 = y0 = y1 = 0;
    
    //save new image
    images.push_back( new Image() );
    
    //create view with origin and orientation values (flip y)
    this->origin = matrix( origin );
    views.push_back(new ViewContext( origin, 1, -1, 1 ));
    
    //create and save file name for image
    time_t timer;
    timer = time(NULL);
    stringstream fileName;
    fileName << "Image" << images.size() << timer << ".txt";
    fileNames.push_back( fileName.rdbuf()->str() );
    
    return;
}

/********
*
*   Destructor, cycle through images and views deleting all
*
********/
Drawing::~Drawing() {
    for( unsigned int i = 0; i < images.size(); i++ ) {
        delete images[i];
        delete views[i];
    }
}

/********
*
*   Activated when window is reset or resized, just redraw image
*
********/
void Drawing::paint(GraphicsContext* gc) {

    images[imageIndex]->draw(gc,views[imageIndex]);
}

/********
*
*   Activated when mouse button pressed, handles dragging, zooming, point saving,
*       and rubberbanding
*
********/
void Drawing::mouseButtonDown(GraphicsContext* gc, unsigned int button, int x, int y) {

    //set origin to mouse location
    if( button == 1 ) {
        dragging = true;
        x0 = x;
        y0 = y;
    }

    //scroll down to zoom in
    if( button == 4 ) {
        views[imageIndex]->zoom(1.05);
        gc->clear();
        images[imageIndex]->draw(gc,views[imageIndex]);
    }
    
    //scroll up to zoom out
    if( button == 5 ) {
        views[imageIndex]->zoom(0.95);
        gc->clear();
        images[imageIndex]->draw(gc,views[imageIndex]);
    }
    
    //deal with shape if there is one
    switch( currentShape ) {
    
        case line:
            
            //complete line if one point done
            if( pointCount == 1 ) {

                //erase and change drawing mode
                gc->drawLine(x0,y0,x1,y1);
                x1 = x;
                y1 = y;
                
                //save new coordinate
                coordinates.push_back(x1);
                coordinates.push_back(y1);
                coordinates.push_back(0);
                coordinates.push_back(1);
                
                //create matrix from coordiantes, line from the matrix, and save the line
                matrix temp(2,4,(double*)&coordinates[0]);
                temp = views[imageIndex]->transformINV(temp);
                Line tempLine( GraphicsContext::GREEN, temp );
                images[imageIndex]->add( tempLine );
                
                //redraw
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex]);

                //reset stuff
                pointCount = 0;
                currentShape = none;
                coordinates.clear();
                break;
                    
            }
            
            //set x0,y0,y1,y1, and gc mode 
            x0 = x1 = x;
            y0 = y1 = y;
            
            //save origin coordinate
            coordinates.push_back(x0);
            coordinates.push_back(y0);
            coordinates.push_back(0);
            coordinates.push_back(1);
            
            pointCount++;
         
            break;
            
        case triangle: 
        
            //end triangle if 2 points done
            if( pointCount == 2 ) {
                
                //save new coordinate
                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(0);
                coordinates.push_back(1);
                
                //create matrix from coordiantes and triangle from the matrix
                matrix temp(3,4,(double*)&coordinates[0]);
                temp =  views[imageIndex]->transformINV(temp);
                Triangle tempTriangle( GraphicsContext::GREEN, temp);
                images[imageIndex]->add( tempTriangle );
        
                //draw that triangle
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex]);

                //reset stuff
                pointCount = 0;
                currentShape = none;
                coordinates.clear();
                break;
                    
            }
            
            //save recent coordinates
            x0 = x1 = x;
            y0 = y1 = y;
            coordinates.push_back(x0);
            coordinates.push_back(y0);
            coordinates.push_back(0);
            coordinates.push_back(1);
            
            pointCount++;
         
            break;
            
        case none: 
            break;
    }
  
    return;
}


/********
*
*   Handles keys when pressed down on keyboard
*
********/
void Drawing::keyDown(GraphicsContext* gc, unsigned int keycode) { 

    //escape, cancel shape and redraw current image
    if( keycode == 65307 ) {
        currentShape = none;
        pointCount = 0;
        coordinates.clear();
        
        gc->clear();
        images[imageIndex]->draw(gc,views[imageIndex]);
    }

    //make sure a shape drawing isnt in progress
    if( pointCount == 0 ) {
    
        switch( keycode ) {
        
            case 'l':
                currentShape = line;
                break;
                
            case 't':
                currentShape = triangle;
                break;
                
            case 'x':
                currentMode = xRotate;
                break;
                
            case 'y':
                currentMode = yRotate;
                break;
                
            case 'z':
                currentMode = zRotate;
                break;
                
            //new image window
            case 'n': 
                {
                    time_t timer;
                    timer = time(NULL);
                    
                    stringstream fileName;
                    fileName << "Image" << images.size() << timer << ".txt";
                    
                    fileNames.push_back( fileName.rdbuf()->str() );
                
                    imageIndex = images.size();
                    images.push_back( new Image() );
                    views.push_back(new ViewContext( origin, 1, -1, 1 ));
                    gc->clear();
                
                }
                
                break;
                
            //delete image window
            case 'r': 
                
                if( images.size() > 1 ) {
                
                    //delete file name
                    //delete fileNames[imageIndex];
                    fileNames.erase(fileNames.begin() + imageIndex);
                
                    //delete image pointer
                    delete images[imageIndex];
                    images.erase(images.begin() + imageIndex);
                    
                    //clear and redraw
                    gc->clear();
                    imageIndex = 0;
                    images[imageIndex]->draw(gc,views[imageIndex]);
                }
                
                break;
               
            //erase image
            case 'c': 
                
                images[imageIndex]->erase();
                gc->clear();
                
                break;
           
            //import image from file, file name input into terminal
            case 'i':
                {
                
                    fstream inFile;
                    string inFileName;
                    
                    //cin.ignore(numeric_limits<streamsize>::max(),'\n');
                
                    cout << "Input image file name" << endl;
                    getline( cin, inFileName );
                    
                    inFile.open( inFileName );
                    
                    if( inFile.fail() ) {
                        cout << "File '" << inFileName << "' not found." << endl;
                        break; 
                    }
                    
                    if ( inFileName.find(".stl") != string::npos ) {
                        images[imageIndex]->stl_in(inFile);
                    } else {
                        images[imageIndex]->in(inFile);
                    }            
                    
                    gc->clear();
                    images[imageIndex]->draw(gc,views[imageIndex]);
                    
                    inFile.close();
                
                }
                break;
                
            //save image to file with unique name
            case 'o': 
                {
                    ofstream fileOut;
                    fileOut.open( fileNames[imageIndex], ios::out | ios::trunc );
                    images[imageIndex]->out( fileOut );
                    cout << "Image saved to file " << fileNames[imageIndex] << endl;
                    fileOut.close();
                }
                break;

            //cycle images one left
            case ';':
                
                if( imageIndex > 0 ) {
                    imageIndex--;
                    gc->clear();
                    images[imageIndex]->draw(gc,views[imageIndex]);
                }
                
                break;
                
            //cycle images one right
            case '\'':
                
                if( imageIndex < images.size()-1 ) {
                    imageIndex++;
                    gc->clear();
                    images[imageIndex]->draw(gc,views[imageIndex]);
                }
            
                break;
                
            //left arrow, move image left
            case 65361:
                
                views[imageIndex]->translate( -2, 0, 0 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
            
            //up arrow, move image up
            case 65362:
                
                views[imageIndex]->translate( 0, 2, 0 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            //right arrow, move image right
            case 65363:
                
                views[imageIndex]->translate( 2, 0, 0 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            //down arrow, move image down
            case 65364:
                
                views[imageIndex]->translate( 0, -2, 0 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
            
            //scale image down
            case '-':
                
                views[imageIndex]->scale( 0.95 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
             
            //scale image up   
            case '=':
                
                views[imageIndex]->scale( 1.05 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            //space bar, reset view  
            case 32:
                
                views[imageIndex]->reset();
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            case 'w':
                views[imageIndex]->verticalOrbit(-0.06);
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            case 'a':
                views[imageIndex]->horizontalOrbit(-0.06);
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            case 's':
                views[imageIndex]->verticalOrbit(0.06);
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            case 'd':
                views[imageIndex]->horizontalOrbit(0.06);
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            //rotate viewport clockwise about z axis
            case 'q': 
            
                views[imageIndex]->rotateLookUp( -0.06 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
                
            //rotate viewport counterclockwise about z axis
            case 'e': 
            
                views[imageIndex]->rotateLookUp( 0.06 );
                gc->clear();
                images[imageIndex]->draw( gc,views[imageIndex] );
                break;
        }
    }
}

/********
*
*   Activated when key is released
*
********/
void Drawing::keyUp(GraphicsContext* gc, unsigned int keycode) {

    if( pointCount == 0 ) {
    
        switch( keycode ) {
        
            case 'x':
            case 'y':
            case 'z':
                currentMode = translate;
                break;
                
        }
    }
    //cout << keycode << endl;
}

/********
*
*   Activated when mouse is moved, handles dragging and rubberbanding shapes
*
********/
void Drawing::mouseMove(GraphicsContext* gc, int x, int y) {

    //make sure shape has been chosen
    if( currentShape != none ) {

        //set draw mode to XOR
        gc->setMode(GraphicsContext::MODE_XOR);

        switch( currentShape ) {
        
            case line:
                
                //rubberband between both line points
                if( pointCount == 1 ) {
                    gc->drawLine(x0,y0,x1,y1);
                    x1 = x;
                    y1 = y;
                    gc->drawLine(x0,y0,x1,y1);
                }
             
                break;
                
            case triangle: 
            
                //rubberband between first two points
                if( pointCount == 1 ) { 
                    gc->drawLine(x0,y0,x1,y1);
                    x1 = x;
                    y1 = y;
                    gc->drawLine(x0,y0,x1,y1);
                }
            
                //rubber band between last-first and last-middle
                if( pointCount > 1 ) { 
                    gc->drawLine(x0,y0,x1,y1);
                    
                    //make sure first line stays visible
                    if( x0 != x1 || y0 != y1 ) {
                        gc->drawLine(coordinates[0],coordinates[1],x1,y1);
                    }
                    x1 = x;
                    y1 = y;
                    gc->drawLine(coordinates[0],coordinates[1],x1,y1);
                    gc->drawLine(x0,y0,x1,y1);
                }
                
                break;
                
            case none: 
                break;
        } 
        
        //set draw mode back
        gc->setMode(GraphicsContext::MODE_NORMAL);
    }else {
        
        //if user is dragging, translate image accordingly and redraw
        if( dragging == true && (abs(x-x0) > 5 || abs(y-y0) > 5) ) {
        
            switch( currentMode ) {
            
                case translate:
                    views[imageIndex]->translate( x-x0, y0-y, 0 );
                    x0 = x;
                    y0 = y;
                    gc->clear();
                    images[imageIndex]->draw(gc,views[imageIndex]);   
                    break;
                
                case xRotate:
                    views[imageIndex]->rotateX( (x0-x+y-y0)/100.0 );
                    x0 = x;
                    y0 = y;
                    gc->clear();
                    images[imageIndex]->draw(gc,views[imageIndex]);   
                    break;
                
                case yRotate:
                    views[imageIndex]->rotateY( (x0-x+y-y0)/100.0  );
                    x0 = x;
                    y0 = y;
                    gc->clear();
                    images[imageIndex]->draw(gc,views[imageIndex]);   
                    break;
                
                case zRotate:
                    views[imageIndex]->rotateZ( (x0-x+y-y0)/100.0  );
                    x0 = x;
                    y0 = y;
                    gc->clear();
                    images[imageIndex]->draw(gc,views[imageIndex]);   
                    break;
            }
        }
    }
    
    return; 
}

/********
*
*   Activated when mouse button is released, handles end of dragging
*
********/
void Drawing::mouseButtonUp(GraphicsContext* gc, unsigned int button, int x, int y) {

    if( button == 1 ) {
        dragging = false;
    }

}
