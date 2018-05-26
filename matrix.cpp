#include "matrix.h"
#include <string>
#include <cmath>
#include <vector>

using namespace std;

// Parameterized constructor
matrix::matrix(unsigned int rows, unsigned int cols):rows(rows),cols(cols) {
  
	if (rows < 1 || cols < 1) {
		throw matrixException("constructor sizes must be greater than 0");
	}
	
	the_matrix = new double[rows*cols]();
}

matrix::matrix(unsigned int rows, unsigned int cols, double* data):rows(rows),cols(cols) {
  
	if (rows < 1 || cols < 1 ) {
		throw matrixException("constructor sizes must be greater than 0");
	} 
	
	the_matrix = new double[rows*cols]();
	
	if( data != NULL ) {
	    //assign values to matrix
        for( unsigned int i = 0; i < rows*cols; i++ ) {
            the_matrix[i] = data[i];
        }
	}
}

// Copy constructor
matrix::matrix(const matrix& from):rows(from.rows),cols(from.cols) {

    the_matrix = new double[rows*cols]();

    //assign values to matrix
	for( unsigned int i = 0; i < rows; i++ ) {
	    for( unsigned int j = 0; j < cols; j++ ) {
	        the_matrix[i*cols+j] = from[i][j];
	    }
	}
}

// Destructor
matrix::~matrix() {
	delete [] the_matrix;
}

int matrix::getSize() {
    return rows*cols;
}

int matrix::getRows() {
    return rows;
}

int matrix::getCols() {
    return cols;
}

// Assignment operator
matrix& matrix::operator=(const matrix& rhs) {

    //return if nothing to copy
	if( &rhs == this ) {    
	    return *this;
	}
	
	//only reset matrix and size if size changed
    if( rows != rhs.rows || cols != rhs.cols ) {    
        rows = rhs.rows;
        cols = rhs.cols;
        delete [] the_matrix;
        the_matrix = new double[rows*cols]();
    }

    //reassign values to matrix
	for( unsigned int i = 0; i < rows; i++ ) {
	    for( unsigned int j = 0; j < cols; j++ ) {
	        the_matrix[i*cols+j] = rhs[i][j];
	    }
	}
	
	return *this;
}

// Named constructor (static)
matrix matrix::identity(unsigned int size) {

    //create a size x size matrix with all 0's
	matrix iden(size,size);
	
	//change values to 1 where appropriate
	for( unsigned int i = 0; i < size; i++ ) {
	    iden[i][i] = 1.0;
	}

	return iden;
}


// Binary operations
matrix matrix::operator+(const matrix& rhs) const {

    //check compatibility
	if( rows != rhs.rows || cols != rhs.cols ) {
		throw matrixException("matricies must be same size to add");
	}

    //create same size matrix
	matrix retVal(rows,cols);
	
	//add each same location value and assign to new matrix
	for( unsigned int i = 0; i < rows; i++ ) {
	    for( unsigned int j = 0; j < cols; j++ ) {
	        retVal[i][j] = operator[](i)[j] + rhs[i][j];
	    }
	}
	
	return retVal;
}


matrix matrix::operator*(const matrix& rhs) const {

    //check compatibility
	if( cols != rhs.rows ) {
	    throw matrixException("matrix 1 must have same number of cols as rows in matrix 2 to multiply");
	}
	
	//create new matrix with dimensions defined by original two
	matrix retVal(this->rows,rhs.cols);
	
	//assign values
	for( unsigned int i = 0; i < this->rows; i++ ) {
	    for( unsigned int j = 0; j < rhs.cols; j++ ) {
	        for( unsigned int k = 0; k < rhs.rows; k++ ) {
	            retVal[i][j] += the_matrix[i*cols+k] * rhs[k][j];
	        }
	    }
	}
	
	return retVal;
}

matrix matrix::operator*(const double scale) const {
    
    //copy current matrix into new one
	matrix retVal(*this);

    //multiply new matrix by scale
	for( unsigned int i = 0; i < rows; i++ ) {
	    for( unsigned int j = 0; j < cols; j++ ) {
	        retVal[i][j] *= scale;
	    }
	}
	
	return retVal;
}


// Unary operations
matrix matrix::operator~() const {

    //create new matrix with rows and cols values swapped
	matrix retVal(cols,rows);
	
	//assign values
	for( unsigned int i = 0; i < rows; i++ ) {
	    for( unsigned int j = 0; j < cols; j++ ) {
	        retVal[j][i] = operator[](i)[j];
	    }
	}
	
	return retVal;
}
	

void matrix::clear() {

    //set all matrix values to 0
	std::fill_n(the_matrix, rows*cols, 0);
	
	return;
}

double* matrix::operator[](unsigned int row) {

    //return the address of the row
    if( row >= 0 && row < rows ) {
	    return &the_matrix[row*cols];
	}
	
	return NULL;
}

double* matrix::operator[](unsigned int row) const {

    //return the address of the row
    if( row >= 0 && row < rows ) {
	    return &the_matrix[row*cols];
	}
	
	return NULL;
}


std::ostream& matrix::out(std::ostream& os) const {

    //print all matrix values formatted properly
	for( unsigned int i = 0; i < rows; i++, os << std::endl ) {
	    for( unsigned int j = 0; j < cols; j++ ) {
	        os << operator[](i)[j];
	        
	        if( j != cols-1 ) {
	            os << " ";
	        }
	    }
	}
	
	return os;	
}



// Global insertion and operator
std::ostream& operator<<(std::ostream& os, const matrix& rhs) {

	rhs.matrix::out(os);
	return os;
}

// Global scalar multiplication
matrix operator*(const double scale, const matrix& rhs) {	
	return rhs * scale;
}

void matrix::floatErrorCorrect( double eps ) {

    for( unsigned int i = 0; i < rows*cols; i++ ) {
	    if( fabs(the_matrix[i]) < eps ) {
	        the_matrix[i] = 0.0;
	    }
	}
	
}

matrix squareInverse( matrix& mat ) {

    if( mat.getRows() != mat.getCols() ) {
        throw matrixException("matricies must be square to use the squareInverse function");
    }
    
    matrix inverse( mat.getCols(), mat.getRows() );
    
    if( mat.getSize() == 1 ) {
        inverse[0][0] = 1/mat[0][0];
        return inverse;
    }
    
    matrix subMat( mat.getCols()-1, mat.getRows()-1 );
    double det = 0;
    
    //find sub matrix
    for( int k = 1; k < mat.getRows(); k++ ) {
        for( int l = 1; l < mat.getCols(); l++ ) {
            subMat[k-1][l-1] = mat[k][l];
        }
    }
    
    for( int i = 0; i < mat.getRows(); i++ ) {
    
        //recalc sub after row change skipping row i
        for( int k = 0; k < subMat.getRows(); k++ ) {
            if( k < i ) {
                for( int l = 1; l < mat.getCols(); l++ ) {
                    subMat[k][l-1] = mat[k][l];
                }
            } else {
                for( int l = 1; l < mat.getCols(); l++ ) {
                    subMat[k][l-1] = mat[k+1][l];
                }
            }
        }
    
        for( int j = 0; j < mat.getCols(); j++ ) {
        
            //change sub matrix col j-1 to col j-1 of mat
            if( j > 0 ) {
                for( int l = 0; l < subMat.getRows(); l++ ) {
                    subMat[(i+l)%subMat.getRows()][j-1] = mat[(i+l+1)%mat.getRows()][j-1];
                }
            }
            
            //set equal to det of sub mat
            if( (i+j)%2 == 0 ) { 
                inverse[j][i] = squareDeterminant( subMat );
            } else {
                inverse[j][i] = -squareDeterminant( subMat );
            }
        }
    }
    
    //shortcut for determinant when inverse is known
    for( int i = 0; i < mat.getRows(); i++ ) {
        det += mat[0][i] * inverse[i][0];
    }
    
    return (1/det) * inverse;

}

double squareDeterminant( matrix& mat ) {

    if( mat.getRows() != mat.getCols() ) {
        throw matrixException("matricies must be square to use the squareDeterminant function");
    }   
    
    //return det of 1x1 matrix right away
    if( mat.getSize() == 1 ) {
        return mat[0][0];
    }
    
    //return det of 2x2 matrix right away also to save some time
    if( mat.getSize() == 4 ) {
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    }
    
    //return det of 3x3 matrix right away also to save some time
    if( mat.getSize() == 9 ) {
        return mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
               mat[1][0] * (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2]) +
               mat[2][0] * (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]);
    }
    
    //return det of 4x4 matrix right away also to save some time
    if( mat.getSize() == 16 ) {
        return mat[0][0] * ( mat[1][1] * (mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2]) - 
                             mat[2][1] * (mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2]) +
                             mat[3][1] * (mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2]) ) + 
               mat[0][1] * (-mat[1][0] * (mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2]) + 
                             mat[2][0] * (mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2]) - 
                             mat[3][0] * (mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2]) ) + 
               mat[0][2] * ( mat[1][0] * (mat[2][1] * mat[3][3] - mat[2][3] * mat[3][1]) - 
                             mat[2][0] * (mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1]) + 
                             mat[3][0] * (mat[1][1] * mat[2][3] - mat[1][3] * mat[2][1]) ) + 
               mat[0][3] * (-mat[1][0] * (mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1]) +
                             mat[2][0] * (mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1]) - 
                             mat[3][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) );
    }
    
    
    
    double det = 0;
    matrix subMat( mat.getCols()-1, mat.getRows()-1 );
    
    //find sub matrix
    for( int k = 1; k < mat.getRows(); k++ ) {
        for( int l = 1; l < mat.getCols(); l++ ) {
            subMat[k-1][l-1] = mat[k][l];
        }
    }
    
    for( int i = 0; i < mat.getRows(); i++ ) {
    
        //change sub matrix row i-1 to mat row i-1
        if( i > 0 ) {
            for( int j = 1; j < mat.getCols(); j++ ) {
                subMat[i-1][j-1] = mat[i-1][j];
            }
        }

        if( i%2 == 0 ) {
            det += mat[i][0] * squareDeterminant( subMat );
        } else {
            det -= mat[i][0] * squareDeterminant( subMat );
        } 
    }
    
    return det;
}


