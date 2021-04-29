#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "framework.h"
#include "character.h"
#include "game.h"
#define INC_KEY 13
#define INC_KEYIDLE 2

#define PI 3.14

Game* game;

void DrawAxes()
{
    GLfloat color_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat color_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat color_b[] = { 0.0, 0.0, 1.0, 1.0 };

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
 
        //x axis
        glPushMatrix();
            glColor3fv(color_r);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(10.0);
        glPopMatrix();

        //y axis
        glPushMatrix();
            glColor3fv(color_g);
            glRotatef(90,0,0,1);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(10.0);
        glPopMatrix();

        //z axis
        glPushMatrix();
            glColor3fv(color_b);
            glRotatef(-90,0,1,0);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // put in one end
            glutSolidCube(10.0);
        glPopMatrix();
    glPopAttrib();
    
}

void renderScene(void)
{   
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

     // Clear the screen.
     glClearColor (0.0,0.0,0.0, 1.0);
     glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


     game->setCamera();

    if(game->gameIsOver){
        game->DrawGameOver();
    } else {
        game->player1->Draw();
        game->player2->Draw();
        game->DrawArena(game->arena.x + game->arena.width/2, game->arena.y + game->arena.height/2, 0);

        //  glPushMatrix();
        //     glTranslatef(game->arena.x + game->arena.width/2, game->arena.y + game->arena.height/2, 0);
        //     DrawAxes();
        // glPopMatrix();
        // game->PrintScore();
    }
    

     glutSwapBuffers(); // Desenha the new frame of the game->
}


void reshape (int w, int h) {

    glViewport (0, 0, (GLsizei)w, (GLsizei)h);

    game->changeCamera(game->cameraAngle, w, h);
}

void keyup(unsigned char key, int x, int y)
{
    game->keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       game->keyStatus[i] = 0;
}

void init(void)
{
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
 
    glShadeModel (GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    
    // glMatrixMode(GL_PROJECTION); // Select the projection matrix    
    // glOrtho(game->arena.x,     // X coordinate of left edge
    //         game->arena.x + game->arena.width,     // X coordinate of right edge
    //         game->arena.y,     // Y coordinate of bottom edge
    //         game->arena.y + game->arena.height,     // Y coordinate of top edge
    //         -100,     // Z coordinate of the “near” plane            
    //         100);    // Z coordinate of the “far” plane
    // glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    // glLoadIdentity();
      
}


GLdouble currentTime, timeDiference;

GLfloat moveByTime(GLfloat amount) {
    return amount * (timeDiference/20);
}

void frameCorrectionFix() {
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDiference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;
}

void idle(void)
{
    game->checkGameOver();
    game->frameCorrectionFix();
    double inc = INC_KEYIDLE;
    
    game->player2->followCharacter(game, game->player1, inc);
    
    //Treat keyPress
    if(game->isKeyPressed('a'))
    {
        game->player1->RotateBody(-inc);
    }
    if(game->isKeyPressed('d'))
    {
        game->player1->RotateBody(inc);
    }

    if(game->isKeyPressed('w'))
    {
        game->player1->MoveForward(inc);
    }
    if(game->isKeyPressed('s'))
    {
        game->player1->MoveForward(-inc);
    }
    
//    cout << "Pos:" << game->player1->gX << game->player1->gY << game->player1->gZ << endl;
    
    glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y) {
    game->keyPress(key, x, y);
}

void mouseEvent(int button, int state, int x, int y) {
    game->mouseEvent(button, state, x, y);
};
void mouseMovement(int x, int y) {
    game->mouseMovement(x, y);
}
 
int main(int argc, char *argv[])
{
    initFramework();
    
    game = new Game(argv[1]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Boxing 2D");
 
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
  
    glutMouseFunc(mouseEvent);
    glutPassiveMotionFunc(mouseMovement);
    glutMotionFunc(mouseMovement);
    
    init();
 
    glutMainLoop();
 
    return 0;
}
