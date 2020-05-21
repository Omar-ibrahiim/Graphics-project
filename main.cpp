#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "imageloader.h"
#include "glm.h"

static int shoulder1 = 0,shoulder2 = 0, elbow = 0, fingerBase = 0, fingerUp = 0,fingerUp2=0,leg1 =-90.0,knee1 = 0.0,anglarm1 = 0.0,anglarm2 = -180 , leg2 = -90.0 , knee2 = 0.0,anglleg1=0.0,anglleg2=0.0;

//static int shoulder = 0, shoulder2 = 0, elbow = 0, fingerBase = 0, fingerUp = 0, rhip = 0, rhip2 = 0, rknee = 0, lknee = 0, lhip = 0, lhip2 = 0 ;
double eye[] = { 0, 0, -20 };
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
		glmScale(pmodel, .15);
		glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}

GLuint startList;
void init(void)
{
    Image* image = loadBMP("floor.bmp");
    _textureId = loadTexture(image);
    delete image;
    glMatrixMode(GL_PROJECTION);
	gluPerspective(65.0, (GLfloat)1024 / (GLfloat)869, 1.0, 60.0);
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
		pmodel = glmReadOBJ("rose+vase.obj");

		if (!pmodel) exit(0);
		glmUnitize(pmodel);
		glmFacetNormals(pmodel);
		glmVertexNormals(pmodel, 90.0);
		glmScale(pmodel, .15);
	}
	glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT );
   	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
  

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
    	glTranslatef(0.0, 0.0, 0.0);
    	// glScalef(.25, .25, .25);
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
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
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
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
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
        if (elbow < 160)
      elbow = (elbow + 5) % (360);
      glutPostRedisplay();
      break;
   case 'E':
        if (elbow > 0)
      elbow = (elbow - 5) % (360);
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("body");
	init();
	glutMouseFunc(mouse);
   glutMotionFunc(motion);
	glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
