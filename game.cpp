#include "game.h"
#include "character.h"
#include "game-config.h"
#include "xmlparser.h"

void Game::initializeCharacters(GLfloat sizep1, GLfloat sizep2){
    this->player1 = new Character(this, 20);
    this->player2 = new Character(this, 20);
}

void Game::PrintScore()
{
   GLfloat x = this->arena.x+20, y = this->arena.y+20;
   glColor3f(1.0, 1.0, 1.0);
   //Cria a string a ser impressa
   char *tmpStr;
   sprintf(this->str, "Player: %d vs Enemy: %d", this->player1->hitScore, this->player2->hitScore);
   //Define a posicao onde vai comecar a imprimir
   glRasterPos2f(x, y);
   //Imprime um caractere por vez
   tmpStr = this->str;
   while( *tmpStr ){
       glutBitmapCharacter(this->font, *tmpStr);
       tmpStr++;
   }
}

void Game::Output(int x, int y, Color color, string str)
{
  glColor3f( color.R, color.G, color.B );
  glRasterPos2f(x, y);
  int len, i;
  len = str.size();
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(this->font, str[i]);
  }
}

void Game::DrawCuboid(GLint width, GLint height, GLint thickness){
    GLfloat color_r[] = {1,0,0, 1};
        glPushMatrix();
                glColor3fv(color_r);
                glScalef(width, height,thickness);
                glutSolidCube(1.0);
        glPopMatrix();
}


void Game::DrawArena(GLfloat x, GLfloat y, GLfloat z) {
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
    GLfloat materialColor[] = { 0.0, 0.0, 1.0, 1.0};
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0};
    GLfloat mat_shininess[] = { 64 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    GLfloat thickness = this->player1->torsoRadius * 2;

    glPushMatrix();
        glTranslatef(x,y,z - (this->player1->torsoRadius + this->player1->legLength*2 + thickness/2 - 2*this->player1->jointRadius));
        DrawCuboid(this->arena.width, this->arena.height, thickness);
    glPopMatrix();
}

void Game::DrawGameOver()
{
    auto calcSize = [](string newStr) -> int { return (newStr.size() * 9)/2; };
    
    string winString = "You Win!";
    string loseString = "You Lose!";
    string tieString = "Game Over!";
    string restartString = "Press 'M' to restart the game";
    
    GLfloat centerX =this->arena.x+this->arena.width/2;
    GLfloat centerY = this->arena.y+this->arena.height/2;
    
    if(player1->hitScore > player2->hitScore)
        Output(centerX - calcSize(winString), centerY, Color(0,255,0), winString);
    
    if(player1->hitScore < player2->hitScore)
        Output(centerX - calcSize(loseString), centerY, Color(255,0,0), loseString);
    
    if(player1->hitScore == player2->hitScore)
        Output(centerX - calcSize(tieString), centerY, Color(0,0,255), tieString);
    
    Output(centerX - calcSize(restartString), centerY - 20, Color(255,255,255), restartString);
}

void Game::keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
             keyStatus[(int)('a')] = 1; //Using keyStatus trick
             break;
        case 'd':
        case 'D':
             keyStatus[(int)('d')] = 1; //Using keyStatus trick
             break;
        case 'w':
        case 'W':
             keyStatus[(int)('w')] = 1; //Using keyStatus trick
             break;
        case 's':
        case 'S':
             keyStatus[(int)('s')] = 1; //Using keyStatus trick
             break;
        case 'p':
        case 'P':
             player2->toggleCharacterEnabled();
             break;
        case 'o':
        case 'O':
            player1->toggleOutsideRadius();
            player2->toggleOutsideRadius();
            break;
        case 'm':
        case 'M':
            resetGame();
            break;
        case 'i':
        case 'I':
            this->maxScore = maxScore == 10 ? 50 : 10;
            break;
        case 'h':
        case 'H':
            player1->toggleShowHitmark();
            player2->toggleShowHitmark();
            break;
         case '1':
            cameraNumber = 1;
            break;
        case '2':
            cameraNumber = 2;
            break;
        case '3':
            cameraNumber = 3;
            break;
        case '+':
        {
            if(cameraNumber != 3) 
                break;
            int inc = cameraAngle >= 180 ? 0 : 1;
            cameraAngle += inc;
            changeCamera(cameraAngle, 
                    glutGet(GLUT_WINDOW_WIDTH), 
                    glutGet(GLUT_WINDOW_HEIGHT));
            break;
        }
        case '-':
        {
            if(cameraNumber != 3) 
                break;
            int inc = cameraAngle <= 5 ? 0 : 1;
            cameraAngle -= inc;
            changeCamera(cameraAngle, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;
        }
        case 'n':
            spotLight = !spotLight;
            break;
        case 27 :
            if(gameIsOver)
             exit(0);
            else {
                this->player1->hitScore = 0;
                this->player2->hitScore = 0;
                gameOver();
            }
        
    }
    
    glutPostRedisplay();
}


void Game::setPlayerStartPosition(Character* pl, GLfloat x, GLfloat y, GLfloat angle) {
    pl->setInitialPos(x, y, angle);
}

Game::Game(string xmlPath) {
    this->xmlPath = xmlPath;
    GameInitObject initObject = XMLParser::parseArena(xmlPath);
    
    this->player1 = new Character(this, initObject.player1.headSize, initObject.player1.position,  initObject.player1.angle - 90);
//    this->player1->setColor(Color(90, 128, 184));
    this->player1->setPlayerType(CharacterType::PLAYER);
    this->player2 = new Character(this, initObject.player2.headSize, initObject.player2.position,  initObject.player2.angle - 90);
//    this->player2->setColor(Color(78, 173, 71));
    this->player2->setPlayerType(CharacterType::ENEMY);

    this->arena = initObject.arena;
}

void Game::resetGame() {
    gameIsOver = false;
    GameInitObject initObject = XMLParser::parseArena(xmlPath);
    
    this->player1 = new Character(this, initObject.player1.headSize, initObject.player1.position,  initObject.player1.angle - 90);
    this->player1->setColor(Color(90, 128, 184));
    this->player1->setPlayerType(CharacterType::PLAYER);
    this->player2 = new Character(this, initObject.player2.headSize, initObject.player2.position,  initObject.player2.angle - 90);
    this->player2->setColor(Color(78, 173, 71));
    this->player2->setPlayerType(CharacterType::ENEMY);

    this->arena = initObject.arena;
}

Game::Game() {
    this->arena = Rectangle(0,0,0,0);
}

void Game::gameOver() {
    this->player1->characterIsEnabled = false;
    this->player2->characterIsEnabled = false;
    gameIsOver = true;
}

void Game::checkGameOver() {
    if(this->player1->hitScore >= this->maxScore || this->player2->hitScore >= this->maxScore) {
        gameOver();
    }
}



void Game :: setCamera(){
    if(cameraNumber == 1){
            Vector3D* inFrontOfNose = new Vector3D(0, 0, 0);
            this->player1->moveForwardTransform(this->player1->headRadius)->apply3D(inFrontOfNose);
            Transformation* tr = new Transformation();
            tr->translate3d(0, 0, this->player1->headRadius + this->player1->torsoRadius);

            Vector3D* inFrontOfNoseMais10 = new Vector3D(0, 0, 0);
            this->player1->moveForwardTransform(this->player1->headRadius + 10)->apply3D(inFrontOfNoseMais10);

            tr->apply3D(inFrontOfNose);
            tr->apply3D(inFrontOfNoseMais10);

            gluLookAt( 
                            inFrontOfNose->x,
                            inFrontOfNose->y,
                            inFrontOfNose->z,
                            inFrontOfNoseMais10->x,
                            inFrontOfNoseMais10->y,
                            inFrontOfNoseMais10->z,
                            0, 0, 1);
    }else if(cameraNumber == 2){
        Vector3D* foreArm = new Vector3D(0, 0, 0);
        Vector3D* hand = new Vector3D(0, 0, 0);

        Transformation* tr = new Transformation();
        tr->translate3d(this->player1->gX, this->player1->gY, 0);
        tr->rotate3d(this->player1->gTheta, 0, 0, 1);
        tr->translate3d(-this->player1->torsoRadius, 0, 0);
        tr->rotate3d(this->player1->leftArmFirstJointAngle, 0, 0, 1);
        tr->translate3d(0, this->player1->armLength, 0);
        tr->rotate3d(this->player1->leftArmSecondJointAngle, 0, 0, 1);
        tr->translate3d(0, this->player1->foreArmLength/2, this->player1->handRadius);
        tr->apply3D(foreArm);

        tr->translate3d(0, this->player1->foreArmLength/2, 0);
        tr->translate3d(0, this->player1->handRadius, 0);
        tr->apply3D(hand);

        gluLookAt( 
                            foreArm->x,
                            foreArm->y,
                            foreArm->z,
                            hand->x,
                            hand->y,
                            hand->z,
                            0, 0, 1);

    }else{
    
        Vector3D* vet = new Vector3D(0, 0, 0);

        Transformation* tr = new Transformation();
        tr->translate3d(this->player1->gX, this->player1->gY, 0);
        tr->rotate3d(this->player1->gTheta + camXYAngle, 0, 0, 1);
        tr->rotate3d(camXZAngle, 1, 0, 0);
        tr->translate3d(0, -500, 500);
        tr->apply3D(vet);

        Vector3D* up = new Vector3D(0, 1, 0);
        Transformation* tr2 = new Transformation();
        tr2->rotate3d(this->player1->gTheta + camXYAngle, 0, 0, 1);
        tr2->rotate3d(camXZAngle, 1, 0, 0);
        tr2->apply3D(up);
        
        gluLookAt( 
                            vet->x,
                            vet->y,
                            vet->z,
                            this->player1->gX,
                            this->player1->gY,
                            this->player1->gZ,
                            up->x, up->y, up->z);  

    }
}

void Game:: changeCamera(int angle, int w, int h)
{
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity ();

    gluPerspective (angle, 
            (GLfloat)w / (GLfloat)h, 1, this->arena.width + this->arena.height);

    glMatrixMode (GL_MODELVIEW);

    glutPostRedisplay();

}

void Game :: setIlumination(){
    if(spotLight){
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        
        GLfloat position_params[] = {this->player1->gX, this->player1->gY, this->player1->gZ + this->player1->torsoRadius + 2*this->player1->headRadius + 500, 1.0};
        glLightfv(GL_LIGHT1, GL_POSITION, position_params);

        GLfloat ambient_params[] = {0.2, 0.2, 0.2, 1};
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_params);

        GLfloat diffuse_params[] = {1, 1, 1, 1};
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_params);
        
        GLfloat spotCutoff_params[] = {45.0};
        glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spotCutoff_params); 
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

        GLfloat spotDirection_params[] = {0, 0, -1};
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection_params);

    }else{
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT0);
        GLfloat ambient_params[] = {0.2, 0.2, 0.2, 1};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_params);

        GLfloat position_params[] = {this->arena.x + this->arena.width/2, this->arena.y + this->arena.height/2, 500, 0};
        glLightfv(GL_LIGHT0, GL_POSITION, position_params);
    }
    
}


