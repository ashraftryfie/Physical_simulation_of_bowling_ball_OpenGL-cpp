#include <windows.h>		// Header File For Windows
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glut.h>		// Header File For The GLut Library
//#include <gl\glaux.h>		// Header File For The Glaux Library
#include <texture.h>
//#include <math3d.h>
//#include <glm/glm.hpp>
#include "lib\Model_3DS.h"
#include "lib\3DTexture.h"
#include "lib\TgaLoader.h"
#include "lib\camera.h"
//#include "lib/objloader.hpp"

HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

using namespace std;


/************** main **************/
float initialSpeedMain, throwAngleMain, ballMassMain, radiusBALLMain, frictionMain, reflectionMain;


/************** Constants **************/
const float GRAVITY = 9.8f;

/************** Variables **************/
float force, time, rolAngle = 0, distanceZ = 0, pinFallAngle = 0, zMPin = 0, vPin = 0;
bool afterCollision = false;

/************** Camera **************/
Camera MyCamera;

/************** Models **************/
Model_3DS ball, table, pin, chair;

/************** Textures **************/
int floor1, floor2, wall, wall2, bowlingImage, painting3, painting5, curtain, door, light;
GLTexture pinImg, ballImg;

/************** Lighting **************/
GLfloat LightPos[] = { 0.0f, 0.0f, -20.0f, 1.0f };
GLfloat LightDir[] = { 0.0f, 0.0f, -60.0f, 1.0f };
GLfloat LightAmb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat LightDiff[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat LightSpec[] = { 0.2f, 0.2f, 0.2f, 1.0f };

GLfloat MaterialAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialDif[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat MaterialSpec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat MaterialShn[] = { 128.0f };

GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)

/************** Shadow **************/
//M3DMatrix44f shadow;
//M3DVector4f equation;
GLUquadric* quadric;

bool stopInc = true;


class Place {
private:
	// Positions of Streams
	int LEFT_BORDER = 0; //const 
	int RIGHT_BORDER = 0; //const 
	float length, height, width;

public:
	Place() {
		length = 35;
		height = 10;
		width = 15;
		LEFT_BORDER = 0;
		RIGHT_BORDER = 0;
	}

	float getLength() {
		return length;
	}

	void floor(float x, float y, float z)
	{

		float h = -0.1;

		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		glColor3f(0.0, 0.0, 0.0);

		glVertex3f(x, y, z + 0.01);
		glVertex3f(x + 1.04, y, z + 0.01);
		glVertex3f(x + 1.04, y + 1, z + 0.01);
		glVertex3f(x, y + 1, z + 0.01);


		// black
		glVertex3f(x - 0.23, y, z + 0.01);
		glVertex3f(x, y, z + 0.01);
		glVertex3f(x, y + 1, z + 0.01);
		glVertex3f(x - 0.23, y + 1, z + 0.01);

		glVertex3f(x - 0.23, y + 1, z + 0.01);
		glVertex3f(x + 1.27, y + 1, z + 0.01);
		glVertex3f(x + 1.27, y + 1.23, z + 0.01);
		glVertex3f(x - 0.23, y + 1.23, z + 0.01);

		glVertex3f(x + 1.04, y, z + 0.01);
		glVertex3f(x + 1.27, y, z + 0.01);
		glVertex3f(x + 1.27, y + 1, z + 0.01);
		glVertex3f(x + 1.04, y + 1, z + 0.01);

		//floor

		glVertex3f(x, y, z);
		glVertex3f(x, y, z + 18);
		glVertex3f(x + 1.04, y, z + 18);
		glVertex3f(x + 1.04, y, z);

		glVertex3f(x, y + h, z);
		glVertex3f(x, y + h, z + 18);
		glVertex3f(x + 1.04, y + h, z + 18);
		glVertex3f(x + 1.04, y + h, z);

		glVertex3f(x, y, z);
		glVertex3f(x, y + h, z);
		glVertex3f(x, y + h, z + 18);
		glVertex3f(x, y, z + 18);

		glVertex3f(x + 1.04, y, z);
		glVertex3f(x + 1.04, y + h, z);
		glVertex3f(x + 1.04, y + h, z + 18);
		glVertex3f(x + 1.04, y, z + 18);

		glVertex3f(x, y, z);
		glVertex3f(x, y + h, z);
		glVertex3f(x + 1.04, y + h, z);
		glVertex3f(x + 1.04, y, z);

		glVertex3f(x, y, z + 18);
		glVertex3f(x, y + h, z + 18);
		glVertex3f(x + 1.04, y + h, z + 18);
		glVertex3f(x + 1.04, y, z + 18);

		glEnd();

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, floor1);
		glBegin(GL_QUADS);

		glTexCoord2d(0, 0); 	glVertex3d(x, y, z + 18);
		glTexCoord2d(1, 0); 	glVertex3d(x + 1.04, y, z + 18);
		glTexCoord2d(1, 1); 	glVertex3d(x + 1.04, y, z + 18 - 1.04);
		glTexCoord2d(0, 1); 	glVertex3d(x, y, z + 18 - 1.04);

		glEnd();

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, floor2);
		glBegin(GL_QUADS);

		glTexCoord2d(0, 0); 	glVertex3d(x, y, z + 18 - 1.04);
		glTexCoord2d(1, 0); 	glVertex3d(x + 1.04, y, z + 18 - 1.04);
		glTexCoord2d(1, 1); 	glVertex3d(x + 1.04, y, z);
		glTexCoord2d(0, 1); 	glVertex3d(x, y, z);

		glEnd();

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, curtain);
		glBegin(GL_QUADS);

		glTexCoord2d(1, 0);  glVertex3f(x, y, z + 0.01);
		glTexCoord2d(0, 0);  glVertex3f(x + 1.04, y, z + 0.01);
		glTexCoord2d(0, 1);  glVertex3f(x + 1.04, y + 1, z + 0.01);
		glTexCoord2d(1, 1);  glVertex3f(x, y + 1, z + 0.01);

		glEnd();

		glDisable(GL_TEXTURE_2D);

	}

	void stream(float x, float y, float z)
	{

		glDisable(GL_TEXTURE_2D);

		float h = -0.1;

		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 0.0);

		glVertex3f(x, y, z);
		glVertex3f(x, y, z + 18);
		glVertex3f(x + 0.23, y, z + 18);
		glVertex3f(x + 0.23, y, z);

		glVertex3f(x, y + h, z);
		glVertex3f(x, y + h, z + 18);
		glVertex3f(x + 0.23, y + h, z + 18);
		glVertex3f(x + 0.23, y + h, z);

		glVertex3f(x, y, z);
		glVertex3f(x, y + h, z);
		glVertex3f(x, y + h, z + 18);
		glVertex3f(x, y, z + 18);

		glVertex3f(x + 0.23, y, z);
		glVertex3f(x + 0.23, y + h, z);
		glVertex3f(x + 0.23, y + h, z + 18);
		glVertex3f(x + 0.23, y, z + 18);

		glVertex3f(x, y, z);
		glVertex3f(x, y + h, z);
		glVertex3f(x + 0.23, y + h, z);
		glVertex3f(x + 0.23, y, z);

		glVertex3f(x, y, z + 18);
		glVertex3f(x, y + h, z + 18);
		glVertex3f(x + 0.23, y + h, z + 18);
		glVertex3f(x + 0.23, y, z + 18);

		glEnd();

	}

	void box(float x, float y, float z)
	{
		float xx = x - 15, yy = y - 0.1, zz = z - 0.1;
		float xDoor = (xx + length - 2) / 2.0;

		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		glColor3f(0.5f, 0.35f, 0.1f);

		glVertex3f(xx, yy, zz);
		glVertex3f(xx + length, yy, zz);
		glVertex3f(xx + length, yy, zz + length);
		glVertex3f(xx, yy, zz + length);

		glVertex3f(xx, yy + height, zz);
		glVertex3f(xx + length, yy + height, zz);
		glVertex3f(xx + length, yy + height, zz + length);
		glVertex3f(xx, yy + height, zz + length);

		glVertex3f(xx, yy, zz);
		glVertex3f(xx, yy, zz + length);
		glVertex3f(xx, yy + height, zz + length);
		glVertex3f(xx, yy + height, zz);

		glVertex3f(xx + length, yy, zz);
		glVertex3f(xx + length, yy, zz + length);
		glVertex3f(xx + length, yy + height, zz + length);
		glVertex3f(xx + length, yy + height, zz);


		glColor3f(0.3f, 0.35f, 0.1f);


		glVertex3f(xx, yy, zz);
		glVertex3f(xx + length, yy, zz);
		glVertex3f(xx + length, yy + height, zz);
		glVertex3f(xx, yy + height, zz);

		glVertex3f(xx, yy, zz + length);
		glVertex3f(xx + length, yy, zz + length);
		glVertex3f(xx + length, yy + height, zz + length);
		glVertex3f(xx, yy + height, zz + length);

		glEnd();

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, wall);
		glBegin(GL_QUADS);

		glTexCoord2d(1, 0);  glVertex3f(xx, yy, zz);
		glTexCoord2d(0, 0);  glVertex3f(xx + length, yy, zz);
		glTexCoord2d(0, 1);  glVertex3f(xx + length, yy, zz + length);
		glTexCoord2d(1, 1);  glVertex3f(xx, yy, zz + length);

		glEnd();

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, light);
		glBegin(GL_QUADS);

		glTexCoord2d(7, 0);  glVertex3f(xx, yy + height, zz);
		glTexCoord2d(0, 0);  glVertex3f(xx + length, yy + height, zz);
		glTexCoord2d(0, 5);  glVertex3f(xx + length, yy + height, zz + length);
		glTexCoord2d(7, 5);  glVertex3f(xx, yy + height, zz + length);

		glEnd();

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, door);
		glBegin(GL_QUADS);

		glTexCoord2d(0, 1);  glVertex3f(xDoor, yy, zz + length - 0.01);
		glTexCoord2d(0, 0);  glVertex3f(xDoor + 2, yy, zz + length - 0.01);
		glTexCoord2d(1, 0);  glVertex3f(xDoor + 2, yy + 2, zz + length - 0.01);
		glTexCoord2d(1, 1);  glVertex3f(xDoor, yy + 2, zz + length - 0.01);

		glEnd();


		glDisable(GL_TEXTURE_2D);

	}

	void drawTable(float x, float y, float z)
	{
		glDisable(GL_TEXTURE_2D);

		glColor3f(0.5, 0.5, 0.0);
		table.pos.x = x;
		table.pos.y = y;
		table.pos.z = z;
		table.scale = 0.0003;
		table.Draw();
	}

	void drawChair(float x, float y, float z, float degree, float t)
	{
		glDisable(GL_TEXTURE_2D);

		glColor3f(1.0, 0.3, 0.0);
		chair.pos.x = x;
		chair.pos.y = y;
		chair.pos.z = z;
		chair.scale = 0.008;
		if (t == 0)
			chair.rot.x = degree;
		else if (t == 1)
			chair.rot.y = degree;
		else if (t == 2)
			chair.rot.z = degree;
		chair.Draw();
	}

	void drawTableAndChair(float x, float y, float z)
	{
		y -= 0.1;
		drawTable(x, y, z);

		drawChair(x, y, z - 0.25, 0, 1);
		drawChair(x - 1, y, z - 0.8, 180, 1);
		drawChair(x - 0.2, y, z - 1, 90, 1);
		drawChair(x - 0.75, y, z, -90, 1);

	}

	void drawPainting(float x, float y, float z)
	{
		z += 0.01;

		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);

		//glColor3f(0.5f, 0.35f, 0.1f);

		glVertex3f(x + 13, y + 3, z);
		glVertex3f(x + 21.8, y + 3, z);
		glVertex3f(x + 21.8, y + 9.06, z);
		glVertex3f(x + 13, y + 9.06, z);

		glEnd();


		glBindTexture(GL_TEXTURE_2D, painting3);
		glBegin(GL_QUADS);

		glTexCoord2d(1, 0);  glVertex3f(x + 13, y + 3, z);
		glTexCoord2d(0, 0);  glVertex3f(x + 21.8, y + 3, z);
		glTexCoord2d(0, 1);  glVertex3f(x + 21.8, y + 9.06, z);
		glTexCoord2d(1, 1);  glVertex3f(x + 13, y + 9.06, z);

		glEnd();

		x += 0.01;
		glBindTexture(GL_TEXTURE_2D, painting5);
		glBegin(GL_QUADS);

		glTexCoord2d(0, 1);  glVertex3f(x, y + 3, z + 21.5);
		glTexCoord2d(1, 1);  glVertex3f(x, y + 3, z + 13.5);
		glTexCoord2d(1, 0);  glVertex3f(x, y + 9, z + 13.5);
		glTexCoord2d(0, 0);  glVertex3f(x, y + 9, z + 21.5);

		glEnd();
	}

	~Place() {}
};



class BowlingGame {

private:
	float xBall, height, zBall, radiusBall, xPin, zPin, mBall, mPin;
	int numOfThrows;
	//Throw *throws;
	//float xBall_curr, yBall_curr;

	GLfloat	throwVelocity,	//سرعة القذف الابتدائية
		throwAngle, //زاوية القذف
		maxHeight,	// الارتفاع الاقصى عن الارض
		time, time2,
		throwTime,	//زمن القذف
		throwTimeHelper,	//زمن القذف المساعد
		fallAngle,	//زاوية السقوط
		reflectionAngle,	//زاوية الارتداد
		reflectionVelocity,	//سرعة الارتداد
		currentX,	//الموضع الحالي
		throwingHeight,	//ارتفاع القذف
		x2,
		levelCount;	//رقم المرحلةالحالية
		
	float a, v, w, α;
	float e, μk;


public:
	BowlingGame()
	{
		//this->e = 0.5f;
		this->e = reflectionMain;
		//this->μk = 0.5f;
		this->μk = frictionMain;
		this->throwingHeight = 0.3f;
		this->xBall = 0.0f;
		this->height = throwingHeight;
		this->zBall = 0.0f;
		//this->radiusBall = 0.08f;
		this->radiusBall = radiusBALLMain;
		this->numOfThrows = 0;
		//this->throwVelocity = 1;
		this->throwVelocity = initialSpeedMain;
		//this->throwAngle = 0;
		this->throwAngle = throwAngleMain;
		this->maxHeight = ((throwVelocity * throwVelocity * pow(sin(throwAngle), 2)) / (2 * GRAVITY)) + throwingHeight;
		this->time = sqrt(2 * height / GRAVITY);
		this->time2 = 0;
		//this->throwTime = ((2 * throwVelocity * sin(throwAngle)) / GRAVITY);
		//this->throwTime = ((throwVelocity*sin(throwAngle))+sqrt(pow(throwVelocity * sin(throwAngle),2)-2*(-height)*GRAVITY));
		this->throwTime = ((throwVelocity * sin(throwAngle)) / GRAVITY) + (sqrt(2 * maxHeight / GRAVITY));
		this->throwTimeHelper = 0;
		this->fallAngle = 50 / sqrt(2 * GRAVITY * height);
		this->reflectionAngle = (3.14 / 2) - atan(throwVelocity / (e * sqrt(2 * GRAVITY * maxHeight)));
		this->reflectionVelocity = sqrt(throwVelocity * cos(throwAngle) * throwVelocity * cos(throwAngle) + (2 * maxHeight * GRAVITY * pow(e, 2)));
		this->currentX = reflectionVelocity * cos(throwAngle) * throwTime;
		this->mPin = 1.5;
		//this->mBall = 10;
		this->mBall = ballMassMain;
		this->zPin = 0;


		this->x2 = 0;
		this->levelCount = 1;
		this->a = 0;
		this->α = 0;
		this->v = 0;
		this->w = 0;

		//this->x_curr = 0;
		//this->y_curr = 0;
		//this->throws = nullptr;

	}

	float getxBall() {
		return xBall;
	}

	float getzBall() {
		return zBall;
	}

	float getzPin() {
		return zPin;
	}

	float getHeight() {
		return height;
	}

	void drawPin(float x, float y, float z, float movePin)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 1.0);  // color 

		glPushMatrix();
		pin.pos.x = x;
		pin.pos.y = y;
		pin.pos.z = z;
		pin.rot.x = -90;
		pin.scale = 0.01;
		
		glTranslated(0, 0, movePin);
		pin.rot.x += pinFallAngle;
		pin.Draw();
		glPopMatrix();

	}
	
	void pins(float x, float y, float z, float movePin)
	{
		/*drawPin(x + 0.17, y, z + 0.13);
		drawPin(x + 0.39, y, z + 0.13);
		drawPin(x + 0.61, y, z + 0.13);
		drawPin(x + 0.83, y, z + 0.13);

		drawPin(x + 0.28, y, z + 0.35);
		drawPin(x + 0.50, y, z + 0.35);
		drawPin(x + 0.72, y, z + 0.35);

		drawPin(x + 0.39, y, z + 0.57);
		drawPin(x + 0.61, y, z + 0.57);
		*/

		drawPin(x + 0.50, y, z + 0.79, movePin);
	}

	void drawBall(float xBall = 15.5f) {

		//glPushMatrix();
		GLfloat MatShn[] = { 128.0f };

		//glColor3f(1, 0, 0);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, bowlingImage);

		GLfloat MatAmb1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat MatDif1[] = { 0.0f, 0.0f, 0.6f, 1.0f };
		GLfloat MatSpec1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		MatShn[0] = 0;

		glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb1);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif1);
		glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpec1);
		glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);
		glEnable(GL_COLOR_MATERIAL);


		glTranslated(xBall, 0, -22);
		glRotated(rolAngle, 1, 0, 0);
		//glScaled(0.2, 0.2, 0.2);
		gluSphere(quadric, radiusBall, 32, 32);
		//auxSolidSphere(radiusBall);
		//glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	void throwingBall() {
		cout << "reflectionVelocity" << reflectionVelocity << endl;
		cout << "reflectionVelocity cos fff=" << reflectionVelocity * cos(reflectionAngle) << endl;

		cout << "maxHeight   ==" << maxHeight << endl;
		cout << "throwTime =" << throwTime << endl;
		cout << "Time =" << time << endl;
		cout << "h 1=" << height << endl;
		cout << "sin(throwAngle) =" << sin(throwAngle) << endl;
		cout << "reflectionAngle =" << reflectionAngle << endl;
		float speed = reflectionVelocity * cos(reflectionAngle);
		if (levelCount == 3) {
			rollingBall(speed);
		}
		else if (throwTime > 0 && levelCount == 2) {
			xBall = reflectionVelocity * cos(reflectionAngle) * throwTimeHelper + x2;
			height = reflectionVelocity * sin(reflectionAngle) * throwTimeHelper - (0.5 * GRAVITY * throwTimeHelper * throwTimeHelper) + throwingHeight + radiusBall;
			throwTime -= 0.05;
			throwTimeHelper += 0.05;

		}

		else if (throwTime >= throwTimeHelper && levelCount == 1) {
			xBall = throwVelocity * cos(throwAngle) * throwTimeHelper + x2;
			height = speed * sin(throwAngle) * throwTimeHelper - (0.5 * GRAVITY * throwTimeHelper * throwTimeHelper) + throwingHeight + radiusBall;
			//height = (-((GRAVITY) / (2 * pow(throwVelocity, 2) * pow(cos(throwAngle), 2)) * pow(xBall, 2)) + xBall * tan(throwAngle)) + throwingHeight;
			throwTimeHelper += 0.05;
			//time = sqrt(2 * height / GRAVITY);

		}

		else {
				if (levelCount == 2) {

					maxHeight = ((reflectionVelocity * reflectionVelocity * pow(sin(reflectionAngle), 2)) / (2 * GRAVITY));
					cout << "maxHeight 222===  " << maxHeight << endl;
					cout << "h" << height << endl;
					reflectionVelocity = sqrt(reflectionVelocity * cos(reflectionAngle) * reflectionVelocity * cos(reflectionAngle) + (2 * maxHeight * GRAVITY * pow(e, 2))) ;
					cout << "reflectionVelocity 2   =" << reflectionVelocity << endl;

					reflectionAngle = (PI / 2) - atan(reflectionVelocity / (e * sqrt(2 * GRAVITY * maxHeight)));

				}

				//if (reflectionAngle > 0.9) 
				if (reflectionVelocity * sin(reflectionAngle) > 0.5)
				{
					throwTime = ((2 * reflectionVelocity * sin(reflectionAngle)) / GRAVITY);
					throwTimeHelper = 0;
					throwingHeight = 0;
					x2 = xBall;
					levelCount = 2;
				}
				else
				{
					levelCount = 3;
					//u = 0;
					time2 = 0;
					height = radiusBall;

				}
			
		}

	}

	void rollingBall(float v0 = 0.5f) { // Params: friction,

		//-------- Slipping:
		// Acceleration
		if (stopInc) {
			a = -μk * GRAVITY;
			v = (float)(a * time2) + v0;
			α = 2.5f * ((μk * GRAVITY) / radiusBall); // (5/2)
			w = α * time2;
		}
		else {
			a = 0;
			α = 0;
		}
		// Velocity (v = a*t + v0)
		//float v = a * time2 + v0;


		//float m1 = 2.5f * ((μk * GRAVITY) / radiusBall); // (5/2)

		//float w = m1 * time2;

		//--------- Rolling:
		// (7/2)
		//v0 = 3.5f * μk * GRAVITY * time2;

		// Distance of slipping (12/49)
		float slippingDistance = 0.2448f * (pow(v0, 2) / (μk * GRAVITY));

		// time when ball stop slipping  (2/7)
		float t = 0.285f * v0 / (μk * GRAVITY);

		float z = 0.5f * a * pow(time2, 2) + (float)v * time2;
		//float z = v;


		// Calculate Rotation Angle
		float theta = w * time2;
		float rollingAngleValue = theta * (180 / PI); // rad -> dgree

		cout << "------------------------------------------------------------------------------------------------" << endl;
		cout << "| zBall = " << z;
		cout << " , t = " << t;
		cout << " , slippingDistance = " << slippingDistance;
		cout << " , time = " << time2 << endl;
		cout << "| a = " << a;
		cout << " , v0 = " << v0;
		cout << " , v = " << v;
		cout << " , w = " << w;
		cout << " , rolAngle = " << rolAngle;
		cout << " , rollingAngleValue = " << rollingAngleValue << endl;
		cout << "------------------------------------------------------------------------------------------------" << endl;

		if (w * radiusBall <= v) {
			time2 += 0.05f;
		}
		else {
			stopInc = false;
		}

		
		if (v > 0 && !checkPinCollision()){
			zBall -= z;
			if (afterCollision) {
				float zPin = (float)vPin * time2;
				zMPin -= zPin;
				pinFallAngle += 3;

			}
			
		}
		else {
			cout <<endl<< "!!!!!!!!!!!!!!!!!!!!!!!! Collision !!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			pinFallAngle += 5;
			v = calcFinalBallVelocity(v);
			cout << " Ball Final Velocity:\t" << v << endl;
			w = v / radiusBall;
			float z2 = (float)v * time2;
			zBall -= z2;
			cout << endl << "zBall :\t" << zBall << endl;
			vPin = calcFinalPinVelocity(v);
			cout << " Pin Final Velocity:\t" << vPin << endl;
			float zPin = (float)vPin * time2;
			this->zPin = zPin;
			zMPin -= zPin;
			afterCollision = true;
			cout << endl << "!!!!!!!!!!!!!!!!!!!!!!!! End Collision !!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			return;
		}

		

		if (abs(zBall) >= slippingDistance) {

			rolAngle -= rollingAngleValue;

		}


	}

	/* Check Collision With Pin*/
	bool checkPinCollision(float xBall=15.5){
		xPin = xBall;
		zPin = -39.21;
		float xDistance = (xBall - xPin);
		float zDistance = ((distanceZ-22)- zPin);
		float distance = sqrt(pow(xDistance,2) + pow(zDistance,2));
		float sumOfRadius = (radiusBall + 0.06);


		cout << "xBall :" << xBall << "\txPin: " << xPin << endl;
		cout << "zBall :" << zBall << "\tzPin: " << zPin << endl;

		cout << "xDistance: " << xDistance << endl;
		cout << "zDistance: " << zDistance << endl;
		cout << "distance: " << distance << endl;
		cout << "sumOfRadius: " << sumOfRadius << endl;
		return (distance < sumOfRadius);
	}


	float calcFinalBallVelocity(float vBall) {
		cout << "mass Ball:\t" << mBall << "\tmass Pin:\t" << mPin << endl;
		return (float)(((mBall - mPin) / (mBall + mPin)) * vBall); // vf
	}

	float calcFinalPinVelocity(float vBall) {
		cout << "mass Ball:\t" << mBall << "\tmass Pin:\t" << mPin << endl;
		return (float)(((2*mBall) / (mBall + mPin)) * vBall); // vf
	}
	/* Checking collision with the floor */
	int checkFloorCollision() {
		/*if (x_curr && y_curr )
		return 1;
		else
		return 0;*/
	}

	int getScore()
	{
		int score = 0;

		//	for (int i = 0; i < numOfThrows; i++)
		//	score += throws[i];

		return score;
	}

	~BowlingGame()
	{
		//delete throws;
	}
};


/************** Game **************/
BowlingGame bowling;
Place place;

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
};

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.50f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, false);

	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShn);

	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHTING);

	// Loop Sound
	//PlaySound("audio\\starting.wav", NULL, SND_LOOP | SND_ASYNC);

	ball.Load((char*)"models//Orange_Bowling_Ball.3ds");
	//ballImg.LoadBMP((char*)"images/BowlingBall.bmp");

	quadric = gluNewQuadric();
	bowlingImage = LoadTexture((char*)"images/earth.bmp");
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricTexture(quadric, GL_TRUE);

	pin.Load((char*)"models//pin.3ds");
	//pinImg.LoadBMP((char*)"images/pin.bmp");


	table.Load((char*)"models//table.3ds");
	chair.Load((char*)"models//cnek.3ds");

	wall = LoadTexture((char*)"images/wall1.bmp");
	wall2 = LoadTexture((char*)"images/wall.bmp");
	floor1 = LoadTexture((char*)"images/floor1.bmp");
	floor2 = LoadTexture((char*)"images/floor2.bmp");
	painting3 = LoadTexture((char*)"images/painting3.bmp");
	painting5 = LoadTexture((char*)"images/painting5.bmp");
	curtain = LoadTexture((char*)"images/curtain.bmp");
	door = LoadTexture((char*)"images/door.bmp");
	light = LoadTexture((char*)"images/light2.bmp");

	/*********** Setup Game ***********/
	MyCamera = Camera();
	bowling = BowlingGame();
	place = Place();

	// Shadow
	/*M3DVector3f points[3] = { { -9, -2, -8 }, { -9, -2, 8 }, { 10, -2, 8 } };
	m3dGetPlaneEquation(equation, points[0], points[1], points[2]);
	m3dMakePlanarShadowMatrix(shadow, equation, LightPos);
	q = gluNewQuadric();*/

	// music
	mciSendString("open music.mp3 alias My_Music", NULL, 0, 0);
	mciSendString("play My_Music ", NULL, 0, NULL);

	return TRUE;
}

bool moveBall = false;

void Key(bool* keys, float speed)
{
	speed *= 10;

	if (keys['S'])
		MyCamera.RotateX(-0.8 * speed);
	if (keys['W'])
		MyCamera.RotateX(0.8 * speed);
	if (keys['A'])
		MyCamera.RotateY(0.8 * speed);
	if (keys['D'])
		MyCamera.RotateY(-0.8 * speed);
	if (keys['Q'])
		MyCamera.RotateZ(-0.8 * speed);
	if (keys['E'])
		MyCamera.RotateZ(0.8 * speed);

	if (keys[VK_UP])
		MyCamera.MoveForward(0.02 * speed);
	if (keys[VK_DOWN])
		MyCamera.MoveForward(-0.02 * speed);
	if (keys[VK_RIGHT])
		MyCamera.MoveRight(0.02 * speed);
	if (keys[VK_LEFT])
		MyCamera.MoveRight(-0.02 * speed);
	if (keys['Z'])
		MyCamera.MoveUpward(0.02 * speed);
	if (keys['X'])
		MyCamera.MoveUpward(-0.02 * speed);

	if (keys['O'])
	{
		moveBall = true;
	}
	if (keys['P'])
	{
		moveBall = false;
	}

	// music
	if (keys['U']){
		mciSendString("pause My_Music", NULL, 0, NULL);
	}
	if (keys['Y']){
		mciSendString("resume My_Music", NULL, 0, NULL);
	}

}


void DrawGLScene(GLvoid)
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	gluLookAt(0, 0, 0, 0, 0, -20, 0, 1, 0);
	//gluPerspective(5, 1.333, 0.008, 1000.0);

	/*********** Camera ***********/
	MyCamera.Render();

	/*********** Keyboard Control ***********/
	Key(keys, 2);

	/*********** Drawing  ***********/

	// Pins

	bowling.pins(15, 0, -40, zMPin);

	/*bowling.pins(19, 0, -40);

	bowling.pins(17, 0, -40);*/


	// Box (Place)
	place.box(15, 0, -40);

	// Bowling Path
	place.stream(15 - 0.23, 0, -40);
	place.stream(15 + 1.04, 0, -40);
	place.floor(15, 0, -40);

	place.stream(17 - 0.23, 0, -40);
	place.stream(17 + 1.04, 0, -40);
	place.floor(17, 0, -40);

	place.stream(19 - 0.23, 0, -40);
	place.stream(19 + 1.04, 0, -40);
	place.floor(19, 0, -40);

	place.drawPainting(15 - 15, 0 - 0.1, -40 - 0.1);

	glDisable(GL_TEXTURE_2D);


	// Bowling Ball
	glPushMatrix();

	distanceZ = -bowling.getxBall() + bowling.getzBall();

	glTranslated(0, bowling.getHeight(), -bowling.getxBall() + bowling.getzBall());
	bowling.drawBall();

	glPopMatrix();

	//rolAngle--;

	if (moveBall)
		bowling.throwingBall();



	// Tables & chairs
	place.drawTableAndChair(10, 0, -30);
	place.drawTableAndChair(25, 0, -30);
	place.drawTableAndChair(30, 0, -25);


	// shadow
	/*glDisable(GL_LIGHTING);
	glPushMatrix();
	glMultMatrixf((GLfloat*)shadow);
	glTranslated(0, 4.5, 0);
	glColor3d(1, 0, 0);
	gluSphere(q, 1, 32, 32);
	glPopMatrix();*/

	glFlush();
	//DO NOT REMOVE THIS
	SwapBuffers(hDC);
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	static PAINTSTRUCT ps;

	switch (uMsg)									// Check For Windows Messages
	{
	case WM_PAINT:
		DrawGLScene();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		DrawGLScene();
		return 0;

	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
													if (!HIWORD(wParam))					// Check Minimization State
													{
														active = TRUE;						// Program Is Active
													}
													else
													{
														active = FALSE;						// Program Is No Longer Active
													}

													return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
													switch (wParam)							// Check System Calls
													{
													case SC_SCREENSAVE:					// Screensaver Trying To Start?
													case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
														return 0;							// Prevent From Happening
													}
													break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
													PostQuitMessage(0);						// Send A Quit Message
													return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
													keys[wParam] = TRUE;					// If So, Mark It As TRUE
													return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
													keys[wParam] = FALSE;					// If So, Mark It As FALSE
													return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
													ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
													return 0;								// Jump Back
	}

	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	//(LPCWSTR)
	if (!CreateGLWindow((char*)"NeHe Template", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}


	//Set drawing timer to 20 frame per second
	UINT timer = SetTimer(hWnd, 0, 50, (TIMERPROC)NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}



/**************************************** Console Input ****************************************/

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	cout << "\nEnter the initial speed: ";
	cin >> initialSpeedMain;

	cout << "\nEnter the throwing angle: ";
	cin >> throwAngleMain;

	throwAngleMain /= (180 / PI);
	cout << "rad = " << throwAngleMain;
	cout << "\nEnter the ball mass: ";
	cin >> ballMassMain;

	cout << "\nEnter the radius of the ball: ";
	cin >> radiusBALLMain;

	cout << "\nEnter the coefficient of friction: ";
	cin >> frictionMain;

	cout << "\nEnter the reflection factor: ";
	cin >> reflectionMain;
	
	/*initialSpeedMain = 1;
	throwAngleMain = 0;
	ballMassMain = 10;
	radiusBALLMain = 0.08f;
	frictionMain = 0.5f;
	reflectionMain = 0.5f;*/
	return WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

