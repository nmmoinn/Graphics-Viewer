
#include "viewcontext.h"
#include "matrix.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;


//********************  Constructors and Destructors **********


//default constuctor	
ViewContext::ViewContext( ) {}

/********
*   Constructor with initial settings for view, allows flipping of 
*       axis and origin setting
*********/
ViewContext::ViewContext( matrix& origin, int xOrient, int yOrient, int zOrient) {

    //set original axes and perspective
    reset();
    
    //set origin 
    morigin[0][0] = xOrient;
    morigin[1][1] = yOrient;
    morigin[2][2] = zOrient;
    morigin[0][3] = origin[0][0];
    morigin[1][3] = origin[0][1];
    morigin[2][3] = origin[0][2];

    //calculate final transformation matrix
    mfinal = morigin * mproject * mtranslate * mscale * mrotate;
}

/*********
*   Copy constructor, copy all data 
*********/
ViewContext::ViewContext( const ViewContext& view ) {
    
    for( int i = 0; i < 3; i++ ) {
        xAxis[i] = view.xAxis[i];
        yAxis[i] = view.yAxis[i];
        zAxis[i] = view.zAxis[i];
    }
    
    mperspective = matrix( view.mperspective );
    mviewCenter = matrix( view.mviewCenter );
    lookUp = matrix( view.lookUp );
    mproject = matrix( view.mproject );
    mrotate = matrix( view.mrotate );
    mscale = matrix( view.mscale );
    mtranslate = matrix( view.mtranslate );
    modelFinal = matrix( view.modelFinal );
    mfinal = matrix( view.mfinal );
    morigin = matrix( view.morigin );
    
}

/*********
*   Matrcies are all references, nothing to do here
*********/
ViewContext::~ViewContext() {}


/****************
*   Set initial view by changing camera location and view plane 
****************/
void ViewContext::reset() {

    //set view plane center
    mviewCenter[0][0] = -5;
    mviewCenter[1][0] = 0;
    mviewCenter[2][0] = 15;
    mviewCenter[3][0] = 1;

    //set vector defining look up direction
    lookUp[0][0] = -1;
    lookUp[1][0] = 0;
    lookUp[2][0] = 0;
    lookUp[3][0] = 1;

    //set initial perspective distance (camera distance)
    mperspective[0][0] = 1;
    mperspective[1][1] = 1;
    mperspective[2][2] = 2;
    mperspective[3][2] = -1/200.0;
    mperspective[3][3] = 1;

    //update axes and perspective
    updateAxes();
    updatePerspective();
    
    //reset model matricies
    mrotate = matrix::identity(4);
    mscale = matrix::identity(4);
    mtranslate = matrix::identity(4);
    
    //save model transform and final transformation matrix
    modelFinal = mtranslate * mscale * mrotate;
    mfinal = morigin * mproject * modelFinal;
}



//*********************  Model Transforming functions ****************


/*********
*   Scales the model view up or down
*********/
void ViewContext::scale( double scale ) {

    //change scale by multiplying each scale value 
    mscale[0][0] *= scale;
    mscale[1][1] *= scale;
    mscale[2][2] *= scale;
    
    //save model transform and final transformation matrix
    modelFinal = mtranslate * mscale * mrotate;
    mfinal = morigin * mproject * modelFinal;
}   

/*********
*   Rotate model around its x axis, this is done by rotating around a unit vector
*       where the unit vector is defined by the rotation matrix since it saves 
*       unit vectors of where the original axes are mapped to
*********/
void ViewContext::rotateX( double angle ) {
 
    //define rotation matrix values
    double rotation[] = {  
            cos(angle)+mrotate[0][0]*mrotate[0][0]*(1-cos(angle)),
            mrotate[0][0]*mrotate[1][0]*(1-cos(angle))-mrotate[2][0]*sin(angle),
            mrotate[0][0]*mrotate[2][0]*(1-cos(angle))+mrotate[1][0]*sin(angle), 0, 
            mrotate[1][0]*mrotate[0][0]*(1-cos(angle))+mrotate[2][0]*sin(angle),
            cos(angle)+mrotate[1][0]*mrotate[1][0]*(1-cos(angle)),
            mrotate[1][0]*mrotate[2][0]*(1-cos(angle))-mrotate[0][0]*sin(angle), 0,
            mrotate[2][0]*mrotate[0][0]*(1-cos(angle))-mrotate[1][0]*sin(angle),
            mrotate[2][0]*mrotate[1][0]*(1-cos(angle))+mrotate[0][0]*sin(angle),
            cos(angle)+mrotate[2][0]*mrotate[2][0]*(1-cos(angle)), 0,
            0, 0, 0, 1 };
                        
    matrix rotate( 4,4, &rotation[0] );
    
    //calculated changed matricies
    mrotate = rotate * mrotate;
    modelFinal = mtranslate * mscale * mrotate;
    mfinal = morigin * mproject * modelFinal;
}

/*********
*   Rotate model around its y axis, this is done by rotating around a unit vector
*       where the unit vector is defined by the rotation matrix since it saves 
*       unit vectors of where the original axes are mapped to
*********/
void ViewContext::rotateY( double angle ) {
    
    //define rotation matrix values
    double rotation[] = {  
            cos(angle)+mrotate[0][1]*mrotate[0][1]*(1-cos(angle)),
            mrotate[0][1]*mrotate[1][1]*(1-cos(angle))-mrotate[2][1]*sin(angle),
            mrotate[0][1]*mrotate[2][1]*(1-cos(angle))+mrotate[1][1]*sin(angle), 0, 
            mrotate[1][1]*mrotate[0][1]*(1-cos(angle))+mrotate[2][1]*sin(angle),
            cos(angle)+mrotate[1][1]*mrotate[1][1]*(1-cos(angle)),
            mrotate[1][1]*mrotate[2][1]*(1-cos(angle))-mrotate[0][1]*sin(angle), 0,
            mrotate[2][1]*mrotate[0][1]*(1-cos(angle))-mrotate[1][1]*sin(angle),
            mrotate[2][1]*mrotate[1][1]*(1-cos(angle))+mrotate[0][1]*sin(angle),
            cos(angle)+mrotate[2][1]*mrotate[2][1]*(1-cos(angle)), 0,
            0, 0, 0, 1 };
                        
    matrix rotate( 4,4, &rotation[0] );
    
    //calculated changed matricies
    mrotate = rotate * mrotate;
    modelFinal = mtranslate * mscale * mrotate;
    mfinal = morigin * mproject * modelFinal;
}

/*********
*   Rotate model around its z axis, this is done by rotating around a unit vector
*       where the unit vector is defined by the rotation matrix since it saves 
*       unit vectors of where the original axes are mapped to
*********/
void ViewContext::rotateZ( double angle ) {

    //define rotation matrix values
    double rotation[] = {  
            cos(angle)+mrotate[0][2]*mrotate[0][2]*(1-cos(angle)),
            mrotate[0][2]*mrotate[1][2]*(1-cos(angle))-mrotate[2][2]*sin(angle),
            mrotate[0][2]*mrotate[2][2]*(1-cos(angle))+mrotate[1][2]*sin(angle), 0,
            mrotate[1][2]*mrotate[0][2]*(1-cos(angle))+mrotate[2][2]*sin(angle),
            cos(angle)+mrotate[1][2]*mrotate[1][2]*(1-cos(angle)),
            mrotate[1][2]*mrotate[2][2]*(1-cos(angle))-mrotate[0][2]*sin(angle), 0,
            mrotate[2][2]*mrotate[0][2]*(1-cos(angle))-mrotate[1][2]*sin(angle),
            mrotate[2][2]*mrotate[1][2]*(1-cos(angle))+mrotate[0][2]*sin(angle),
            cos(angle)+mrotate[2][2]*mrotate[2][2]*(1-cos(angle)), 0,
            0, 0, 0, 1 };

    matrix rotate( 4,4, &rotation[0] );
    
    //calculated changed matricies
    mrotate = rotate * mrotate;
    modelFinal = mtranslate * mscale * mrotate;
    mfinal = morigin * mproject * modelFinal;
}

/*********
*   Translate shape offset by view axes so it looks like it moves based on
*       view plane coordinates
*********/
void ViewContext::translate( double dx, double dy, double dz ) {
    
    mtranslate[0][3] += dx * xAxis[0] + dy * yAxis[0] + dz * zAxis[0];
    mtranslate[1][3] += dx * xAxis[1] + dy * yAxis[1] + dz * zAxis[1];
    mtranslate[2][3] += dx * xAxis[2] + dy * yAxis[2] + dz * zAxis[2];
    
    //calculated changed matricies
    modelFinal = mtranslate * mscale * mrotate;
    mfinal = morigin * mproject * modelFinal;
}



//************************  Final transforming functions *****************


/*********
*   Transform coordinates based on final transform matrix
*********/
matrix ViewContext::transform( matrix& points ) {

    matrix tranny( mfinal * points );
   
    return homogenize( tranny );

}

/**************
*   Transform without model changes, useful for viewing original axes
***************/
matrix ViewContext::transformNM( matrix& points ) {

    matrix tranny( morigin * mproject * points );
   
    return homogenize( tranny );

}

/*********
*   Transform coordinates based on inverse of final transform matrix
*       calculate inverse here to minimize matrix multiplications while
*       transforming view, this is done so motion is smoother
*********/
matrix ViewContext::transformINV( matrix& points ) {

    matrix tranny( squareInverse( mfinal ) * ~points );
   
    return ~homogenize( tranny );
}

/**
*   Convert homogenous cordiantes to Euclidean by dividing by last value,
*       only do for x and y since there is no depth functionality yet
*/
matrix ViewContext::homogenize( matrix& points ) {

    //divide x and y by last value, z should be 0 at this point
    for( int i = 0; i < points.getCols(); i++ ) {
        points[0][i] /= points[3][i];
        points[1][i] /= points[3][i];
    }
    
    return points;
}



//*************************  Camera view changing functions ************


/*********
*   Zoom by changing the persective matrix value Zf
*********/
void ViewContext::zoom( double zoomValue ) {

    mperspective[3][2] *= zoomValue;
    
    updatePerspective();
    
    mfinal = morigin * mproject * modelFinal;
}  

/*********
*   Rotate lookup vector around current viewing y axis, this results in looking 
*       as if one is tilting their head
*********/
void ViewContext::rotateLookUp( double angle ) {

    double rotation[] = {  
            cos(angle)+zAxis[0]*zAxis[0]*(1-cos(angle)),
            zAxis[0]*zAxis[1]*(1-cos(angle))-zAxis[2]*sin(angle),
            zAxis[0]*zAxis[2]*(1-cos(angle))+zAxis[1]*sin(angle), 0,
            zAxis[1]*zAxis[0]*(1-cos(angle))+zAxis[2]*sin(angle),
            cos(angle)+zAxis[1]*zAxis[1]*(1-cos(angle)),
            zAxis[1]*zAxis[2]*(1-cos(angle))-zAxis[0]*sin(angle), 0,
            zAxis[2]*zAxis[0]*(1-cos(angle))-zAxis[1]*sin(angle),
            zAxis[2]*zAxis[1]*(1-cos(angle))+zAxis[0]*sin(angle),
            cos(angle)+zAxis[2]*zAxis[2]*(1-cos(angle)), 0,
            0, 0, 0, 1 };

    matrix rotate( 4,4, &rotation[0] );
    
    lookUp = rotate * lookUp;
    
    updateAxes();
    updatePerspective();
    
    mfinal = morigin * mproject * modelFinal;
} 

/*********
*   Rotate view plane center and lookup vector around current view x axis
*********/
void ViewContext::verticalOrbit( double angle ) {

    double rotation[] = {  
            cos(angle)+xAxis[0]*xAxis[0]*(1-cos(angle)),
            xAxis[0]*xAxis[1]*(1-cos(angle))-xAxis[2]*sin(angle),
            xAxis[0]*xAxis[2]*(1-cos(angle))+xAxis[1]*sin(angle), 0, 
            xAxis[1]*xAxis[0]*(1-cos(angle))+xAxis[2]*sin(angle),
            cos(angle)+xAxis[1]*xAxis[1]*(1-cos(angle)),
            xAxis[1]*xAxis[2]*(1-cos(angle))-xAxis[0]*sin(angle), 0,
            xAxis[2]*xAxis[0]*(1-cos(angle))-xAxis[1]*sin(angle),
            xAxis[2]*xAxis[1]*(1-cos(angle))+xAxis[0]*sin(angle),
            cos(angle)+xAxis[2]*xAxis[2]*(1-cos(angle)), 0,
            0, 0, 0, 1 };
                        
    matrix rotate( 4,4, &rotation[0] );
    
    mviewCenter = rotate * mviewCenter;
    lookUp = rotate * lookUp;
    
    updateAxes();
    updatePerspective();
    
    mfinal = morigin * mproject * modelFinal;
}

/*********
*   Rotate view plane center and lookup vector around current view y axis
*********/
void ViewContext::horizontalOrbit( double angle ) {
                                    
    double rotation[] = {  
            cos(angle)+yAxis[0]*yAxis[0]*(1-cos(angle)),
            yAxis[0]*yAxis[1]*(1-cos(angle))-yAxis[2]*sin(angle),
            yAxis[0]*yAxis[2]*(1-cos(angle))+yAxis[1]*sin(angle), 0, 
            yAxis[1]*yAxis[0]*(1-cos(angle))+yAxis[2]*sin(angle),
            cos(angle)+yAxis[1]*yAxis[1]*(1-cos(angle)),
            yAxis[1]*yAxis[2]*(1-cos(angle))-yAxis[0]*sin(angle), 0,
            yAxis[2]*yAxis[0]*(1-cos(angle))-yAxis[1]*sin(angle),
            yAxis[2]*yAxis[1]*(1-cos(angle))+yAxis[0]*sin(angle),
            cos(angle)+yAxis[2]*yAxis[2]*(1-cos(angle)), 0,
            0, 0, 0, 1 };
                        
    matrix rotate( 4,4, &rotation[0] );
    
    mviewCenter = rotate * mviewCenter;
    lookUp = rotate * lookUp;
    
    updateAxes();
    updatePerspective();
    
    mfinal = morigin * mproject * modelFinal;
}





//****************   Perspective/Projection functions ********************



/*********************
*   Recalculate viewport axes based on reference point, view origin point
*       and lookup vector
**********************/
void ViewContext::updateAxes() {

    //calculate zAxis unit vector
    double N[] = { mviewCenter[0][0], mviewCenter[1][0], mviewCenter[2][0] };
    
    double Nmag = sqrt( N[0] * N[0] + 
                        N[1] * N[1] +
                        N[2] * N[2] );
    
    zAxis[0] = mviewCenter[0][0]/Nmag;
    zAxis[1] = mviewCenter[1][0]/Nmag;
    zAxis[2] = mviewCenter[2][0]/Nmag;
    
    
    //calculate xAxis unit vector
    double L[] = {  lookUp[1][0] * N[2] - lookUp[2][0] * N[1],
                    lookUp[2][0] * N[0] - lookUp[0][0] * N[2],
                    lookUp[0][0] * N[1] - lookUp[1][0] * N[0]  };
    
    double Lmag = sqrt( L[0] * L[0] + 
                        L[1] * L[1] +
                        L[2] * L[2] );
               
    xAxis[0] = L[0]/Lmag;
    xAxis[1] = L[1]/Lmag;
    xAxis[2] = L[2]/Lmag;
    
    
    //calculate yAxis unit vector
    double M[] = {  N[1] * L[2] - N[2] * L[1],
                    N[2] * L[0] - N[0] * L[2],
                    N[0] * L[1] - N[1] * L[0]  };
                    
    double Mmag = sqrt( M[0] * M[0] + 
                        M[1] * M[1] +
                        M[2] * M[2] );
          
    yAxis[0] = M[0]/Mmag;
    yAxis[1] = M[1]/Mmag;
    yAxis[2] = M[2]/Mmag;
    
}

/*********************
*   Calculate projection matrix based on current axes
*********************/
void ViewContext::updatePerspective() {

    
    mproject[0][0] = xAxis[0];
    mproject[0][1] = xAxis[1];
    mproject[0][2] = xAxis[2];
    mproject[0][3] = -( xAxis[0] * mviewCenter[0][0] + 
                        xAxis[1] * mviewCenter[1][0] +
                        xAxis[2] * mviewCenter[2][0] );
    
    mproject[1][0] = yAxis[0];
    mproject[1][1] = yAxis[1];
    mproject[1][2] = yAxis[2];
    mproject[1][3] = -( yAxis[0] * mviewCenter[0][0] + 
                        yAxis[1] * mviewCenter[1][0] +
                        yAxis[2] * mviewCenter[2][0] );
    
    mproject[2][0] = zAxis[0];
    mproject[2][1] = zAxis[1];
    mproject[2][2] = zAxis[2];
    mproject[2][3] = -( zAxis[0] * mviewCenter[0][0] + 
                        zAxis[1] * mviewCenter[1][0] +
                        zAxis[2] * mviewCenter[2][0] );

    mproject[3][0] = 0;
    mproject[3][1] = 0;
    mproject[3][2] = 0;
    mproject[3][3] = 1;

    mproject = mperspective * mproject;
}











/**      unfinished clipping code, im a failure :(

int ViewContext::clippingCode( double x, double y, double z ) {

    unsigned int code = 000000;

    if( y > z ) {
        code |= 1;
    }
    
    if( y < -z ) {
        code |= 2;
    }
    
    if( x > z ) {
        code |= 4;
    }
    
    if( x < -z ) {
        code |= 8;
    }
    
    if( z < -zf ) {
        //code |= 16;
    }
    
    if( z > zf ) {
        //code |= 32;
    }

    return code;
}

matrix ViewContext::clip( matrix& points ) {
    
    vector<double> newPoints;
    int p1;
    int p2;
    int code1;
    int code2;
    
    double x1;
    double y1;
    double z1;
    
    double x2;
    double y2;
    double z2;
    
    double newx;
    double newy;
    double newz;
    
    double t;
    double zf = -1/mperspective[3][2];
    
    for( int i = 0; i < points.getCols(); i++ ){
    
        p1 = i;
        p2 = (i+1)%points.getCols();
        
        x1 =  points[0][p1];
        y1 =  points[1][p1];
        z1 = -points[2][p1] + zf + mviewCenter[0][2];
        
        x2 =  points[0][p2];
        y2 =  points[1][p2];
        z2 = -points[2][p2] + zf + mviewCenter[0][2];
        
        code1 = clippingCode( x1,y1,z1 );
        code2 = clippingCode( x2,y2,z2 );
    
        //if point and next are visible
        if( !( code1 | code2 ) ) {
            //cout << "not clipping" <<endl;
            //save point
            newPoints.push_back( x1 );
            newPoints.push_back( y1 );
            newPoints.push_back( z1 );
            newPoints.push_back( 1 );
        
            //save next point
            newPoints.push_back( x2 );
            newPoints.push_back( y2 );
            newPoints.push_back( z2 );
            newPoints.push_back( 1 );
            
        //if not visible at all
        } else if( false ) {
            //skip
            
        //else partially visible 
        } else {

            //if p1 needs clipping, clip
            if( code1 != 0 ) {
                
                if( code1 & 1 ) {            //above
                
                    //cout << "clipping above p1" << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                
                    t = - ( y1 - z1 ) 
                        / ( y2 - y1 - z2 + z1 );
                        
                    //cout << "t = " << t << endl;
                    
                    newx = x1 + (x2-x1) * t;
                    newy = y1 + (y2-y1) * t;
                    newz = -(z1 + (z2-z1) * t) + zf + mviewCenter[0][2];
                    
                    //cout << newx << " , " << newy << " , " << newz << endl;
                      
                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                    
                
                } else if( code1 & 2 ) {     //below
                
                    //cout << "clipping below p1" << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                
                    t = ( y1 + z1 ) 
                        / ( y2 - y1 - z2 + z1 );
                        
                    //cout << "t = " << t << endl;
                      
                    newx = x1 - (x2-x1) * t;
                    newy = y1 - (y2-y1) * t;
                    newz = -(z1 - (z2-z1) * t) + zf + mviewCenter[0][2];
                    
                    //cout << newx << " , " << newy << " , " << newz << endl;
                      
                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                
                } else if( code1 & 4 ) {     //right
                    
                    //cout << "clipping right p1" << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                
                    t = - ( x1 - z1 ) 
                        / ( x2 - x1 - z2 + z1 );
                        
                    //cout << "t = " << t << endl;
                      
                    newx = x1 + (x2-x1) * t;
                    newy = y1 + (y2-y1) * t;
                    newz = -(z1 + (z2-z1) * t) + zf + mviewCenter[0][2];
                    
                    //cout << newx << " , " << newy << " , " << newz << endl;
                      
                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                
                } else if( code1 & 8 ) {     //left
                
                    //cout << "clipping left p1" << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                
                    t = ( x1 + z1 ) 
                        / ( x2 - x1 - z2 + z1 );
                        
                    //cout << "t = " << t << endl;
                      
                    newx = x1 - (x2-x1) * t;
                    newy = y1 - (y2-y1) * t;
                    newz = -(z1 - (z2-z1) * t) + zf + mviewCenter[0][2];
                      
                    //cout << newx << " , " << newy << " , " << newz << endl;
                    
                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                
                } else if( code1 & 16 ) {    //behind
                
                } else {                        //in front
                
                }
            }
            
            
            //if p2 needs clipping, clip
            if( code2 != 0 ) {
                
                if( code2 & 1 ) {            //above
                
                    //cout << "clipping above p2" << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                
                    t = - ( y2 - z2 ) 
                        / ( y1 - y2 - z1 + z2 );
                    
                    //cout << "t = " << t << endl;
                      
                    newx = x2 + (x1-x2) * t;
                    newy = y2 + (y1-y2) * t;
                    newz = -(z2 + (z1-z2) * t) + zf + mviewCenter[0][2];
                    
                    //cout << newx << " , " << newy << " , " << newz << endl;
                      
                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                    
                
                } else if( code2 & 2 ) {     //below
                
                    //cout << "clipping below p2" << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                
                    t = ( y2 + z2 ) 
                        / ( y1 - y2 - z1 + z2 );
                      
                    //cout << "t = " << t << endl;
                      
                    newx = x2 - (x1-x2) * t;
                    newy = y2 - (y1-y2) * t;
                    newz = -(z2 - (z1-z2) * t) + zf + mviewCenter[0][2];
                    
                    //cout << newx << " , " << newy << " , " << newz << endl;
                      
                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                
                } else if( code2 & 4 ) {     //right
                
                
                    //cout << "clipping right p2" << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                
                    t = - ( x2 - z2 ) 
                        / ( x1 - x2 - z1 + z2 );
                    
                    
                    //cout << "t = " << t << endl;
                      
                    newx = x2 + (x1-x2) * t;
                    newy = y2 + (y1-y2) * t;
                    newz = -(z2 + (z1-z2) * t) + zf + mviewCenter[0][2];
                    
                    
                    //cout << newx << " , " << newy << " , " << newz << endl;

                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                
                } else if( code2 & 8 ) {     //left
                
                    //cout << "clipping left p2" << endl;
                    //cout << x2 << " , " << y2 << " , " << z2 << endl;
                    //cout << x1 << " , " << y1 << " , " << z1 << endl;
                
                    t = ( x2 + z2 ) 
                        / ( x1 - x2 - z1 + z2 );
                        
                    //cout << "t = " << t << endl;
                      
                    newx = x2 - (x1-x2) * t;
                    newy = y2 - (y1-y2) * t;
                    newz = -(z2 - (z1-z2) * t) + zf + mviewCenter[0][2];
                    
                    //cout << newx << " , " << newy << " , " << newz << endl;
                      
                    //save point
                    newPoints.push_back( newx );
                    newPoints.push_back( newy );
                    newPoints.push_back( newz );
                    newPoints.push_back( 1 );
                
                } else if( code2 & 16 ) {    //behind
                
                } else {                        //in front
                
                }
            }
        }
    }
    
    //create and return matrix from new point set
    matrix newmat( 4, newPoints.size()/4 );
    
    for( int i = 0; i < 2*points.getCols(); i++ ) {
        newmat[0][i] = newPoints[4*i+0];
        newmat[1][i] = newPoints[4*i+1];
        newmat[2][i] = newPoints[4*i+2];
        newmat[3][i] = newPoints[4*i+3];
    }
    
    return newmat;
    
    
}

matrix ViewContext::test( matrix& points ) {

    matrix tranny( points );
    
    //cout << "points" << endl;
    //cout << points << endl;
    
    tranny = mproject * tranny;
    
    //cout << "project" << endl;
    //cout << tranny << endl;
    
    tranny = clip( tranny );
    
    //cout << "clip" << endl;
    //cout << tranny << endl;

    tranny = morigin * mperspective * tranny;
    return homogenize( tranny );

}*/









