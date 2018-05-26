#ifndef DRAWING_H
#define DRAWING_H

#include "drawbase.h"
#include "viewcontext.h"
#include <vector>
#include "matrix.h"
#include "image.h"
#include <fstream>


// forward reference
class GraphicsContext;

class Drawing : public DrawingBase {

    public:

    Drawing( );
    
    Drawing( matrix& origin );
    
    ~Drawing();
    
    virtual void paint(GraphicsContext* gc);
    virtual void keyDown(GraphicsContext* gc, unsigned int keycode);
	virtual void keyUp(GraphicsContext* gc, unsigned int keycode);
    virtual void mouseButtonDown(GraphicsContext* gc, unsigned int button, int x, int y);
    virtual void mouseButtonUp(GraphicsContext* gc, unsigned int button,int x, int y);
    virtual void mouseMove(GraphicsContext* gc, int x, int y);
    
    private:
    
    enum PossibleShapes { none, line, triangle };
    enum Mode { translate, xRotate, yRotate, zRotate };

    int x0;
    int y0;
    int x1;
    int y1;
    
    bool dragging = false;
    
    unsigned int imageIndex = 0;
    unsigned int lastKey;
    int pointCount = 0;
    
    PossibleShapes currentShape = none;
    Mode currentMode = translate;
    matrix origin = matrix(1,4);
    
    vector<double> coordinates;
    vector<Image*> images;
    vector<ViewContext*> views;
    vector<string> fileNames;
};
        
#endif
