#include "x11context.h"
#include <unistd.h>
#include <iostream>
#include "drawing.h"
#include "line.h"
#include "triangle.h"
#include <chrono>
#include "viewContext.h"

using namespace std;

int main(void) {
    
    double origin[] = { 400,300,0,1 };
    matrix morigin( 1, 4, &origin[0] ); 
    
    GraphicsContext* gc = new X11Context(800,600,GraphicsContext::BLACK);
    
    Drawing md( morigin );
    
    gc->runLoop(&md);
    
    delete gc;
    return 0; 
}
