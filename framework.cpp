#include "framework.h"
#include <math.h>
#include <iostream>
#include <vector>
#define PI 3.14

using namespace std;
void initFramework() {
    srand (static_cast <unsigned> (time(0)));
}

void Circle::setColor(Color _color) {
    this->color = _color;
}

void Transformation::log(){
    if(shouldLog) {
        cout << "Matrix:" << endl;
        for(auto c : this->matrix) {
            for(auto b : c) {
                cout << b << ",";
            }
            cout << endl;
        }
        cout << endl;
    }
};

Matrix Transformation::getIdentity(){
    Matrix identity = {
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };

    return identity;
};

void Transformation::logTranslate(GLfloat x, GLfloat y){
    if(shouldLog){
        cout << "Translate: " << x << ", " << y << endl;
        this->log();
    }
};

void Transformation::logScale(GLfloat x, GLfloat y){
    if(shouldLog){
        cout << "Scale: " << x << ", " << y << endl;
        this->log();
    }
};

void Transformation::logRotate(GLfloat angle){
    if(shouldLog){
        cout << "Rotate: " << angle << endl;
        this->log();
    }
};
    
void Transformation::matrixMultiply(Matrix mat2, int n){
    Matrix mat1 = this->matrix;
            
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            this->matrix[i][j] = 0;
            for (k = 0; k < n; k++)
                this->matrix[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
};
void Transformation::matrixVectorMultiply(Matrix vet1,
                          Matrix &res, int n){
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 1; j++) {
            res[i][j] = 0;
            for (k = 0; k < n; k++){
                res[i][j] += this->matrix[i][k] * vet1[k][j];
            }
        }
    }
};


void Transformation::translate2d(GLfloat x, GLfloat y){
    Matrix translateMatrix = {
        {1, 0, x},
        {0, 1, y},
        {0, 0, 1}
    };
    matrixMultiply(translateMatrix, 3);
    this->logTranslate(x, y);
};

void Transformation::scale2d(GLfloat Sx, GLfloat Sy){
    Matrix scaleMatrix = {
        {Sx, 0,  0},
        {0,  Sy, 0},
        {0,  0,  1}
    };
    matrixMultiply(scaleMatrix, 3);
    this->logScale(Sx, Sy);
};

void Transformation::rotate2d(GLfloat angle){
    GLfloat rad = angle*PI/180;
    Matrix rotationMatrix = {
        {cos(rad), -sin(rad), 0},
        {sin(rad), cos(rad), 0},
        {0, 0, 1}
    };
    matrixMultiply(rotationMatrix, 3);
    this->logRotate(angle);
};

void Transformation::apply(Point2D* point){
    Matrix vet = {
        {point->x},
        {point->y},
        {1}
    };
    
    matrixVectorMultiply(vet, vet, 3);

    point->x =vet[0][0];
    point->y =vet[1][0];
};


void Transformation::translate3d(GLfloat x, GLfloat y, GLfloat z){
    Matrix translateMatrix = {
        {1,  0,  0, x},
        {0,  1,  0, y},
        {0,  0,  1, z},
        {0,  0,  0, 1}
    };
    matrixMultiply(translateMatrix, 4);
    this->logTranslate(x, y);
};

void Transformation::scale3d(GLfloat Sx, GLfloat Sy, GLfloat Sz){
    Matrix scaleMatrix = {
        {Sx, 0,   0, 0},
        {0,  Sy,  0, 0},
        {0,  0,  Sz, 0},
        {0,  0,   0, 1}
    };
    matrixMultiply(scaleMatrix, 4);
    this->logScale(Sx, Sy);
};
//
//void Transformation::rotate3d(GLfloat angle, int x, int y, int z){
//    GLfloat rad = angle*PI/180;
//    Matrix rotationXMatrix = {
//        {1,    0,        0,         0},
//        {0,    cos(rad), -sin(rad), 0},
//        {0,    sin(rad), cos(rad),  0},
//        {0,    0,        0,         1}
//    };
//    Matrix rotationYMatrix = {
//        {cos(rad),  0,    sin(rad),  0},
//        {0,         1,    0,         0},
//        {-sin(rad), 0,    cos(rad),  0},
//        {0,         0,    0,         1}
//    };
//    Matrix rotationZMatrix = {
//        {cos(rad), -sin(rad), 0,  0},
//        {sin(rad), cos(rad),  0,  0},
//        {0,        0,         1,  0},
//        {0,        0,         0,  1}
//    };
//
//    if(x) {
//        matrixMultiply(rotationMatrix);
//    } if (x) {
//        matrixMultiply(rotationMatrix);
//    }
//    matrixMultiply(rotationMatrix);
//    this->logRotate(angle);
//}


void Transformation::rotate3d(GLfloat angle, GLfloat x, GLfloat y, GLfloat z){
    Vector3D vet = Vector3D(x,y,x).normalize();
    
    GLfloat rad = angle*PI/180;
    
    GLfloat c = cos(rad);
    GLfloat s = sin(rad);
    GLfloat onemc = 1 - c;
    GLfloat ux = vet.x;
    GLfloat ux2 = ux*ux;
    GLfloat uy = vet.y;
    GLfloat uy2 = uy*uy;
    GLfloat uz = vet.z;
    GLfloat uz2 = uz*uz;
    
    Matrix totalRotation = {
        {c + ux2*onemc,     ux*uy*onemc-uz*s,       ux*uz*onemc+uy*s,   0},
        {ux*uy*onemc-uz*s,  c+uy2*onemc,            uy*uz*onemc+ux*s,   0},
        {ux*uz*onemc-uy*s,  uy*uz*onemc-ux*s,       c+uz2*onemc,        0},
        {0,                 0,                      0,                  1}
    };
    
    matrixMultiply(totalRotation, 4);
    this->logRotate(angle);
}

void Transformation::apply3D(Vector3D* vector){
    Matrix vet = {
        {vector->x},
        {vector->y},
        {vector->z},
        {1}
    };
    
    matrixVectorMultiply(vet, vet, 4);

    vector->x = vet[0][0];
    vector->y = vet[1][0];
    vector->z = vet[2][0];
};


void Transformation::logMode(bool state) {
    this->shouldLog = state;
};
