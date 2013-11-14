#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>


float zRotation = 0.0;
float zoom = 1;

float yaw = 0.0f;
float pitch = 0.0f;
float pitchM = 0.0f;
float yawM = 0.0f;
float cameraDistance = 0;

int LeftButtonDown = 0;
int ShiftLeftButtonDown = 0;
int MiddleButtonDown = 0;

int oldX, oldY, newX, newY;

//Variable for displacement of mouse
float moveX = 0, moveY = 0;

//variable to store maximum value of Z coordinate in given dataset
double maxZ = -100.0;

GLenum linePoly = GL_FALSE;
GLenum pointDraw = GL_FALSE;
GLenum backColor = GL_FALSE;
int flyThrough = 1;
GLint windW = 800, windH = 600;

GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 }, green_light[] = { 0.0, 1.0, 0.0, 1.0 }, white_light[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat left_light_position[] = { -20.0, 0.0, 10.0, 1.0 }, right_light_position[] = { 20.0, 0.0, 10.0, 1.0 }, front_light_position[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat white_ambient[] = { 0.9, 0.9, 0.9, 1.0 }, white_diffuse[] = { 0.9, 0.9, 0.9, 1.0 }, white_specular[] = { 0.9, 0.9, 0.9, 1.0 }, white_shininess = 60.0;

unsigned char* sunImg;
unsigned char* earthImg;
unsigned char* moonImg;
unsigned char* jupitorImg;
unsigned char* marsImg;
unsigned char* neptuneImg;
unsigned char* uranusImg;
unsigned char* venusImg;
unsigned char* mercuryImg;
unsigned char* saturnImg;
unsigned char* backImg;

typedef struct vertex {
	GLfloat coord[3];
} Vertex;

typedef struct Normals {
	GLfloat normal[3];
	int count;
} Normals;

typedef struct Triangle {
	int Vertices[3];
} Triangle;

GLint noOfVertices = 0;
Vertex* vertices;
Triangle* polygons;
Normals* normals;
Vertex* origin;

int noOfFaces = 0;
long imageSize;
int imgWidth, imgHeight;
GLuint textureIDEarth, textureIDSun, textureIDMoon, textureIDMars, textureIDJupiter, textureIDNeptune, textureIDVenus, textureIDUranus, textureIDMercury, textureIDBack, textureIDSaturn;

#define PI 3.141592654
#define TWOPI 6.283185308

void SphereMap(float* vertices, float *t, float *s) {

	float radius = 1.0f;

	float x = vertices[0] * radius;
	float y = vertices[1] * radius;
	float z = vertices[2] * radius;

	*t = (atan2(z, x) / (TWOPI)) + 0.5;

	*s = 0.5 + (asin(y) / PI);

}

void calculatePerVertexNormal(int vertex1, int vertex2, int vertex3) {
	/* calculate Vector1 and Vector2 */

	GLfloat *coord1 = vertices[vertex1].coord;
	GLfloat *coord2 = vertices[vertex2].coord;
	GLfloat *coord3 = vertices[vertex3].coord;

	GLfloat va[3], vb[3], vr[3], val;
	va[0] = coord2[0] - coord1[0];
	va[1] = coord2[1] - coord1[1];
	va[2] = coord2[2] - coord1[2];

	vb[0] = coord3[0] - coord1[0];
	vb[1] = coord3[1] - coord1[1];
	vb[2] = coord3[2] - coord1[2];

	/* cross product */
	vr[0] = va[1] * vb[2] - va[2] * vb[1];
	vr[1] = va[2] * vb[0] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - va[1] * vb[0];

	/* normalization factor */
	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	normals[vertex1].normal[0] += vr[0] / val;
	normals[vertex2].normal[0] += vr[0] / val;
	normals[vertex3].normal[0] += vr[0] / val;

	normals[vertex1].normal[1] += vr[1] / val;
	normals[vertex2].normal[1] += vr[1] / val;
	normals[vertex3].normal[1] += vr[1] / val;

	normals[vertex1].normal[2] += vr[2] / val;
	normals[vertex2].normal[2] += vr[2] / val;
	normals[vertex3].normal[2] += vr[2] / val;

	normals[vertex1].count += 1;
	normals[vertex2].count += 1;
	normals[vertex3].count += 1;

}

static void Normalize() {
	int i = 0;
	GLdouble normalizingFactor = sqrt(origin[0].coord[0] * origin[0].coord[0] + origin[0].coord[1] * origin[0].coord[1] + origin[0].coord[2] * origin[0].coord[2]);

	if (normalizingFactor < 0.000001)
		return;

	for (i = 0; i < noOfVertices; i++) {

		vertices[i].coord[0] /= normalizingFactor;
		vertices[i].coord[1] /= normalizingFactor;
		vertices[i].coord[2] /= normalizingFactor;

	}

	origin[0].coord[0] /= normalizingFactor;
	origin[0].coord[1] /= normalizingFactor;
	origin[0].coord[2] /= normalizingFactor;

}

static void loadTriFile(char *fileName) {

	FILE *vertexReader, *coordinateReader;
	int i = 0;
	int coordinateStartPoint;
	int verticesStartPoint;

	if ((vertexReader = fopen(fileName, "r")) == NULL) {
		printf("File not found !");
		return;
	}
	coordinateReader = fopen(fileName, "r");

	fscanf(coordinateReader, "%d\n", &noOfVertices);

	coordinateStartPoint = ftell(coordinateReader);

	i = 0;

	vertices = (Vertex*) malloc(sizeof(Vertex) * noOfVertices);
	origin = (Vertex*) calloc(1, sizeof(Vertex));

	while (i < noOfVertices) {
		fscanf(coordinateReader, "%f %f %f\n", &vertices[i].coord[0], &vertices[i].coord[1], &vertices[i].coord[2]);

		origin[0].coord[0] += vertices[i].coord[0];
		origin[0].coord[1] += vertices[i].coord[1];
		origin[0].coord[2] += vertices[i].coord[2];

		i++;
	}

	origin[0].coord[0] /= noOfVertices;
	origin[0].coord[1] /= noOfVertices;
	origin[0].coord[2] /= noOfVertices;

	Normalize();

	moveX = -1 * origin[0].coord[0];
	moveY = -1 * origin[0].coord[1];

	fscanf(coordinateReader, "%d\n", &noOfFaces);

	polygons = (Triangle*) malloc(sizeof(Triangle) * noOfFaces);
	normals = (Normals*) calloc(noOfVertices, sizeof(Normals));

	verticesStartPoint = ftell(coordinateReader);
	fseek(vertexReader, verticesStartPoint, SEEK_SET);
	fseek(coordinateReader, coordinateStartPoint, SEEK_SET);
	int ff;
	for (i = 0; i < noOfFaces; i++) {
		fscanf(vertexReader, "%d %d %d %d\n", &ff, &polygons[i].Vertices[0], &polygons[i].Vertices[1], &polygons[i].Vertices[2]);
		calculatePerVertexNormal(polygons[i].Vertices[0], polygons[i].Vertices[1], polygons[i].Vertices[2]);
	}

	fclose(vertexReader);
	fclose(coordinateReader);

	for (i = 0; i < noOfVertices; i++) {
		normals[i].normal[0] /= normals[i].count;
		normals[i].normal[1] /= normals[i].count;
		normals[i].normal[2] /= normals[i].count;
	}

}

static void Init(void) {

	loadTriFile("sphere.tri");

	earthImg = SOIL_load_image("EarthImg.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);

	glGenTextures(1, &textureIDEarth);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureIDEarth);
	// Give the image to OpenGL
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, earthImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(earthImg);

	sunImg = SOIL_load_image("sun.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDSun);
	glBindTexture(GL_TEXTURE_2D, textureIDSun);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sunImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(sunImg);

	moonImg = SOIL_load_image("MoonImg.bmp", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDMoon);
	glBindTexture(GL_TEXTURE_2D, textureIDMoon);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, moonImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(moonImg);

	marsImg = SOIL_load_image("MarsImg.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDMars);
	glBindTexture(GL_TEXTURE_2D, textureIDMars);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, marsImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(marsImg);

	jupitorImg = SOIL_load_image("Jupiter.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDJupiter);
	glBindTexture(GL_TEXTURE_2D, textureIDJupiter);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, jupitorImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(jupitorImg);

	saturnImg = SOIL_load_image("SaturnImg.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDSaturn);
	glBindTexture(GL_TEXTURE_2D, textureIDSaturn);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, saturnImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(saturnImg);

	uranusImg = SOIL_load_image("UranusImg.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDUranus);
	glBindTexture(GL_TEXTURE_2D, textureIDUranus);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, uranusImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(uranusImg);

	venusImg = SOIL_load_image("VenusImg.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDVenus);
	glBindTexture(GL_TEXTURE_2D, textureIDVenus);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, venusImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(venusImg);

	mercuryImg = SOIL_load_image("MercuryImg.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDMercury);
	glBindTexture(GL_TEXTURE_2D, textureIDMercury);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mercuryImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(mercuryImg);

	neptuneImg = SOIL_load_image("NeptuneImg.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDNeptune);
	glBindTexture(GL_TEXTURE_2D, textureIDNeptune);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, neptuneImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(neptuneImg);

	backImg = SOIL_load_image("Space6.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureIDBack);
	glBindTexture(GL_TEXTURE_2D, textureIDBack);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, backImg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_SPHERE_MAP); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_SPHERE_MAP);
	SOIL_free_image_data(backImg);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	imageSize = imgWidth * imgHeight;

}

static void Reshape(int width, int height) {
	windW = width;
	windH = height;
	glViewport(0, 0, windW, windH);
	// Set viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float) width / height, 0.001f, 180);

	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void RenderObject(int textureId, int displacement, int normalSign) {

	GLint i;

	int v1, v2, v3;

	GLfloat texX;
	GLfloat texY;

	glBindTexture(GL_TEXTURE_2D, textureId);

	for (i = 0; i < noOfFaces; i++) {

		v1 = polygons[i].Vertices[0];
		v2 = polygons[i].Vertices[1];
		v3 = polygons[i].Vertices[2];

		glBegin(GL_TRIANGLES);

		glNormal3f(normals[v1].normal[0] * normalSign, normals[v1].normal[1] * normalSign, normals[v1].normal[2] * normalSign);
		SphereMap(vertices[v1].coord, &texX, &texY);
		glTexCoord2f(texX, texY);
		glVertex3fv(vertices[v1].coord);

		glNormal3f(normals[v2].normal[0] * normalSign, normals[v2].normal[1] * normalSign, normals[v2].normal[2] * normalSign);
		SphereMap(vertices[v2].coord, &texX, &texY);
		glTexCoord2f(texX, texY);
		glVertex3fv(vertices[v2].coord);

		glNormal3f(normals[v3].normal[0] * normalSign, normals[v3].normal[1] * normalSign, normals[v3].normal[2] * normalSign);
		SphereMap(vertices[v3].coord, &texX, &texY);
		glTexCoord2f(texX, texY);
		glVertex3fv(vertices[v3].coord);

		glEnd();

	}

}

static void Draw(void) {

	double moveZ = -1 * origin[0].coord[2];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (flyThrough == 1)
		gluLookAt(cos(cameraDistance) * 35, sin(cameraDistance) * 10, sin(cameraDistance = cameraDistance + 0.01) * 35, 0, 0, 0, 0, 1, 0);
	else if (flyThrough == 0)
		gluLookAt(cos(cameraDistance) * 35, sin(cameraDistance) * 10, sin(cameraDistance) * 35, 0, 0, 0, 0, 1, 0);
	else
		gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

	glTranslated(moveX, moveY, moveZ);

	glRotatef(yawM, 1, 0, 0);
	glRotatef(pitchM, 0, 1, 0);

	glPushMatrix();
	glScalef(45, 45, 45);
	RenderObject(textureIDBack, 0, -1);
	glPopMatrix();

	glScalef(zoom, zoom, zoom);

	//Render(GL_RENDER);
	glPushMatrix();
	glScalef(2, 2, 2);
	glRotatef(pitch, 0, 1, 0);
	RenderObject(textureIDSun, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch / 0.241, 0, 1, 0);
	glPushMatrix();
	glTranslated(moveX + 5, moveY, moveZ);
	glScalef(0.383, 0.383, 0.383);
	glRotatef(pitch / 58.8, 0, 1, 0);
	RenderObject(textureIDMercury, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch / 0.615, 0, 1, 0);
	glPushMatrix();
	glTranslated(moveX + 7, moveY, moveZ);
	glScalef(0.949, 0.949, 0.949);
	glRotatef(pitch / -244, 0, 1, 0);
	RenderObject(textureIDVenus, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch, 0, 1, 0);
	glTranslated(moveX + 11, moveY, moveZ);
	//glScalef(0.1, 0.1, 0.1);
	glRotatef(pitch, 0, 1, 0.3);
	//glRotatef(yaw, 1, 0, 0);
	RenderObject(textureIDEarth, 10, 1);

	glPushMatrix();
	glRotatef(pitch / 0.0748, 0, 1, 0);
	glTranslated(moveX + 2, moveY, moveZ);
	glScalef(0.2724, 0.2724, 0.2724);
	glRotatef(pitch / 27.4, 0, 1, 0);
	RenderObject(textureIDMoon, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch / 1.88, 0, 1, 0);
	glPushMatrix();
	glTranslated(moveX + 14, moveY, moveZ);
	glScalef(0.532, 0.532, 0.532);
	glRotatef(pitch / 1.03, 0, 1, 0);
	RenderObject(textureIDMars, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch / 11.9, 0, 1, 0);
	glPushMatrix();
	glTranslated(moveX - 17, moveY, moveZ);
	glScalef(1.121, 1.121, 1.121);
	glRotatef(pitch / 0.415, 0, 1, 0);
	RenderObject(textureIDJupiter, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch / 29.4, 0, 1, 0);
	glPushMatrix();
	glTranslated(moveX + 20, moveY, moveZ);
	glScalef(1.121, 1.121, 1.121);
	glRotatef(pitch / 0.445, 0, 1, 0);
	RenderObject(textureIDSaturn, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch / 83.7, 0, 1, 0);
	glPushMatrix();
	glTranslated(moveX - 23, moveY, moveZ);
	glScalef(0.8, 0.8, 0.8);
	glRotatef(pitch / -0.720, 0, 1, 0);
	RenderObject(textureIDUranus, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(pitch / 163.7, 0, 1, 0);
	glPushMatrix();
	glTranslated(moveX + 25, moveY, moveZ);
	glScalef(0.388, 0.388, 0.388);
	glRotatef(pitch / 0.673, 0, 1, 0);
	RenderObject(textureIDNeptune, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}

static void Key(unsigned char key, int x, int y) {

	switch (key) {
	case '+':
		zoom /= 0.75;
		glutPostRedisplay();
		break;
	case '-':
		zoom *= 0.75;
		glutPostRedisplay();
		break;
	case 'l':
		linePoly = !linePoly;
		if (linePoly) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glutPostRedisplay();
		break;
	case 'p':
		pointDraw = !pointDraw;
		if (pointDraw) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glutPostRedisplay();
		break;
	case 'b':
		backColor = !backColor;
		if (backColor) {
			glClearColor(0.0, 0.0, 0.0, 1.0);
		} else {
			glClearColor(1.0, 1.0, 1.0, 1.0);
		}
		glutPostRedisplay();
		break;
	case 'f':
		if (flyThrough == -1) {
			flyThrough = 0;
		}
		flyThrough ^= 1;
		glutPostRedisplay();
		break;
	case 'r':
		flyThrough = -1;
		cameraDistance = 35;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
	}
}

void mouse(int button, int state, int x, int y) {
	newX = x;
	newY = y;

	// Left mouse button pressed
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		LeftButtonDown = 1;
		ShiftLeftButtonDown = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
	} else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		MiddleButtonDown = 1;
	} else {
		MiddleButtonDown = ShiftLeftButtonDown = LeftButtonDown = 0;
	}
}

void motion(int x, int y) {
	float RelX, RelY;

	oldX = newX;
	oldY = newY;
	newX = x;
	newY = y;

	RelX = (newX - oldX) / (float) glutGet((GLenum) GLUT_WINDOW_WIDTH);
	RelY = (newY - oldY) / (float) glutGet((GLenum) GLUT_WINDOW_HEIGHT);

	if (ShiftLeftButtonDown) {
		// Change distance
		cameraDistance += 10 * RelY;

	} else if (LeftButtonDown) {
		// Change Rotation
		pitchM += (RelX * 180);
		yawM += (RelY * 180);
	} else if (MiddleButtonDown) {

		moveX += RelX * 2 / zoom;
		moveY -= RelY * 2 / zoom;
	}

	glutPostRedisplay();
}

void timer(int v) {

	pitch = (pitch + 1.0);
	glutPostRedisplay();
	glutTimerFunc(30, timer, v);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Solar System");
	glutReshapeWindow(windW, windH);
	Init();
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutDisplayFunc(Draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT2, GL_POSITION, front_light_position);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	glutTimerFunc(100, timer, 0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glutMainLoop();
	return 0;
}

