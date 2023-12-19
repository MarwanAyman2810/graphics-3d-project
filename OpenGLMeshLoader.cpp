#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <cmath>
#include <iostream>
#include <glut.h>

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

bool isFirstPersonView = true;
bool isFirstENV = false;
bool env1Win = false;
bool env2Win = false;

int env1Score = 0;
int env2Score = 0;
int healthENV1 = 5;
int healthENV2 = 5;
bool flower1 = false;
bool flower2 = false;
bool flower3 = false;

bool burger1 = false;
bool burger2 = false;
bool burger3 = false;
bool burger4 = false;
bool burger5 = false;
bool burger6 = false;
bool trash1 = false;
bool trash2 = false;
bool trash3 = false;
bool trash4 = false;
bool trash5 = false;
bool bottle1 = false;
bool bottle2 = false;
bool goalEnv2 = false;

float playerSpeed = 0.5f; // Movement speed

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};
float sunPositionX = -10.0f;
Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);
Vector playerPosition(0.0, 0.0, 0.0); // Change as per your scene
Vector playerRotation(0.0, 0.0, 0.0);
int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS modelMan;
Model_3DS modelFlag;
Model_3DS modelFlower;
Model_3DS modelSpike;
Model_3DS modelBurger;
Model_3DS modelMedicine;
Model_3DS modelChair;
Model_3DS modelDesk;
Model_3DS modelBottle;
Model_3DS modelTrash;
int startTime = 0;
int gameDuration = 60; // 60 seconds
bool isGameOver = false;

// Textures
GLTexture tex_ground;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//***********//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//***********//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//***********//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//***********//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void startSecondEnvironment() {
	isFirstENV = false;
	startTime = glutGet(GLUT_ELAPSED_TIME) / 1000; // Get current time in seconds
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

bool vibration = false;
void vibrate() {
	// Only allow jumping if the player is not already jumping
	if (!vibration) {
		vibration = true;
		// Add an upward force to the player's position (adjust the value as needed)
		playerPosition.y += 0.5f;
	}
}

void updateVibration() {
	// If the player is currently jumping, update the position
	if (vibration) {
		// Add gravity to simulate a jump trajectory (adjust the value as needed)
		playerPosition.y -= 0.1f;

		// Check if the player has landed (you may need to adjust the landing threshold)
		if (playerPosition.y <= 0.0f) {
			playerPosition.y = 0.0f;  // Ensure the player is at ground level
			vibration = false;  // Reset the jumping state
		}
	}
}

float playerSpinAngle = 0.0f;
bool isSpinning = false;
int spinsCompleted = 0;
int maxSpins = 2;

void updateStarIntensity() {
	// Get the current time
	int currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000; // Convert to seconds


	// Determine the blue color based on the time with looping
	float starIntensity = 0.5 + sin(0.1 * currentTime);

	// Clamp intensity between 0.1 and 1.0
	starIntensity = fmax(0.1f, fmin(1.0f, starIntensity));

	GLfloat lightColor[] = { starIntensity, starIntensity, starIntensity, 1.0f }; // White light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
}

void init() {

	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);



	GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // Directional light from the top
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	updateStarIntensity();
}

float currentTime; // Assume this is updated in your program
void drawStreetLight() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// Draw pole
	glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the pole
	glPushMatrix();
	glTranslatef(2.0f, -0.5f, 2.0f); // Set the position of the street light
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotate the pole to stand upright
	//drawSolidCylinder(0.05f, 1.0f, 50); // Adjust the radius and height as needed
	//GLUquadric* quad = gluNewQuadric();
	//gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
	//gluDeleteQuadric(quad);
	glPopMatrix();

	// Draw light bulb
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the light bulb
	glPushMatrix();
	init();
	glTranslatef(2.0f, 0.5f, 2.0f); // Set the position of the light bulb
	glutSolidSphere(0.2f, 20, 20); // Adjust the radius as needed
	glPopMatrix();
}

void switchCameraView() {
	isFirstPersonView = !isFirstPersonView;
}

void updateCamera() {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
	float firstPersonHeight = 2.0f; // Adjust for eye-level height in first-person
	float thirdPersonDistance = 3.0f; // Distance behind the player in third-person
	float thirdPersonHeight = 6.0f; // Height above the player in third-person

	if (isFirstPersonView) {
		// First-person camera settings
		Eye.x = playerPosition.x;
		Eye.y = playerPosition.y + firstPersonHeight; // Eye-level height
		Eye.z = playerPosition.z;

		At.x = Eye.x + sin(playerRotation.y * M_PI / 180.0);
		At.y = Eye.y;
		At.z = Eye.z - cos(playerRotation.y * M_PI / 180.0);
	}
	else {
		// Third-person camera settings
		Eye.x = playerPosition.x - thirdPersonDistance * sin(playerRotation.y * M_PI / 180.0);
		Eye.y = playerPosition.y + thirdPersonHeight;
		Eye.z = playerPosition.z + thirdPersonDistance * cos(playerRotation.y * M_PI / 180.0);

		At.x = playerPosition.x;
		At.y = playerPosition.y + 1.0f; // Focus on the player's upper body
		At.z = playerPosition.z;
	}

	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

void updateGame() {

	int currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000;
	if (!isFirstENV && !isGameOver && (currentTime - startTime) >= gameDuration && isFirstENV) {
		isGameOver = true;
	}

	// Update the sun's position to make it move from left to right
	sunPositionX += 0.1f;  // Adjust the translation speed as needed

	// Wrap around the sun's position when it goes beyond the screen
	if (sunPositionX > 10.0f) {
		sunPositionX = -10.0f;
	}

	// Check if the player is currently spinning
	if (isSpinning) {
		// Update the rotation angle for spinning
		playerSpinAngle += 5.0f;  // Adjust the spinning speed as needed

		// Check if two spins are completed
		if (playerSpinAngle >= 360.0f * maxSpins) {
			// Reset spinning variables after two spins
			isSpinning = false;
			playerSpinAngle = 0.0f;
			spinsCompleted = 0;
		}
	}
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);



	GLfloat lightIntensity[] = { 1.0f, 1.0f, 0.0, 1.0f };  // Yellow light for the sun
	GLfloat lightPosition[] = { sunPositionX, 5.0, 0.0, 1.0f };  // Set the sun's position as the light source
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}


void drawSun() {
	// Set up lighting properties
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Call the setupLights function to set up the sun as a light source
	setupLights();
	// Draw the sun as a sphere with the specified color
	glColor3f(1.0, 1.0, 0.0);  // Yellow color
	glPushMatrix();
	glTranslatef(sunPositionX, 5.0, 0.0);  // Set the sun's position
	glutSolidSphere(1.0, 20, 20);  // Adjust the sphere radius as needed
	glPopMatrix();
}
void drawEnv1() {
	drawSun();
	// Draw Ground
	RenderGround();


	// Person
	glPushMatrix();
	glTranslatef(10, 0, 7);
	glTranslatef(playerPosition.x, playerPosition.y, playerPosition.z);
	glRotatef(playerRotation.y, 0, 1, 0);
	glRotatef(playerRotation.x, 1, 0, 0);
	glRotatef(playerRotation.z, 0, 0, 1);
	glScalef(0.07, 0.07, 0.07);
	glRotatef(180, 0, 1, 0);
	modelMan.Draw();
	glPopMatrix();


	// Flag
	glPushMatrix();
	glTranslatef(18, 0, -15);
	glScalef(0.07, 0.04, 0.07);
	modelFlag.Draw();
	glPopMatrix();


	// Flower1
	if (!flower1) {
		glPushMatrix();
		glTranslatef(-5, 0, 5);
		glScalef(0.004, 0.004, 0.004);
		modelFlower.Draw();
		glPopMatrix();
	}


	// Flower2
	if (!flower2) {
		glPushMatrix();
		glTranslatef(-11, 0, 10);
		glScalef(0.004, 0.004, 0.004);
		modelFlower.Draw();
		glPopMatrix();
	}

	// Flower3
	if (!flower3) {
		glPushMatrix();
		glTranslatef(4, 0, -5);
		glScalef(0.004, 0.004, 0.004);
		modelFlower.Draw();
		glPopMatrix();
	}


	// spike1
	glPushMatrix();
	glTranslatef(10, -1, 2);
	glScalef(0.09, 0.09, 0.09);
	modelSpike.Draw();
	glPopMatrix();

	// spike2
	glPushMatrix();
	glTranslatef(-10, -1, 2);
	glScalef(0.09, 0.09, 0.09);
	modelSpike.Draw();
	glPopMatrix();

	// spike3
	glPushMatrix();
	glTranslatef(-10, -1, -5);
	glScalef(0.09, 0.09, 0.09);
	modelSpike.Draw();
	glPopMatrix();


	// spike3
	glPushMatrix();
	glTranslatef(8, -1, -7);
	glScalef(0.09, 0.09, 0.09);
	modelSpike.Draw();
	glPopMatrix();



	// med 1
	glPushMatrix();
	glTranslatef(0, 2, -3);
	glRotated(-90, 1, 0, 1);
	glScalef(0.02, 0.02, 0.02);
	modelMedicine.Draw();
	glPopMatrix();



	// med 2
	glPushMatrix();
	glTranslatef(14, 2, 8);
	glRotated(-90, 1, 0, 1);
	glScalef(0.02, 0.02, 0.02);
	modelMedicine.Draw();
	glPopMatrix();

	//sky box
	glPushMatrix();


}
void checkCollisionsflower() {
	//const float tolerance = 0.01f;



	if ((!flower1) && playerPosition.x == -15 && playerPosition.z == -2) {
		flower1 = true;
		std::cout << "Flower1 collected" << std::endl;
		std::cout << "flower1 position {x: " << playerPosition.x << ", y: " << playerPosition.y << ", z: " << playerPosition.z << "}" << std::endl;
		//updateVibration();
	}
	if ((!flower2) && playerPosition.x == -21 && playerPosition.z == 3) {
		flower2 = true;
		std::cout << "Flower2 collected" << std::endl;
		std::cout << "flower2 position{x: " << playerPosition.x << ", y: " << playerPosition.y << ", z: " << playerPosition.z << "}" << std::endl;
		//updateVibration();
	}
	if ((!flower3) && playerPosition.x == -6 && playerPosition.z == -12) {
		flower3 = true;
		std::cout << "Flower3 collected" << std::endl;
		std::cout << "flower3 position{x: " << playerPosition.x << ", y: " << playerPosition.y << ", z: " << playerPosition.z << "}" << std::endl;
		//updateVibration();
	}


}

void drawEnv2() {

	drawSun();
	// Draw Ground
	RenderGround();

	// Person
	glPushMatrix();
	glTranslatef(10, 0, 7);
	glTranslatef(playerPosition.x, playerPosition.y, playerPosition.z);
	glRotatef(playerRotation.x, 1, 0, 0);
	glRotatef(playerRotation.y, 0, 1, 0);
	glRotatef(playerRotation.z, 0, 0, 1);

	glScalef(0.07, 0.07, 0.07);
	glRotatef(180, 0, 1, 0);

	modelMan.Draw();

	glPopMatrix();

	//burger1
	if (!burger1) {
		glPushMatrix();
		modelBurger.Draw();
		glPopMatrix();
	}


	//burger 2
	if (!burger2) {
		glPushMatrix();
		glTranslatef(10, 0, -5);
		modelBurger.Draw();
		glPopMatrix();
	}

	if (!burger3) {	//burger 3
		glPushMatrix();
		glTranslatef(-5, 0, -5);
		modelBurger.Draw();
		glPopMatrix();
	}


	//burger 4
	if (!burger4) {
		glPushMatrix();
		glTranslatef(10, 0, 10);
		modelBurger.Draw();
		glPopMatrix();
	}

	if (!burger5) {
		//burger 5
		glPushMatrix();
		glTranslatef(-13, 0, -7);
		modelBurger.Draw();
		glPopMatrix();
	}


	if (!burger6) {
		//burger 6
		glPushMatrix();
		glTranslatef(-5, 0, -15);
		modelBurger.Draw();
		glPopMatrix();
	}



	//chair
	glPushMatrix();
	glTranslatef(-10, 0, 15);
	glRotatef(90, 0, 1, 0);
	glScalef(0.0016, 0.0016, 0.0016);
	modelChair.Draw();
	glPopMatrix();


	//bottle 1
	glPushMatrix();
	glTranslatef(0, 2, 10);
	glRotatef(45, 1, 0, 0);
	glScalef(0.09, 0.09, 0.09);
	modelBottle.Draw();
	glPopMatrix();


	//bottle 1
	glPushMatrix();
	glTranslatef(13, 2, -10);
	glRotatef(45, 1, 0, 0);
	glScalef(0.09, 0.09, 0.09);
	modelBottle.Draw();
	glPopMatrix();

	//trash 1
	glPushMatrix();
	//glTranslatef(13, 2, -10); 
	//glRotatef(45, 1, 0, 0);
	glScalef(2, 2, 2);
	modelTrash.Draw();
	glPopMatrix();


	//trash 2
	glPushMatrix();
	glTranslatef(13, 0, -7);
	//glRotatef(45, 1, 0, 0);
	glScalef(2, 2, 2);
	modelTrash.Draw();
	glPopMatrix();

	//trash 3
	glPushMatrix();
	glTranslatef(-7, 0, -7);
	//glRotatef(45, 1, 0, 0);
	glScalef(2, 2, 2);
	modelTrash.Draw();
	glPopMatrix();

	//trash 4
	glPushMatrix();
	glTranslatef(9, 0, 11);
	//glRotatef(45, 1, 0, 0);
	glScalef(2, 2, 2);
	modelTrash.Draw();
	glPopMatrix();


	//trash 5
	glPushMatrix();
	glTranslatef(-11, 0, 9);
	//glRotatef(45, 1, 0, 0);
	glScalef(2, 2, 2);
	modelTrash.Draw();
	glPopMatrix();
}


void checkCollisions() {
	const float tolerance = 0.01f;

	if (!isFirstENV) {

		if (playerPosition.x >= 2 && playerPosition.x <= 3 && playerPosition.z >= 1.5 && playerPosition.x <= 4) {
			burger4 = true;
			env2Score++;
			updateVibration();
		}

		if (playerPosition.x == 2.5 && playerPosition.z == -11) {
			burger2 = true;
			env2Score++;
			updateVibration();

		}

		if (playerPosition.x == -7.5 && playerPosition.z == -6.5) {
			burger1 = true;
			env2Score++;
			updateVibration();

		}

		if (playerPosition.x == -12.5 && playerPosition.z == -11.5) {
			burger3 = true;
			env2Score++;
			updateVibration();

		}

		if (playerPosition.x == -20.5 && playerPosition.z == -13.5) {
			burger5 = true;
			env2Score++;
			updateVibration();

			//std::cout << "The position now {herre: " << env2Score << "}" << std::endl;
		}

		if (playerPosition.x == -12.5 && playerPosition.z == -22) {
			burger6 = true;
			env2Score++;
			updateVibration();

			//Fstd::cout << "The position now {herre222: " << env2Score << "}" << std::endl;
		}
		if (playerPosition.x == -19.5 && playerPosition.z == 7.5) {
			env1Win = true;

			//Fstd::cout << "The position now {herre222: " << env2Score << "}" << std::endl;
		}
		/*
		if (playerPosition.x == -12.5 && playerPosition.z == -22) {
			trash1 = true;
			env2Score++;
			std::cout << "The position now {herre222: " << env2Score << "}" << std::endl;
		}
		if (playerPosition.x == -12.5 && playerPosition.z == -22) {
			trash2 = true;
			env2Score++;
			std::cout << "The position now {herre222: " << env2Score << "}" << std::endl;
		}
		if (playerPosition.x == -12.5 && playerPosition.z == -22) {
			trash3 = true;
			env2Score++;
			std::cout << "The position now {herre222: " << env2Score << "}" << std::endl;
		}
		if (playerPosition.x == -12.5 && playerPosition.z == -22) {
			trash4 = true;
			env2Score++;
			std::cout << "The position now {herre222: " << env2Score << "}" << std::endl;
		}
		if (playerPosition.x == -12.5 && playerPosition.z == -22) {
			trash5 = true;
			env2Score++;
			std::cout << "The position now {herre222: " << env2Score << "}" << std::endl;
		}*/
	}



}

void renderBitmapString(float x, float y, void* font, const char* string) {
	const char* c;
	glColor3f(1.0, 1.0, 1.0);  // Set text color to white
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}
void drawBitmapText(const char* text, float x, float y, float z) {
	glRasterPos3f(x, y, z);

	for (const char* c = text; *c != '\0'; ++c) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
}
void drawGameOverScreen() {
	// Set up a red background
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	drawBitmapText("Game Over", -0.5, 0.0, 0.0);

	glutSwapBuffers();
}
void print(int x, int y, int z, char* string)
{
	//set the position of the text in the window using the x, y and z coordinates
	glRasterPos3f(x, y, z);
	//get the length of the string to display
	int len = (int)strlen(string);

	//loop to display character by character
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
};
void updateAndDisplayScoreAndTime() {
	int currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000;
	int remainingTime = gameDuration - (currentTime - startTime);

	if (remainingTime < 0) {
		remainingTime = 0;
	}

	// Prepare strings
	char scoreString[50];
	sprintf(scoreString, "Score: %d", env2Score);

	char timeString[50];
	sprintf(timeString, "Time left: %d", remainingTime);

	// Display strings
	// Adjust the x, y, z positions as needed
	print(10, HEIGHT - 30, 0, scoreString);
	print(10, HEIGHT - 60, 0, timeString);
}




//=======================================================================
// Keyboard Function
//=======================================================================

//=======================================================================
// Motion Function
//=======================================================================
//void myMotion(int x, int y)
//{
//	y = HEIGHT - y;
//
//	if (cameraZoom - y > 0)
//	{
//		Eye.x += -0.1;
//		Eye.z += -0.1;
//	}
//	else
//	{
//		Eye.x += 0.1;
//		Eye.z += 0.1;
//	}
//
//	cameraZoom = y;
//
//	glLoadIdentity();	//Clear Model_View Matrix
//
//	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters
//
//	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//
//	glutPostRedisplay();	//Re-draw scene 
//}




//=======================================================================
// Mouse Function
//=======================================================================

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}


//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	modelMan.Load("Models/man/Man old N290313.3DS");
	modelFlag.Load("Models/flag/Flag.3ds");
	modelFlower.Load("Models/flowers/Vase plant N021213.3DS");
	modelSpike.Load("Models/spike/Spike weapon N161214.3DS");
	modelBurger.Load("Models/cheeseburger/cheeseburger.3DS");
	modelMedicine.Load("Models/med/med.3DS");
	modelChair.Load("Models/chair/chair.3DS");
	modelDesk.Load("Models/desk/desk.3DS");
	modelBottle.Load("Models/bottle/bottle.3DS");
	modelTrash.Load("Models/trash/trash.3DS");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}



void Keyboard(unsigned char key, int x, int y) {
	float speed = 1.0f;

	switch (key) {
	case 'w':
		playerRotation.y = 180.0;
		if (playerPosition.z >= -23) {
			playerPosition.z -= speed;
		}
		break;
	case 's':
		playerRotation.y = 0.0;
		if (playerPosition.z <= 11) {
			playerPosition.z += speed;
		}
		break;
	case 'a':
		playerRotation.y = -90.0;
		if (playerPosition.x >= -25) {
			playerPosition.x -= speed;
		}
		break;
	case 'd':
		playerRotation.y = 90.0;
		if (playerPosition.x < 9) {
			playerPosition.x += speed;
		}
		break;
	case 27: // ASCII value for the escape key
		exit(EXIT_SUCCESS);
	}

	// Redraw the screen or update the display here
	glutPostRedisplay();

	std::cout << "The position now {x: " << playerPosition.x << ", y: " << playerPosition.y << ", z: " << playerPosition.z << "}" << std::endl;
}
//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);



	/*updateAndDisplayScoreAndTime();
	updateGame();*/
	checkCollisions();

	drawEnv1();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();
	checkCollisionsflower();


	glutSwapBuffers();
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(Keyboard);

	//	glutMotionFunc(myMotion);

	//glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}