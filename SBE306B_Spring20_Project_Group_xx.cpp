#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "imageloader.h"
#include "glm.h"

static int shoulder1 = -90,shoulder2 = -90, elbow1 = -350, elbow2=10, fingerBase = 0, fingerUp = 0,fingerUp2=0,leg1 =-90.0,knee1 = 0.0,anglarm1 =-90,anglarm2 = -90 , leg2 = -90.0 , knee2 = 0.0,anglleg1=0.0,anglleg2=0.0;
//for light
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0 };

GLfloat light_position[] = {0.5,5.0, 0.0, 1.0 };
GLfloat lightPos1[] = {-0.5,-5.0,-2.0, 1.0 };

GLfloat mat_amb_diff[] = {0.643, 0.753, 0.934, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = {100.0 };  

GLfloat teapotl_diff[] = { 0.0,0.0, 1.0, 1.0 };
GLfloat teapotl_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat teapotl_shininess[] = {10.0 };  

GLfloat teapotm_diff[] = { 1.0, 0, 0.0, 1.0 };
GLfloat teapotm_specular[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat teapotm_shininess[] = {1.0 };  

GLfloat teapotr_diff[] = { 1.0, .0, 0.0, 1.0 };
GLfloat teapotr_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat teapotr_shininess[] = {1000.0 };  

GLfloat cube_diff[] = {1.0,0.0, 0.0, 1.0 };
GLfloat cube_specular[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat cube_shininess[] = {10.0 }; 
double eye[] = { 0, 0, 20 };
double center[] = { 0, 0, 1 };
double up[] = { 0, 1, 0 };
double direction[] = {0,0,0};
static double out[] = {0,0,0};
double moving, startx ;
double left;
double right;
double up1;
double down;

double angle ;   /* in degrees */
double angle2 ;   /* in degrees */

//GLMmodel* pmodel;
GLMmodel* pmodel ;
GLMmodel* pmodel1;


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
      GLuint textureId;
      glGenTextures(1, &textureId); //Make room for our texture
      glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
      //Map the image to the texture
      glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                               0,                            //0 for now
                               GL_RGB,                       //Format OpenGL uses for image
                               image->width, image->height,  //Width and height
                               0,                            //The border of the image
                               GL_RGB, //GL_RGB, because pixels are stored in RGB format
                               GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                                 //as unsigned numbers
                               image->pixels);               //The actual pixel data
      return textureId; //Returns the id of the texture
}

GLuint _textureId; //The id of the texture
GLuint _textureId1; //The id of the texture


void drawmodel(void)
{
		glmUnitize(pmodel);
		glmFacetNormals(pmodel);
		glmVertexNormals(pmodel, 90.0);
		glmScale(pmodel, 2);
		glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}

void init(void)
{
  Image* image = loadBMP("grass-background.bmp");
    _textureId = loadTexture(image);
    delete image;
    
// Turn on the power
        glEnable(GL_LIGHTING);
        // Flip light switch
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        // assign light parameters
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties         
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	GLfloat lightColor1[] = {1.0f, 1.0f,  1.0f, 1.0f };
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
        glEnable(GL_NORMALIZE);
        //Enable smooth shading
        glShadeModel(GL_SMOOTH);
        // Enable Depth buffer
        glEnable(GL_DEPTH_TEST);



  glMatrixMode(GL_PROJECTION);
	gluPerspective(65.0, (GLfloat)1024 / (GLfloat)869, 1.0, 60.0);

}
static int run=0;
static int jump=0;
int poses[16][15]={
   {-90, -90,-270,90, 0, 0, 0, -110, 0, -90,-90,-80, -20,0 , 0 },
  {-85, -95,-270,90, 0, 0, 0, -105, 0, -90,-90,-85, -10, 0, 0 },
   {-80, -100,-270,90, 0, 0, 0, -100, 0, -90,-90,-90 ,0, 0, 0 },
   {-75, -105,-270,90, 0, 0, 0, -95, 0, -90,-90,-95, 0, 0, 0 },
   {-70, -110,-270,90, 0, 0, 0, -90, 0, -90,-90,-100, 0,0 , 0 },
   {-75, -105,-270,90, 0, 0, 0, -85, 0, -90,-90,-105, 0,0 , 0 },
   {-80, -100,-270,90, 0, 0, 0, -80, -10, -90,-90,-110, 0,0 , 0 },
   {-85, -95,-270,90, 0, 0, 0, -75, -10, -90,-90,-115, 0,0 , 0 },
   

   {-90, -90,-270,90, 0, 0, 0, -80, -20, -90,-90,-110,0,0 , 0 },
   {-95, -85,-270,90, 0, 0, 0, -85, -10, -90,-90,-105, 0,0 , 0 },
   {-100, -80,-270,90, 0, 0, 0, -90, 0, -90,-90,-100, 0,0 , 0 },
   {-105, -75,-270,90, 0, 0, 0, -95, 0, -90,-90,-95, 0,0 , 0 },
   {-110, -70,-270,90, 0, 0, 0, -100, 0, -90,-90,-90, 0,0 , 0 },
   {-105, -75,-270,90, 0, 0, 0, -105, 0, -90,-90,-85, 0,0 , 0 },
   {-100, -80,-270,90, 0, 0, 0, -110, 0, -90,-90,-80, -10,0 , 0 },
  {-95, -85,-270,90, 0, 0, 0, -115, 0, -90,-90,-75, -10,0 , 0 },
};
int poses2[14][15]={
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,0,0},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,5,-5},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,10,-10},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,15,-15},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,20,-20},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,25,-25},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,30,-30},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,35,-35},

  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,30,-30},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,25,-25},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,20,-20},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,15,-15},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,10,-10},
  {-135,-135,-270,90,0,0,0,-90,0,-160,-20 ,-90,0,5,-5},
};
void setposes(int frameNum)
{
  shoulder1 = poses[frameNum][0];
  shoulder2 = poses[frameNum][1];
  elbow1 = poses[frameNum][2];
  elbow2 =poses[frameNum][3];
  fingerBase = poses[frameNum][4];
  fingerUp = poses[frameNum][5];
  fingerUp2=poses[frameNum][6];
  leg1 =poses[frameNum][7];
  knee1 = poses[frameNum][8];
  anglarm1 = poses[frameNum][9];
  anglarm2 = poses[frameNum][10];
  leg2 =  poses[frameNum][11];
  knee2 = poses[frameNum][12];
  anglleg1=poses[frameNum][13];
  anglleg2=poses[frameNum][14];
}
void setposes2(int Num)
{
  shoulder1 = poses2[Num][0];
  shoulder2 = poses2[Num][1];
  elbow1 = poses2[Num][2];
  elbow2 =poses2[Num][3];
  fingerBase = poses2[Num][4];
  fingerUp = poses2[Num][5];
  fingerUp2=poses2[Num][6];
  leg1 =poses2[Num][7];
  knee1 = poses2[Num][8];
  anglarm1 = poses2[Num][9];
  anglarm2 = poses2[Num][10];
  leg2 =  poses2[Num][11];
  knee2 = poses2[Num][12];
  anglleg1=poses2[Num][13];
  anglleg2=poses2[Num][14];
}


int f=0;
void Timer(int value){
    if (run==1){
    
    f = f % 5;
    setposes(f);   
    f++;     
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 0);
    }
    glutPostRedisplay();

}
int k=0;
void Timer2(int value){
    if (jump==1){
    
    k = k % 5;
    setposes2(k);   
    k++;     
    glutPostRedisplay();
    glutTimerFunc(100, Timer2, 0);
    }
    glutPostRedisplay();

}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = (a[1] * b[2] - a[2] * b[1])*0.01;
	c[1] = (a[2] * b[0] - a[0] * b[2])*0.01;
	c[2] = (a[0] * b[1] - a[1] * b[0])*0.01;
}

void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];

	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta))*(a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta))*(a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta))*(a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta)*p[0];
	temp[1] += cos(theta)*p[1];
	temp[2] += cos(theta)*p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];

}

void Left()
{
    rotatePoint(up,left,eye);
    rotatePoint(up,angle2,eye);
}

void Right()
{
    rotatePoint(up,right,eye);
    rotatePoint(up,angle2,eye);
}

void Up()
{
	crossProduct(eye,up,out);
	normalize(out);
	rotatePoint(out, up1,eye);
}

void Down()
{
	crossProduct(eye,up,out);
	normalize(out);
	rotatePoint(out, down,eye);
}

void moveForward()
{
    double speed;
    direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
    eye[0]    += direction[0] *0.01;
	eye[1]    += direction[1] *0.01;
	eye[2]    += direction[2] * 0.01;

	center[0] += direction[0] * 0.01;
	center[1] += direction[1] * 0.01;
	center[2] += direction[2] * 0.01;
}

void moveBack()
{
    direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
    eye[0]    -= direction[0] * 0.01;
	eye[1]    -= direction[1] * 0.01;
	eye[2]    -= direction[2] * 0.01;

	center[0] -= direction[0] * 0.001;
	center[1] -= direction[1] * 0.001;
	center[2] -= direction[2] * 0.01;
}

void drawmodel1(void)
{
	if (!pmodel) {
		pmodel = glmReadOBJ("re-optimized_sphere.obj");

		if (!pmodel) exit(0);
		glmUnitize(pmodel);
		glmFacetNormals(pmodel);
		glmVertexNormals(pmodel, 90.0);
		glmScale(pmodel, 1.0);
	}
	glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //for light
   	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        glPushMatrix();
glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glPopMatrix();
        //materials properties
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

glPushMatrix();

 //  glRotatef(angle2, 1.0, 0.0, 0.0);
   //glRotatef(angle, 0.0, 1.0, 0.0);
    glTranslatef (0.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (3.0, 3.0, 1.0);
   glutWireCube (1.0);
   glPopMatrix();

   //floor
glPushMatrix();
glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef (-10.0, -6.0, -10.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);

   glBegin(GL_QUADS);
    // Bottom left
    glTexCoord2f(0.0f, 10.0f);
    glVertex2i(0.0f, 50.0f);

    // Top left
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0.0f, 0.0f);

    // Top right
    glTexCoord2f(10.0f, 0.0f);
    glVertex2i(50.0f, 0.0f);

    // Bottom right
    glTexCoord2f(10.0f, 10.0f);
    glVertex2i(50.0f, 50.0f);
glEnd();
glDisable(GL_TEXTURE_2D);
glPopMatrix();
//model
glPushMatrix();
    	glTranslatef(3.0, 1.0, 3.0);
    	drawmodel1();
	glPopMatrix();
   //head
   glPushMatrix();
   glTranslatef (0.0, 2.5, 0.0);
   glPushMatrix();
   glutWireSphere (0.7,16.0,6.0) ;
   glPopMatrix();
   glPopMatrix();
   //arm1
    glPushMatrix();
   glTranslatef (2.0, 1.3, 0.0);
    glRotatef((GLfloat)anglarm1, 0.0, 1.0, 0.0);
   glRotatef ((GLfloat) shoulder1, 0.0, 0.0, 1.0);
   glTranslatef (0.7, 0.0, 0.0);
   glPushMatrix();
   glScalef (1.5, 0.6, 0.6);
   glutWireCube (1.0);
   glPopMatrix();
   glTranslatef (0.8, 0.0, 0.0);
   glRotatef ((GLfloat) elbow1, 0.0, 0.0, 1.0);
   glTranslatef (0.7, 0.0, 0.0);
   glPushMatrix();
   glScalef (1.5, 0.6, 0.6);
   glutWireCube (1.0);
   glPopMatrix();

   // finger 1
   glPushMatrix();
   glTranslatef(0.7, 0.25, 0.25);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();


   // finger 2
   glPushMatrix();
   glTranslatef(0.7, -0.25, 0.25);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();


   // finger 3
   glPushMatrix();
   glTranslatef(0.7, -0.25, 0.10);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();

   // finger 4
   glPushMatrix();
   glTranslatef(0.7, -0.25,-0.05);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();

   // finger 5
   glPushMatrix();
   glTranslatef(0.7, -0.25, -0.20);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 3.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();


   glPopMatrix();

  //arm2
   glPushMatrix();
   glTranslatef (-2.0, 1.3, 0.0);
   glRotatef((GLfloat)anglarm2, 0.0, 1.0, 0.0);
   glRotatef ((GLfloat) shoulder2, 0.0, 0.0, 1.0);
   glTranslatef (0.7, 0.0, 0.0);
   glPushMatrix();
   glScalef (1.5, 0.6, 0.6);
   glutWireCube (1.0);
   glPopMatrix();
   glTranslatef (0.8, 0.0, 0.0);
   glRotatef ((GLfloat) elbow2, 0.0, 0.0, 1.0);
   glTranslatef (0.7, 0.0, 0.0);
   glPushMatrix();
   glScalef (1.5, 0.6, 0.6);
   glutWireCube (1.0);
   glPopMatrix();
   // finger 1
   glPushMatrix();
   glTranslatef(0.7, 0.25, 0.25);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();


   // finger 2
   glPushMatrix();
   glTranslatef(0.7, -0.25, 0.25);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();


   // finger 3
   glPushMatrix();
   glTranslatef(0.7, -0.25, 0.10);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();

   // finger 4
   glPushMatrix();
   glTranslatef(0.7, -0.25,-0.05);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();

   // finger 5
   glPushMatrix();
   glTranslatef(0.7, -0.25, -0.20);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 3.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glTranslatef(0.05, 0.0, 0.0);
   glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
   glTranslatef(0.15, 0.0, 0.0);
   glPushMatrix();
   glScalef(0.2, 0.1, 0.1);
   glutWireCube(1);
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();

   //leg1
   glPushMatrix();
   glTranslatef (1.0, -1.5, 0.0);
   glRotatef(-90.0, 0.0, 1.0, 0.0);
   glRotatef ((GLfloat) anglleg1, 1.0, 0.0, 0.0);
   glRotatef ((GLfloat) leg1, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.6, 1.0);
   glutWireCube (1.0);
   glPopMatrix();
   glTranslatef (1.0, 0.0, 0.0);
   glRotatef ((GLfloat) knee1, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.6, 1.0);
   glutWireCube (1.0);
   glPopMatrix();
//foot1
glPushMatrix();
    glTranslatef (1.3, 0.0, 0.0);
    glScalef (0.6, 2.0, 1.0);
    glutSolidCube (1.0);
glPopMatrix();
glPopMatrix();

//leg2
   glPushMatrix();
   glTranslatef (-1.0, -1.5, 0.0);
   glRotatef(-90.0, 0.0, 1.0, 0.0);
   glRotatef ((GLfloat) anglleg2, 1.0, 0.0, 0.0);
   glRotatef ((GLfloat) leg2, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.6, 1.0);
   glutWireCube (1.0);
   glPopMatrix();
   glTranslatef (1.0, 0.0, 0.0);
   glRotatef ((GLfloat) knee2, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.6, 1.0);
   glutWireCube (1.0);
   glPopMatrix();
   //foot2
glPushMatrix();
    glTranslatef (1.3, 0.0, 0.0);
    glScalef (0.6, 2.0, 1.0);
    glutSolidCube (1.0);
glPopMatrix();
   glPopMatrix();


    glPopMatrix();
	glutSwapBuffers();
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
    left += 0.01;
	Left();
	break;
	case GLUT_KEY_RIGHT:
	right -=0.01;
    Right();
    break;
	case GLUT_KEY_UP:
	up1 += 0.01;
    Up();
    break;
	case GLUT_KEY_DOWN:
    down -= 0.01;
    Down();
    break;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		moveForward();
		glutPostRedisplay();
		break;
	case 'b':
		moveBack();
		glutPostRedisplay();
		break;
     case 'x':
          if (anglarm1<0)
      anglarm1 = (anglarm1 + 5) % (360);
      glutPostRedisplay();
      break;
   case 'X':
       if(anglarm1>-160)
      anglarm1 = (anglarm1 - 5) % (360);
      glutPostRedisplay();
      break;
    case 'y':
        if (anglarm2>-180)
            anglarm2 =(anglarm2- 5) % (360);
            glutPostRedisplay();
      break;
   case 'Y':
       if(anglarm2<-20)
    anglarm2 = (anglarm2 + 5) % (360);
      glutPostRedisplay();
      break;
case 's':
    if(shoulder1 <90 )
      shoulder1 = (shoulder1 + 5) % (360);
      glutPostRedisplay();
      break;
   case 'S':
    if(shoulder1 >-180)
      shoulder1 = (shoulder1 - 5) % (360);
      glutPostRedisplay();
      break;
  case 't':
      if(shoulder2 <90 )
      shoulder2 = (shoulder2 + 5) % (360);
      glutPostRedisplay();
      break;
   case 'T':
    if(shoulder2 >-180)
      shoulder2 = (shoulder2 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'e':
        if (elbow1 < -195)
      elbow1 = (elbow1 + 5) % (360);
      glutPostRedisplay();
      break;
   case 'E':
        if (elbow1 > -350)
      elbow1 = (elbow1 - 5) % (360);
      glutPostRedisplay();
      break;
    case 'W':
        if (elbow2 > 10)
      elbow2 = (elbow2 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'w':
        if (elbow2 < 165)
      elbow2 = (elbow2 + 5) % (360);
      glutPostRedisplay();
      break;    
   case 'N':
       if (fingerBase <0)
      fingerBase = (fingerBase + 5) % (360);
      glutPostRedisplay();
      break;
   case 'n':
        if (fingerBase >-90)
      fingerBase = (fingerBase - 5) % (360);
      glutPostRedisplay();
      break;
    case 'g':
        if (fingerUp <0)
      fingerUp = (fingerUp + 5) % (360);
        glutPostRedisplay();
      break;
   case 'G':
       if (fingerUp >-90)
      fingerUp = (fingerUp - 5) % (360);
      glutPostRedisplay();
      break;
    case 'q':
        if (fingerUp2 <90)
      fingerUp2 = (fingerUp2 + 5) % (360);
        glutPostRedisplay();
      break;
   case 'Q':
       if (fingerUp2 >0)
      fingerUp2 = (fingerUp2 - 5) % (360);
      glutPostRedisplay();
      break;
  case 'l':
      if(leg1 >-170)
      leg1 = (leg1 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'L':
    if(leg1 <0.0)
      leg1 = (leg1 + 5) % (360);
      glutPostRedisplay();
      break;
  case 'k':
      if(knee1 >-90)
      knee1 = (knee1 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'K':
       if(knee1 <0)
      knee1 = (knee1 + 5) % (360);
      glutPostRedisplay();
      break;
      case 'p':
        if(leg2 >-170)
      leg2 = (leg2 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'P':
       if(leg2 <0.0)
      leg2 = (leg2 + 5) % (360);
      glutPostRedisplay();
      break;
  case 'o':
      if(knee2 >-90)
      knee2 = (knee2 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'O':
       if(knee2 <0)
      knee2 = (knee2 + 5) % (360);
      glutPostRedisplay();
      break;
    case 'v':
      if(anglleg1 >-60)
      anglleg1 = (anglleg1 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'V':
       if(anglleg1 <90)
      anglleg1 = (anglleg1 + 5) % (360);
      glutPostRedisplay();
      break;
      case 'm':
      if(anglleg2 >-90)
      anglleg2 = (anglleg2 - 5) % (360);
      glutPostRedisplay();
      break;
   case 'M':
       if(anglleg2 <60)
      anglleg2 = (anglleg2 + 5) % (360);
      glutPostRedisplay();
      break;
  case 'r':
        if (run==0)
        {run=1; }
        else
        {
          run=0;
        }
        
        glutTimerFunc(0,Timer,0);
        glutPostRedisplay();

       break;
  case 'j':
        if (jump==0)
        {jump=1; }
        else
        {
          jump=0;
        }
        
        glutTimerFunc(0,Timer2,0);
        glutPostRedisplay();

       break;

   default:
		break;


	}
}

static void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_LEFT) {
      moving = 1;
      startx = x;
    }
    if (state == GLUT_UP) {
    moving = 0;
    }
  }
}


static void motion(int x, int y)
{
  if (moving) {
    angle = (x - startx)*0.01;
    if (angle2 < angle)
    {
        angle2 = angle;
        startx=x;
        Left();
        angle = 0.0;
        angle2 = 0.0;
    }
    else
    {
        angle2 = angle;
        startx=x;
        Right();
        angle = 0.0;
        angle2 = 0.0;
    }
  }
  glutPostRedisplay();
}
//for light
void main_menu(int value)
{
        if (value == 1)
        {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glutPostRedisplay();
        }
        else if (value == 2)
        {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glutPostRedisplay();
        }
}
////////

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	// for light
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//////
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("body");
	init();
	glutMouseFunc(mouse);
   glutMotionFunc(motion);
    glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
	glutMainLoop();
  // for light
	glutCreateMenu(main_menu);
  glutAddMenuEntry("White Background", 1);
  glutAddMenuEntry("Black Background", 2);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
//

	return 0;
}
