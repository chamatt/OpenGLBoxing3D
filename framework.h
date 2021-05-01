#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifndef FRAMEWORK_H
#define	FRAMEWORK_H

#include <math.h>
#include <iostream>
#include <vector>
#include "SOIL.h"


#define PI 3.14

using namespace std;

void initFramework();

class Point2D;
class Circle;
class Rectangle;
class Color;
class Random;
class Collision;

typedef vector< vector<GLfloat> > Matrix;

class Util {
public:
    static GLfloat degToRad(GLfloat angle) {
        GLfloat rad = (angle*PI)/180.0;
        return rad;
    }
    
    static GLfloat radToDeg(GLfloat angle) {
        GLfloat deg = (angle*180.0)/PI;
        return deg;
    }
    
    static GLfloat map(GLfloat x, GLfloat in_min, GLfloat in_max, GLfloat out_min, GLfloat out_max)
    {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    
    static GLfloat clamp(GLfloat v, GLfloat lo, GLfloat hi) {
        GLfloat clamped_value = v;
        if      ( v < lo ) clamped_value = lo;
        else if ( v > hi ) clamped_value = hi;
        return clamped_value;
    }

    static bool equal(GLfloat a, GLfloat b){
        GLfloat epsilon = 1e-7;
        return fabs(a - b) < epsilon;
    }

    static bool lt(GLfloat a, GLfloat b){
        GLfloat epsilon = 1e-7;
        return a + epsilon < b; 
    }

};


class Point2D {
    public:
        GLfloat x;
        GLfloat y;

        Point2D(GLfloat x, GLfloat y){
            this->x = x;
            this->y = y;
        };
    
        GLfloat distanceTo(Point2D* another) {
            return sqrt(pow(this->x - another->x, 2) + pow(this->y - another->y, 2));
        }
    
        // Angle in degrees
        GLfloat getAngle() {
            GLfloat theta_rad = atan2(this->y, this->x);
            GLfloat theta_deg = (theta_rad/M_PI*180) + (theta_rad > 0 ? 0 : 360);
            
            return theta_deg;
        }
        
        Point2D operator-(Point2D other) {
//            cout << this->toString() << " - " << other.toString() << "=" << Point2D(this->x - other.x, this->y - other.y).toString() << endl;
            
            return Point2D(this->x - other.x, this->y - other.y);
        }
    
        string toString() {
            return "Point(" + to_string(this->x) + "," + to_string(this->y) + ")";
        }
};


class Vector3D
{
public:
    GLfloat x, y, z;
    Vector3D() : x(0), y(0), z(0) {
    }
    Vector3D(GLfloat nx, GLfloat ny, GLfloat nz) : x(nx), y(ny), z(nz) {}

    Vector3D& normalize() {
        GLfloat invLength = 1.0f / sqrtf(x*x + y*y + z*z);
        
        this->x *= invLength;
        this->y *= invLength;
        this->z *= invLength;
        return *this;
    }
};



class Random {
    public:
        GLfloat number;
        
        Random() {
            this->number = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
        Random(GLfloat X) {
            this->number = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X));
        }
        Random(GLfloat LO, GLfloat HI) {
            this->number = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        }
};

class Color {
public:
    GLfloat R;
    GLfloat G;
    GLfloat B;
    
    string colorString;
    
    Color() {
        this->R = 0;
        this->G = 0;
        this->B = 0;
    }
    
    Color(GLfloat R, GLfloat G, GLfloat B) {
        this->R = (float)R/(float)255;
        this->G = (float)G/(float)255;
        this->B = (float)B/(float)255;
    }
    
    Color(string _color): colorString(_color) {}
};

class Circle {
public:
    GLfloat x;
    GLfloat y;
    Point2D point;
    GLfloat radius;
    Color color;
    
    void setColor(Color _color);
    
    Circle(GLfloat px, GLfloat py, GLfloat pradius) : x(px), y(py), radius(pradius), point(Point2D(px, py)) {}
    
    string toString() {
        return "Circle(" + to_string(this->x) + "," + to_string(this->y) + "," + to_string(this->radius) + ")";
    }
};

class Rectangle {
public:
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
    
    Rectangle(GLfloat px, GLfloat py, GLfloat pwidth, GLfloat pheight) : x(px), y(py), width(pwidth), height(pheight) {}
    
    string toString() {
        return "Rectangle(" + to_string(this->x) + "," + to_string(this->y) + "," + to_string(this->x + this->width) + "," + to_string(this->y + this->height) + ")";
    }
};

class Collision {
public:
    
    static bool circleInsideRectIntersect(Circle circle, Rectangle rect) {
        GLfloat leftWall = rect.x;
        GLfloat rightWall = rect.x + rect.width;
        GLfloat bottomWall = rect.y;
        GLfloat topWall = rect.y + rect.height;
        
        if(circle.x - circle.radius < leftWall) return true;
        if(circle.x + circle.radius > rightWall) return true;
        if(circle.y - circle.radius < bottomWall) return true;
        if(circle.y + circle.radius > topWall) return true;
        return false;
    }
    
    static bool circleCircleIntersect(Circle circle1, Circle circle2) {
        GLfloat intersectionRadius = circle1.radius + circle2.radius;
        return circle2.point.distanceTo(&circle1.point) < intersectionRadius;
    }
};

class Transformation {
    private:
        bool shouldLog = false;
        
        void log();
        
        void logTranslate(GLfloat x, GLfloat y);
        
        void logScale(GLfloat x, GLfloat y);
    
        void logRotate(GLfloat angle);
    
        void matrixMultiply(Matrix mat2, int n);

        void matrixVectorMultiply(Matrix vet1,
                                  Matrix &res, int n);

    public:
        Matrix matrix;

        Matrix getIdentity();
        Matrix getIdentity3D();
        Transformation(){
            this->matrix = this->getIdentity();
            this->log();
        };

        void translate2d(GLfloat x, GLfloat y);
    
        void scale2d(GLfloat Sx, GLfloat Sy);

        void rotate2d(GLfloat angle);
    
        void translate3d(GLfloat x, GLfloat y, GLfloat z);

        void scale3d(GLfloat Sx, GLfloat Sy, GLfloat Sz);

        void rotate3d(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

        void apply(Point2D* point);
        
        void apply3D(Vector3D* vector);
    
        void logMode(bool state);
};

class Texture{
public:
    GLuint texture;
    void LoadTextureRAW( const char * filename );
};

class Vertice{
public:
     //Vertex coordinate
    GLfloat X;
    GLfloat Y;
    GLfloat Z;
    
    //Vertex normal 
    GLfloat nX;
    GLfloat nY;
    GLfloat nZ;
    
    //Vertex texture coordinate
    GLfloat U;
    GLfloat V;
};

class Sphere{
private:
    void calculateCoordinates(GLfloat current_v, GLfloat current_h, int idx){
        this->vertices[idx]->X = radius * sin(current_h / 180 * M_PI) * sin(current_v / 180 * M_PI);   
        this->vertices[idx]->Y = - radius * cos(current_h / 180 * M_PI) * sin(current_v / 180 * M_PI);
        this->vertices[idx]->Z = radius * cos(current_v / 180 * M_PI);
        this->vertices[idx]->V = current_v / 180;
        this->vertices[idx]->U = current_h / 360;
        GLfloat norm = sqrt(this->vertices[idx]->X * this->vertices[idx]->X +
            this->vertices[idx]->Y * this->vertices[idx]->Y +
            this->vertices[idx]->Z * this->vertices[idx]->Z);

        this->vertices[idx]->nX = this->vertices[idx]->X/norm;
        this->vertices[idx]->nY = this->vertices[idx]->Y/norm;
        this->vertices[idx]->nZ = this->vertices[idx]->Z/norm;
    }
public:
    vector <Vertice*> vertices;
    int numberOfVertices;
    GLfloat radius;
    Sphere(GLfloat radius, GLfloat space=1){
        this->radius = radius;
        this->numberOfVertices = (180 / space) * (2 + 360 / (2*space)) * 4;
        this->vertices.resize(this->numberOfVertices);

        for(int i=0;i<this->numberOfVertices;i++)
            vertices[i] = new Vertice();

        int idx = 0;

        for(GLfloat v = 0;v <= 180-space; v += space)
            for(GLfloat h = 0;h <= 360+2*space; h += 2*space){
                GLfloat current_v = v;
                GLfloat current_h = h;
                calculateCoordinates(current_v, current_h, idx);
                
                idx++;

                current_v = v + space;
                current_h = h;
                calculateCoordinates(current_v, current_h, idx);

                idx++;

                current_v = v;
                current_h = h + space;
                calculateCoordinates(current_v, current_h, idx);

                idx++;

                current_v = v + space;
                current_h = h + space;
                calculateCoordinates(current_v, current_h, idx);

                idx++;
            }
    }
    void Draw( Color color, Texture* texture = NULL){
        GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
        GLfloat materialColorA[] = { 0.2, 0.2, 0.2, 1};
        GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
        GLfloat mat_shininess[] = { 100.0 };
        glColor3f(1,1,1);
    
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        if(texture){
            glBindTexture (GL_TEXTURE_2D, texture->texture);
        }
        
        glBegin (GL_TRIANGLE_STRIP);
        for ( int i = 0; i <this->numberOfVertices; i++){
            glNormal3f(this->vertices[i]->nX, this->vertices[i]->nY, this->vertices[i]->nZ);

            if(texture){
                glTexCoord2f(this->vertices[i]->U, this->vertices[i]->V);
            }

            glVertex3f (this->vertices[i]->X, this->vertices[i]->Y, this->vertices[i]->Z);
        }
        glEnd();

        glBindTexture (GL_TEXTURE_2D, 0);
        }
};


#endif	/* FRAMEWORK_H */
