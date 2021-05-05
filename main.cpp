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

void renderScene(void)
{   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor (0.0,0.0,0.0, 1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game->PrintScore();
    if(game->gameIsOver){
        game->DrawGameOver();
        glutSwapBuffers();
        return;
    }

    game->DrawMinimap();
    game->setTexture();
    game->setCamera();
    game->setIlumination();

    if(!game->gameIsOver){
        game->DrawGame();
    }
    
    if(!(game->player1->characterIsMoving()))
        game->player1->resetLegAngles();

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
    for(i = 0; i < 256; i++)
       game->keyStatus[i] = 0;
}

void init(void)
{
    ResetKeyStatus();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
    glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL); 

    game->LoadTexture();
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
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
