/* This is an abstract base class representing a generic graphics
 * context.  Most implementation specifics will need to be provided by
 * a concrete implementation.  See header file for specifics. */

#define _USE_MATH_DEFINES	// for M_PI
#include <cmath>	// for trig functions
#include "gcontext.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include "matrix.h"

using namespace std;

/*
 * Destructor - does nothing
 */
GraphicsContext::~GraphicsContext()
{
	// nothing to do
	// here to insure subclasses handle destruction properly
}

/*void GraphicsContext::drawShape( matrix& mpoints ) {	

    
    vector<int> straights[600];
    
    int xend = *points++;
    int yend = *points++;
    int x0 = xend;
    int y0 = yend;
    int x1 = *points++;
    int y1 = *points++;
    
    int lydir = 0;
    int fydir = (y1>=y0)-(y1<y0);
    
    int ymin = 599;
    int ymax = 0;
    int ycmin;
    int ycmax;
    
    do {
    
        ycmin = min(y0,y1);
        ycmax = max(y0,y1);
        
        if( ycmin < ymin ) {
            ymin = ycmin;
        }
        
        if( ycmax > ymax ) {
            ymax = ycmax;
        }
    
        //std::cout << x0 << "," << y0 << " to " << x1 << "," << y1 << std::endl;
            
        const int xdir = (x1>=x0)-(x1<x0);
	    const int ydir = (y1>=y0)-(y1<y0);

	    int dx = (x1-x0)*xdir;
	    int dy = (y1-y0)*ydir;

	    int error;
	    
	    if( lydir != ydir && lydir != 0 ) {
	        straights[y0].pop_back();
	    }
	    
	    lydir = ydir; 
	            

	    if (dx >= dy) {

		    dy *= 2;
		    error = dy - dx;
		    dx *= 2;

		    while (x0 != x1) {

			    setPixel(x0, y0);

			    if (error >= 0) {
				    y0 += ydir;
				    straights[y0].push_back(x0+xdir);
				    error -= dx;
			    }
			    
			    error += dy;
			    x0 += xdir;
		    } 
		    
		    setPixel(x0, y0);

	    } else { 	
	    
		    dx *= 2;
		    error = dx - dy;
		    dy *= 2;

		    while (y0 != y1) {

			    setPixel(x0, y0);
			    
			    if (error >= 0) {
				    x0 += xdir;
				    error -= dy;
			    }

			    error += dx;
			    y0 += ydir;
			    straights[y0].push_back(x0);
		    } 
		    
		    setPixel(x0, y0);	
	    }	
	    
	    x0 = x1;
	    y0 = y1;
	    x1 = *points++;
        y1 = *points++;
	    
    } while( !(x0==xend && y0==yend) );
    
    if( lydir != fydir ) {
        straights[y0].pop_back();
    }
    
    for( int i = ymin; i <= ymax; i++ ) {
    
        //cout << i << " : ";
        
        for( int j = 0; j < ((int)straights[i].size()-1); j += 2 ) {
        
            sort(straights[i].begin(), straights[i].end());
            
            //cout << straights[i][j] << "," << straights[i][j+1] << ",";
            
            for( int k = straights[i][j]; k <= straights[i][j+1]; k++ ) {
                setPixel(k,i);
            }
        }
        
        //cout << endl;
    }
}*/

/* This is a naive implementation that uses floating-point math
 * and "setPixel" which will need to be provided by the concrete
 * implementation.
 * 
 * Parameters:
 * 	x0, y0 - origin of line
 *  x1, y1 - end of line
 * 
 * Returns: void
 */
void GraphicsContext::drawLine(int x0, int y0, int x1, int y1) {	
	
	const int xdir = (x1>=x0)-(x1<x0);
	const int ydir = (y1>=y0)-(y1<y0);

	int dx = (x1-x0)*xdir;
	int dy = (y1-y0)*ydir;

	int error;
	
	int x = x0;
	int y = y0;

	//if dx is the larger distance
	if (dx >= dy) {

		//calculate error
		dy *= 2;
		error = dy - dx;
		dx *= 2;

		//draw line 
		while (x != x1) {

			setPixel(x, y);

			//if error is positive
			if (error >= 0) {
				//increment y
				y += ydir;
				//decrease error
				error -= dx;
			}
			
			//add error and increment x
			error += dy;
			x += xdir;
		} 
		
		setPixel(x, y);

	}
       //if dy is the larger distance	
	else { 	
		//calulate error	
		dx *= 2;
		error = dx - dy;
		dy *= 2;

		//draw line
		while (y != y1) {

			setPixel(x, y);
			
			//if error is positive
			if (error >= 0) {
				//increment x
				x += xdir;
				//decrease error
				error -= dy;
			}

			//add error and increment y
			error += dx;
			y += ydir;
		} 
		
		setPixel(x, y);	
	}	
}

/* This is a naive implementation that uses floating-point math
 * and "setPixel" which will need to be provided by the concrete
 * implementation.
 * 
 * Parameters:
 * 	x0, y0 - origin/center of circle
 *  radius - radius of circle
 * 
 * Returns: void
 */
void GraphicsContext::drawCircle(int x0, int y0, unsigned int radius)
{
    int x = radius;
    int y = 0;
    int error = radius;

    while (x >= y) {
	
	//set pixels
    	setPixel(x0 + x, y0 + y);
    	setPixel(x0 + y, y0 + x);
    	setPixel(x0 - y, y0 + x);
   	setPixel(x0 - x, y0 + y);
   	setPixel(x0 - x, y0 - y);
   	setPixel(x0 - y, y0 - x);
   	setPixel(x0 + y, y0 - x);
   	setPixel(x0 + x, y0 - y);
	
	//subtract error metric and increment y
	error -= 2*(++y)+1;

	//if error is one radius worth away decrement x and add error
	if( error < -(int)radius ) {
		error += 2*(x--)-1;
	}
    }
}

void GraphicsContext::endLoop()
{
	run = false;
}

void GraphicsContext::drawElipse(int xc,int yc,int rx, int ry) {

    int x;
    int y;
    float p;

    //Region 1
    p=ry*ry-rx*rx*ry+rx*rx/4;
    x=0;y=ry;
    while(2.0*ry*ry*x <= 2.0*rx*rx*y) {
    
        if(p < 0) {
	        x++;
	        p = p+2*ry*ry*x+ry*ry;
        } else {
	        x++;y--;
	        p = p+2*ry*ry*x-2*rx*rx*y-ry*ry;
        }
        
            setPixel(xc+x,yc+y);
            setPixel(xc+x,yc-y);
            setPixel(xc-x,yc+y);
            setPixel(xc-x,yc-y);
    }

    //Region 2
    p=ry*ry*(x+0.5)*(x+0.5)+rx*rx*(y-1)*(y-1)-rx*rx*ry*ry;
        
    while(y > 0) {
    
        if(p <= 0) {
            x++;y--;
            p = p+2*ry*ry*x-2*rx*rx*y+rx*rx;
        }
        else
        {
            y--;
            p = p-2*rx*rx*y+rx*rx;
        }
        
        setPixel(xc+x,yc+y);
        setPixel(xc+x,yc-y);
        setPixel(xc-x,yc+y);
        setPixel(xc-x,yc-y);
    }

}


