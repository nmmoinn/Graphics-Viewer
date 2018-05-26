
#include "line.h"
#include "shape.h"
#include "gcontext.h"
#include "matrix.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>


using namespace std;


Shape::Shape() {}

Shape::Shape( unsigned int color, matrix& origin ) {
    
    if( origin.getCols() != 4 ) {
        throw shapeException("origin for shape object requires a 1x4 matrix of the form {x,y,z,1}");
    } else {
        this->origin = matrix( 4,1, origin[0] );
    }

    this->color = color;
}

Shape::Shape( const Shape& shape ) { 
    this->origin = matrix( shape.origin );
    this->color = shape.color;   
}

Shape::~Shape() {

}

void Shape::setColor( unsigned int color ) {
    this->color = color;
}

void Shape::out(std::ostream& os) const {

    os << "color " << color << std::endl;
    os << "origin " << ~origin;
}

vector<string> split(const string& s, char delimiter) {

    vector<string> words;
    string word;
    istringstream wordStream(s);
    
    while ( getline( wordStream, word, delimiter ) ) {
        words.push_back(word);
    }
    
    return words;
}

vector<string> removeGarbage( vector<string> trim ) {

    vector<string> final;

    for ( uint32_t i = 0; i < trim.size(); i++ ) {  
       
        if( trim[i] != "" ) {
            final.push_back(trim[i]);
        }
       
    }
    
    return final;
}
