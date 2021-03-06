#include <math.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include "character.h"
#include "framework.h"
#include "game.h"
#define PI 3.14

using namespace std;


Character::Character(Game* game, GLfloat size){
    this->gameObject = game;
    
    gX = 0;
    gY = -200;
    gTheta = 0;
    
    handColor = defaultColors.handColor;
    handStroke = defaultColors.handStroke;
    
    this->torsoRadius = size * 0.8;
    this->handRadius = 0.45 * size;
    this->noseRadius = size / 4;
    this->outsideRadius = size * 4.0;
    this->headRadius = 0.6 * size;
    
    this->armLength = size*1.3;
    this->foreArmLength = size*1.5;
    this->armWidth = size/3;

    this->legLength = this->armLength * 0.5;
    this->legWidth = this->armWidth * 0.5;

    this->jointRadius = this->armWidth/2;

    GLfloat space = 1;

    this->hand = new Sphere(this->handRadius, space);
    this->head = new Sphere(this->headRadius, space);
    this->torso = new Sphere(this->torsoRadius, space);
    this->armJoint = new Sphere(this->jointRadius, space);
    this->legJoint = new Sphere(this->jointRadius * 0.85, space);
};

Character::Character(Game* game, GLfloat size, Point2D position, GLfloat angle){
    this->gameObject = game;
    
    armsColor = defaultColors.armsColor;
    handColor = defaultColors.handColor;
    handStroke = defaultColors.handStroke;
    
    this->torsoRadius = 0.8 * size * 0.8;
    this->handRadius = 0.5 * size;
    this->noseRadius = size / 4;
    this->outsideRadius = size * 3.0;
    this->headRadius = 0.6 * this->torsoRadius;
    
    this->armLength = size*1.3;
    this->foreArmLength = size*1.5;
    this->armWidth = size/3;

    this->legLength = this->armLength * 0.6;
    this->legWidth = this->armWidth * 0.5;
    
    this->jointRadius = this->armWidth/2;

    this->gX = position.x;
    this->gY = position.y;
    this->gTheta = angle;

    GLfloat space = 10;

    this->hand = new Sphere(this->handRadius, space);
    this->head = new Sphere(this->headRadius, space);
    this->torso = new Sphere(this->torsoRadius, space);
    this->armJoint = new Sphere(this->jointRadius, space);
    this->legJoint = new Sphere(this->jointRadius * 0.85, space);
};

void Character::DrawRectangle(GLint height, GLint width, Color color)
{
    GLint x1 = -width/2, x2 = width/2;
    GLint y1 = 0, y2 = height;
    glBegin(GL_POLYGON);
        glColor3f(color.R, color.G, color.B);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}
void Character::DrawCuboid(GLint height, GLint width, GLint thickness, Color color)
{
   GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
   GLfloat materialColor[] = { 1.0, 1.0, 0.0, 1.0};
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
   GLfloat mat_shininess[] = { 128 };
   glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
   glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    GLfloat color_r[] = {color.R, color.G, color.B, 1};

    glPushMatrix();
            glColor3fv(color_r);
            glScalef(width, height,thickness);
            glTranslatef(0, 0.5, 0); 
            glutSolidCube(1.0);
    glPopMatrix();
}

void Character::DrawCircle(GLint radius, Color color)
{
    float theta;
    float step = 20.0;
    glColor3f(color.R, color.G, color.B);
    glBegin(GL_POLYGON);
        for(int i = 0; i < 360/step; i++) {
            theta = i*step*3.14/180;
            glVertex2f(radius*cos(theta), radius*sin(theta));
        }
    glEnd();
}

void Character::DrawStroke(GLint radius, Color color)
{
    float theta;
    float step = 20.0;
    glColor3f(color.R, color.G, color.B);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < 360/1; i++) {
            theta = i*1*3.14/180;
            glVertex2f(radius*cos(theta), radius*sin(theta));
        }
    glEnd();
}

void Character::DrawCircleDashed(GLint radius, Color color)
{
    glPointSize(2);
    float theta;
    glColor3f(color.R, color.G, color.B);

    GLfloat materialColor[] = {color.R, color.G, color.B, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT,materialColor );

    for(int i = 0; i < 360/40; i++) {
        theta = i*40*3.14/180;
        glPushMatrix();
            glTranslatef(radius*cos(theta), radius*sin(theta), 0);
            glutSolidSphere(5, 10, 20);
        glPopMatrix();
    }
}

void Character::DrawLeftArms(GLfloat x, GLfloat y)
{   
    //Interpola????o
    GLfloat x0 = -30, y0 = MIN_LEFT_ANGLE;
    GLfloat X, Y = this->leftArmFirstJointAngle;
    GLfloat x1 = 15, y1 = MAX_LEFT_ANGLE;
    X = x0 + (x1-x0)*(Y-y0)/(y1-y0);

    glPushMatrix();
    glTranslatef(x, y, 0); /* Move to left arm base */
    glRotatef(this->leftArmFirstJointAngle, 0, 0, 1);
    glRotatef(X, 1, 0, 0);
    this->armJoint->Draw(this->armsColor); /* Smooth joint */
    this->DrawCuboid(this->armLength, this->armWidth, 7, this->armsColor);
    
    glTranslatef(0, this->armLength, 0);  /* Move to second left arm joint */
    glRotatef(this->leftArmSecondJointAngle, 0, 0, 1);
    this->armJoint->Draw(this->armsColor); /* Smooth joint */
    this->DrawCuboid(this->foreArmLength, this->armWidth, 7, this->armsColor);
    
    glTranslatef(0, this->foreArmLength, 0);  /* Move to second right arm tip */
    glTranslatef(0, handRadius, 0);
    this->DrawHand();
    
    glPopMatrix();
}

void Character::DrawRightArms(GLfloat x, GLfloat y)
{   
    //Interpola????o
    GLfloat x0 = -30, y0 = MIN_RIGHT_ANGLE;
    GLfloat X, Y = this->rightArmFirstJointAngle;
    GLfloat x1 = 15, y1 = MAX_RIGHT_ANGLE;
    X = x0 + (x1-x0)*(Y-y0)/(y1-y0);

    glPushMatrix();
    glTranslatef(x, y, 0); /* Move to right arm base */
    glRotatef(this->rightArmFirstJointAngle, 0, 0, 1);
    glRotatef(X, 1, 0, 0);
    this->armJoint->Draw(this->armsColor); /* Smooth joint */
    this->DrawCuboid(this->armLength, this->armWidth, 7, this->armsColor);
    
    glTranslatef(0, this->armLength, 0);  /* Move to second right arm joint */
    glRotatef(this->rightArmSecondJointAngle, 0, 0, 1);
    this->armJoint->Draw(this->armsColor); /* Smooth joint */
    this->DrawCuboid(this->foreArmLength, this->armWidth, 7, this->armsColor);

    glTranslatef(0, this->foreArmLength, 0);  /* Move to second right arm tip */
    glTranslatef(0, handRadius, 0);
    this->DrawHand();
    
    glPopMatrix();
}

bool Character::RotateLeftArm(GLfloat inc, bool applyFix)
{
    if(!characterIsEnabled) return false;
    
    if(applyFix)
        inc = this->gameObject->applyTimeFix(inc);
    
    if(this->leftArmFirstJointAngle >= MIN_LEFT_ANGLE && inc > 0) {
        this->nextNPCState(CharacterPunchSignal::MIN_REACHED);
        return false;
    }
    else if(this->leftArmFirstJointAngle <= MAX_LEFT_ANGLE && inc < 0) {
        this->nextNPCState(CharacterPunchSignal::MAX_REACHED);
        return false;
    }
    
    this->leftArmFirstJointAngle += inc;
    this->leftArmSecondJointAngle -= inc*0.80;
    return true;
}

bool Character::RotateRightArm(GLfloat inc, bool applyFix)
{
    if(!characterIsEnabled) return false;
    
    if(applyFix)
        inc = this->gameObject->applyTimeFix(inc);
    
    if(this->rightArmFirstJointAngle <= MIN_RIGHT_ANGLE && inc > 0) {
        this->nextNPCState(CharacterPunchSignal::MIN_REACHED);
        return false;
    }
    else if(this->rightArmFirstJointAngle >= MAX_RIGHT_ANGLE && inc < 0) {
        this->nextNPCState(CharacterPunchSignal::MAX_REACHED);
        return false;
    }
    
    this->rightArmFirstJointAngle -= inc;
    this->rightArmSecondJointAngle += inc*0.80;
    return true;
}

void Character::RotateLeftArmToAngle(GLfloat angle)
{
    this->leftArmFirstJointAngle = angle;
    this->leftArmSecondJointAngle = -MIN_ANGLE - (angle - MIN_ANGLE)*0.80;
}
void Character::RotateRightArmToAngle(GLfloat angle)
{
    this->rightArmFirstJointAngle = angle;
    this->rightArmSecondJointAngle = MIN_ANGLE - (angle+MIN_ANGLE)*0.80;
}

void Character::DrawHand()
{
    this->hand->Draw(this->handColor, this->handTexture);
}

void Character::DrawTorso()
{   
    this->torso->Draw(this->torsoColor, this->torsoTexture);    
}

// void Character::DrawNose()
// {
//     glPushMatrix();
//     glTranslatef(0, this->torsoRadius, 0);
    
//     glTranslatef(0, this->noseRadius, 0);
    
//     this->DrawSphere(this->noseRadius, this->noseColor);
//     glPopMatrix();
// }

void Character :: DrawHead(GLfloat x, GLfloat y, GLfloat z){
    glPushMatrix();

    glTranslatef(x, y, z);
    
    this->head->Draw(this->torsoColor);

    glPopMatrix();
}

void Character :: DrawLeftLeg(){
    glPushMatrix();
        glTranslatef(-this->torsoRadius*0.5, 0, -(this->torsoRadius + this->legLength/2));
        glRotatef(90 - leftLegAngle, 1, 0, 0); //Use this angle to rotate the whole leg
        this->DrawCuboid(this->legLength, this->legWidth, 10, this->armsColor);
        glRotatef(-90, 1, 0, 0);
        this->legJoint->Draw(this->armsColor); //Smooth joint
        glRotatef(-90 - this->leftSecondLegAngle, 1, 0, 0);   //Use this angle to rotate the "second leg"
        this->DrawCuboid(this->legLength, this->legWidth, 10, this->armsColor);
    glPopMatrix();
}

void Character :: DrawRightLeg(){
     glPushMatrix();
        glTranslatef(this->torsoRadius*0.5, 0, -(this->torsoRadius + this->legLength/2));
        glRotatef(90 - rightLegAngle, 1, 0, 0);
        this->DrawCuboid(this->legLength, this->legWidth, 10, this->armsColor);
        glRotatef(-90, 1, 0, 0);
        this->legJoint->Draw(this->armsColor); //Smooth joint
        glRotatef(-90 - rightSecondLegAngle, 1, 0, 0);
        this->DrawCuboid(this->legLength, this->legWidth, 10, this->armsColor);
    glPopMatrix();
}

void Character::DrawCharacter(GLfloat x, GLfloat y)
{
    glPushMatrix();
        glTranslatef(x, y, 0);              //Move to character position
        glRotatef(this->gTheta, 0, 0, 1);   //Rotate character
        
        this->DrawHead(0, 0, this->torsoRadius);
        this->DrawTorso();
        this->DrawLeftArms(-this->torsoRadius, 0);
        this->DrawRightArms(this->torsoRadius, 0);
        this->DrawLeftLeg();
        this->DrawRightLeg();

        if(showOutsideRadius){
            this->DrawCircleDashed(outsideRadius, Color(255, 255, 255));
        }

    glPopMatrix(); 
}

void Character::MoveForward(GLfloat dx, bool applyFix)
{
    if(applyFix)
        dx = this->gameObject->applyTimeFix(dx);
        
    this->nextNPCState(CharacterPunchSignal::NONE);
    
    if(willColide(gameObject, dx)) return;

    bool moved = false;

    Vector3D* charPosition = new Vector3D(0, 0, 0);

    Transformation* tr = new Transformation();
    tr->translate3d(this->gX, this->gY, this->gZ);

    GLfloat inc_y = dx * cos(Util :: degToRad(-this->gTheta));
    GLfloat inc_x = dx * sin(Util :: degToRad(-this->gTheta));

    Rectangle rect = Rectangle(this->gameObject->arena.x, this->gameObject->arena.y, this->gameObject->arena.width, this->gameObject->arena.height);

    GLfloat leftWall = rect.x;
    GLfloat rightWall = rect.x + rect.width;
    GLfloat bottomWall = rect.y;
    GLfloat topWall = rect.y + rect.height;

    GLfloat nx = this->gX + inc_x;
    GLfloat ny = this->gY + inc_y;

    if(fabs(ny - topWall) >= this->torsoRadius and fabs(ny - bottomWall) >= this->torsoRadius){
        tr->translate3d(0, inc_y, 0);
        moved = true;
    }

    if(fabs(nx - rightWall) >= this->torsoRadius and fabs(nx - leftWall) >= this->torsoRadius){
        tr->translate3d(inc_x, 0, 0);
        moved = true;
    }
    
    tr->apply3D(charPosition);
    
    this->gX = charPosition->x;
    this->gY = charPosition->y;
    this->gZ = charPosition->z;

    if(moved)
        walkMovement(dx);
}

bool Character::willColideWithOtherPlayer(Character* another, GLfloat dx)
{
    if(another == this) return false;
    Vector3D* thisPoint = new Vector3D(0, 0, 0);
    moveForwardTransform(dx)->apply3D(thisPoint);
    
    Circle thisCircle = Circle(thisPoint->x, thisPoint->y, this->torsoRadius);
    Circle anotherCircle = Circle(another->gX, another->gY, this->outsideRadius);
    
    return Collision::circleCircleIntersect(thisCircle, anotherCircle);
}

void Character::AnotherCharacterIsWithinRadius(Character* another, GLfloat dx)
{
    if(another == this) return;
    Vector3D* thisPoint = new Vector3D(0, 0, 0);
    moveForwardTransform(dx)->apply3D(thisPoint);
    
    Circle thisCircle = Circle(thisPoint->x, thisPoint->y, this->outsideRadius);
    Circle anotherCircle = Circle(another->gX, another->gY, another->outsideRadius);
    
    bool colliding = Collision::circleCircleIntersect(thisCircle, anotherCircle);
    if(colliding){
        this->charState = CharacterState::AGGRESSIVE;
    } else {
        this->charState = CharacterState::PASSIVE;
    }
}

bool Character::willColideWithGameWalls(GLfloat dx) {
    Rectangle rect = Rectangle(this->gameObject->arena.x, this->gameObject->arena.y, this->gameObject->arena.width, this->gameObject->arena.height);

    Vector3D* thisPoint = new Vector3D(0, 0, 0);
    moveForwardTransform(dx)->apply3D(thisPoint);
    Circle circ = Circle(thisPoint->x, thisPoint->y, this->torsoRadius);

    bool isIntersecting = Collision::circleInsideRectIntersect(circ, rect);
    return isIntersecting;
}


bool Character::willColide(Game* game, GLfloat dx) {
    dx = this->gameObject->applyTimeFix(dx);
//    return false;
    
    // Test aggresiveness
    AnotherCharacterIsWithinRadius(game->player1, dx);
    AnotherCharacterIsWithinRadius(game->player2, dx);
    
    if(willColideWithOtherPlayer(game->player1, dx)) return true;
    if(willColideWithOtherPlayer(game->player2, dx)) return true;
    // if(willColideWithGameWalls(dx)) return true;
    
    return false;
}

void Character::hitDetection(Character* another) {
    if(!characterIsEnabled) return;
    if(another == this) return;
    
    if(this->punchState == CharacterPunchState::LEFT_PUNCH) {
        Vector3D* thisPoint = new Vector3D(0, 0, 0);
        leftGloveTransform()->apply3D(thisPoint);

        Sphere* thisSphere = new Sphere(thisPoint->x, thisPoint->y, thisPoint->z, this->handRadius);
        Sphere* anotherSphere = new Sphere(another->gX, another->gY, another->gZ + this->torsoRadius, this->headRadius);

        bool didCollide = Collision :: sphereSphereIntersect(thisSphere, anotherSphere);

        // Circle thisCircle = Circle(thisPoint->x, thisPoint->y, this->handRadius);
        // Circle anotherCircle = Circle(another->gX, another->gY, this->torsoRadius);
        
        // bool didCollide = Collision::circleCircleIntersect(thisCircle, anotherCircle);

        if(didCollide && countPoint){
            handleHitOpponent(another);
        }
        
    } else if(this->punchState == CharacterPunchState::RIGHT_PUNCH) {
        Vector3D* thisPoint = new Vector3D(0, 0, 0);
        rightGloveTransform()->apply3D(thisPoint);

        Sphere* thisSphere = new Sphere(thisPoint->x, thisPoint->y, thisPoint->z, this->handRadius);
        Sphere* anotherSphere = new Sphere(another->gX, another->gY, another->gZ  + this->torsoRadius, this->headRadius);

        bool didCollide = Collision :: sphereSphereIntersect(thisSphere, anotherSphere);
        
        // Circle thisCircle = Circle(thisPoint->x, thisPoint->y, this->handRadius);
        // Circle anotherCircle = Circle(another->gX, another->gY, another->torsoRadius);
        
        // bool didCollide = Collision::circleCircleIntersect(thisCircle, anotherCircle);
        if(didCollide && countPoint){
            handleHitOpponent(another);
        }
    }
}


void Character::RotateBody(GLfloat inc) {
    inc = this->gameObject->applyTimeFix(inc);
    this->gTheta -= inc;
}

void Character::followCharacter(Game* game, Character* other, GLfloat dx) {
    if(!characterIsEnabled) return;
    Point2D* charPosition = new Point2D(0, 0);
    
    GLfloat angle = atan2(this->gY - other->gY, this->gX - other->gX);
    GLfloat deg = (angle*180)/PI;
    
    this->gTheta = deg + 90;
    
    this->MoveForward(dx/2);
}

void Character::setColor(Color _color) {
    this->torsoColor = _color;
}

GLfloat Character::getLeftMouseAngle(GLfloat xDistance) {
    GLfloat mappedValue = Util::map(xDistance, 0, (float)this->gameObject->arena.width/2, MIN_LEFT_ANGLE, MAX_LEFT_ANGLE);
    return Util::clamp(mappedValue, MAX_LEFT_ANGLE, MIN_LEFT_ANGLE);
}
GLfloat Character::getRightMouseAngle(GLfloat xDistance) {
    GLfloat mappedValue = Util::map(xDistance, 0, (float)this->gameObject->arena.width/2, MIN_RIGHT_ANGLE, MAX_RIGHT_ANGLE);
    return Util::clamp(mappedValue, MIN_RIGHT_ANGLE, MAX_RIGHT_ANGLE);
}

void Character::handlePlayerPunchControls() {
    if(!characterIsEnabled) return;
    
    if(this->gameObject->player1 == this){
        if(this->gameObject->mouse.leftButton.isPressed && !characterIsMoving()) {
            GLfloat xDistance = this->gameObject->mouse.currentPosition.x - this->gameObject->mouse.leftButton.clickPosition.x;
            
            GLfloat finalAngle;
            if(this->gameObject->mouse.currentPosition.x > this->gameObject->mouse.leftButton.clickPosition.x){
                if(punchState == CharacterPunchState::LEFT_PUNCH) resetHitOpponent();
                
                this->setPunchState(CharacterPunchState::RIGHT_PUNCH);
                finalAngle = getRightMouseAngle(abs(xDistance));
                this->RotateRightArmToAngle(finalAngle);
                this->RotateLeftArmToAngle(MIN_ANGLE);
            }
            else if(this->gameObject->mouse.currentPosition.x < this->gameObject->mouse.leftButton.clickPosition.x){
                if(punchState == CharacterPunchState::RIGHT_PUNCH) resetHitOpponent();
                
                this->setPunchState(CharacterPunchState::LEFT_PUNCH);
                finalAngle = getLeftMouseAngle(abs(xDistance));
                this->RotateLeftArmToAngle(finalAngle);
                this->RotateRightArmToAngle(-MIN_ANGLE);
            }
            else {
                resetHitOpponent();
                
                this->RotateLeftArmToAngle(MIN_LEFT_ANGLE);
                this->RotateRightArmToAngle(MIN_RIGHT_ANGLE);
            }
        } else {
            this->setPunchState(CharacterPunchState::IDLE);
            this->RotateRightArm(4, false);
            this->RotateLeftArm(4, false);
        }
    }
}

void Character::Draw(){
    DrawCharacter(gX, gY);
    
    hitDetection(this->gameObject->player1);
    hitDetection(this->gameObject->player2);
    
    handlePunchControls();
};

bool Character::characterIsMoving() {
    vector<char> moveKeys = {'w', 'a', 's', 'd', 'W', 'A', 'S', 'D'};
    for(char key : moveKeys) {
        if(this->gameObject->isKeyPressed(key)){
            this->gameObject->mouse.leftButton.setIsPressed(false);
            return true;
        }
    }
    return false;
}

void Character :: walkMovement(GLfloat inc){
    inc = fabs(inc)*0.7;

    GLfloat legMaxAngle = 20;

    if(this->moveLeftLeg){
        this->leftLegAngle += inc;
        this->rightLegAngle -= inc;

        if(Util :: gte(this->leftLegAngle , legMaxAngle))
            this->moveLeftLeg = !this->moveLeftLeg;

        if(Util :: lt(this->leftLegAngle, 0))
            this->leftSecondLegAngle += 2*inc;
        else
            this->leftSecondLegAngle -= 2*inc;

        if(Util :: lt(this->leftSecondLegAngle, 0))
            this->leftSecondLegAngle = 0;

    }else{
        this->leftLegAngle -= inc;
        this->rightLegAngle += inc;

        if(Util :: gte(this->rightLegAngle, legMaxAngle))
            this->moveLeftLeg = !this->moveLeftLeg;

        if(Util :: lt(this->rightLegAngle, 0))
            this->rightSecondLegAngle += 2*inc;
        else
            this->rightSecondLegAngle -= 2*inc;

        if(Util :: lt(this->rightSecondLegAngle, 0))
            this->rightSecondLegAngle = 0;
    }    
}

void Character :: resetLegAngles(){
    leftLegAngle = 0;
    rightLegAngle = 0;
    leftSecondLegAngle = 0;
    rightSecondLegAngle = 0;
}
