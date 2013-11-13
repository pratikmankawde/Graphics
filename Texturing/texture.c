#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>

#define GLEW_STATIC

#define	SOLID 1
#define	LINE 2
#define	POINT 3

GLint vp[4];
float zRotation = 0.0;
float zoom = 1;
GLint objectCount;
GLint numObjects;

float yaw = 0;
float pitch = 0;
float cameraDistance = 1;

int LeftButtonDown = 0;
int ShiftLeftButtonDown = 0;
int MiddleButtonDown = 0;

int oldX, oldY, newX, newY;

//Variable for displacement of mouse
float moveX = 0, moveY = 0;

//variable to store maximum value of Z coordinate in given dataset
double maxZ = -100.0, maxX = -100.0, maxY = -100.0, minX = 0.0, minY = 0.0, minZ = 0.0;

GLenum linePoly = GL_FALSE;
GLenum pointDraw = GL_FALSE;
GLenum backColor = GL_FALSE;

GLint windW = 800, windH = 600;

enum {
	BRASS, RED_PLASTIC, EMERALD, MATTE
} MaterialType;
enum {
	TORUS_MATERIAL = 1, TEAPOT_MATERIAL = 2, ICO_MATERIAL = 3
} MaterialDisplayList;
enum {
	LIGHT_OFF, LIGHT_RED, LIGHT_WHITE, LIGHT_GREEN
} LightValues;

GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 }, green_light[] = { 0.0, 1.0, 0.0, 1.0 }, white_light[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat left_light_position[] = { -20.0, 0.0, -10.0, 1.0 }, right_light_position[] = { 20.0, 0.0, -10.0, 1.0 };
//	front_light_position[] ={0.0, 0.0, 1.0, 1.0};

GLfloat brass_ambient[] = { 0.33, 0.22, 0.03, 1.0 }, brass_diffuse[] = { 0.78, 0.57, 0.11, 1.0 }, brass_specular[] = { 0.99, 0.91, 0.81, 1.0 }, brass_shininess = 100.0;

GLfloat red_plastic_ambient[] = { 0.0, 0.0, 0.0 }, red_plastic_diffuse[] = { 0.5, 0.0, 0.0 }, red_plastic_specular[] = { 0.7, 0.6, 0.6 }, red_plastic_shininess = 32.0;

GLfloat emerald_ambient[] = { 0.0215, 0.1745, 0.0215 }, emerald_diffuse[] = { 0.07568, 0.61424, 0.07568 }, emerald_specular[] = { 0.633, 0.727811, 0.633 }, emerald_shininess = 76.8;

GLfloat white_ambient[] = { 0.9, 0.9, 0.9, 1.0 }, white_diffuse[] = { 0.4, 0.4, 0.4, 1.0 }, white_specular[] = { 0.5, 0.5, 0.5, 1.0 }, white_shininess = 60.0;

int shade_model = GL_SMOOTH;
char *left_light, *right_light;
char *ico_material, *teapot_material, *torus_material;
unsigned char* image;
unsigned char* ground;
unsigned char* environment;

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
GLint vp[4];
Vertex* vertices;
Triangle* polygons;
Normals* normals;
Vertex* origin;

int noOfFaces = 0;
float imageSize;
int imgWidth, imgHeight;
GLuint textureIDSkin, textureIDGround, textureIDEnvironment;

#define PI 3.141592654
#define TWOPI 6.283185308

void SphereMap(float* vertices, float *t, float *s) {

	float radius = 0.1f;

	float x = vertices[0] * radius;
	float y = vertices[1] * radius;
	float z = vertices[2] * radius;

	*t = (atan2(z, x) / (2 * PI)) + 0.5;

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

	printf("normalizingFactor %f\n", normalizingFactor);
	if (normalizingFactor < 0.000001)
		return;

	maxZ /= normalizingFactor;
	for (i = 0; i < noOfVertices; i++) {

		vertices[i].coord[0] /= normalizingFactor;
		vertices[i].coord[1] /= normalizingFactor;
		vertices[i].coord[2] /= normalizingFactor;

		if (maxX < vertices[i].coord[0])
			maxX = vertices[i].coord[0];

		if (maxY < vertices[i].coord[1])
			maxY = vertices[i].coord[1];

		if (maxZ < vertices[i].coord[2])
			maxZ = vertices[i].coord[2];

		if (minX > vertices[i].coord[0])
			minX = vertices[i].coord[0];

		if (minY > vertices[i].coord[1])
			minY = vertices[i].coord[1];
		if (minZ > vertices[i].coord[2])
			minZ = vertices[i].coord[2];

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
	//int lineSize = sizeof(float)
	if ((vertexReader = fopen(fileName, "r")) == NULL) {
		printf("File not found !");
		return;
	}
	coordinateReader = fopen(fileName, "r");

//	printf("%s found !\n",fileName);

	fscanf(coordinateReader, "%d\n", &noOfVertices);

	//printf("noOfVertices=%d\n",noOfVertices);

	coordinateStartPoint = ftell(coordinateReader);

	i = 0;

	vertices = (Vertex*) malloc(sizeof(Vertex) * noOfVertices);
	origin = (Vertex*) calloc(1, sizeof(Vertex));

	while (i < noOfVertices) {
		fscanf(coordinateReader, "%f %f %f\n", &vertices[i].coord[0], &vertices[i].coord[1], &vertices[i].coord[2]);

		origin[0].coord[0] += vertices[i].coord[0];
		origin[0].coord[1] += vertices[i].coord[1];
		origin[0].coord[2] += vertices[i].coord[2];

		if (maxX < vertices[i].coord[0])
			maxX = vertices[i].coord[0];

		if (maxY < vertices[i].coord[1])
			maxY = vertices[i].coord[1];

		if (maxZ < vertices[i].coord[2])
			maxZ = vertices[i].coord[2];

		if (minX > vertices[i].coord[0])
			minX = vertices[i].coord[0];

		if (minY > vertices[i].coord[1])
			minY = vertices[i].coord[1];

		if (minZ > vertices[i].coord[2])
			minZ = vertices[i].coord[2];

		i++;
	}

	origin[0].coord[0] /= noOfVertices;
	origin[0].coord[1] /= noOfVertices;
	origin[0].coord[2] /= noOfVertices;

	Normalize();

	printf("Origin %f %f %f\n", origin[0].coord[0], origin[0].coord[1], origin[0].coord[2]);

	cameraDistance = maxZ - (origin[0].coord[2]);

	cameraDistance = cameraDistance * 5;
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
		fscanf(vertexReader, "%d %d %d\n", &polygons[i].Vertices[0], &polygons[i].Vertices[1], &polygons[i].Vertices[2]);
		calculatePerVertexNormal(polygons[i].Vertices[0], polygons[i].Vertices[1], polygons[i].Vertices[2]);
		//	printf("%d %d %d\n",triangles[i].Vertices[0],triangles[i].Vertices[1],triangles[i].Vertices[2]);
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
	//numObjects = 10;

	loadTriFile("cow.tri");

	glShadeModel(GL_SMOOTH);

	image = SOIL_load_image("img1.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);

	glGenTextures(1, &textureIDSkin);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureIDSkin);

	// Give the image to OpenGL
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	ground = SOIL_load_image("G2.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(2, &textureIDGround);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureIDGround);

	// Give the image to OpenGL
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ground);
	//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	environment = SOIL_load_image("E1.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);
	glGenTextures(3, &textureIDEnvironment);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureIDEnvironment);

	// Give the image to OpenGL
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, environment);
	//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, environment);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_SPHERE_MAP GL_CLAMP GL_REPEAT
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP); // GL_NORMAL_MAP  GL_EYE_LINEAR GL_OBJECT_LINEAR GL_REFLECTION_MAP
	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);



	static GLfloat sgenparams[] = {0, 10, 0, 0 };
	static GLfloat tgenparams[] = { 0, 0, 10, 0 };

	glTexGenfv( GL_S, GL_OBJECT_PLANE, sgenparams); //GL_OBJECT_PLANE GL_EYE_PLANE
	glTexGenfv( GL_T, GL_OBJECT_PLANE, tgenparams);

	//   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	SOIL_free_image_data(image);
	SOIL_free_image_data(ground);
	SOIL_free_image_data(environment);
	imageSize = imgWidth * imgHeight;

}

static void Reshape(int width, int height) {
	windW = width;
	windH = height;
	glViewport(0, 0, windW, windH);
	glGetIntegerv(GL_VIEWPORT, vp);
	// Set viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float) width / height, 0.001f, 80);

	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


static void surround(){

	//glEnable( GL_TEXTURE_GEN_S);
	//glEnable( GL_TEXTURE_GEN_T);

	glBindTexture(GL_TEXTURE_2D, textureIDGround);
		glBegin(GL_POLYGON);
		glNormal3d(0, 10, 0);
		glTexCoord2f(0, 1);
		glVertex3d(minX - 2, minY + 0.4, maxZ + 2);

		glTexCoord2f(1, 1);
		glVertex3d(maxX + 2, minY + 0.4, maxZ + 2);

		glTexCoord2f(1, 0);
		glVertex3d(maxX + 2, minY + 0.4, minZ - 2);

		glTexCoord2f(0, 0);
		glVertex3d(minX - 2, minY + 0.4, minZ - 2);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, textureIDEnvironment);
		glBegin(GL_QUADS);
		glNormal3d(1, 0, 0);
		glTexCoord2f(0, 1);
		glVertex3d(minX - 2, minY + 0.4, maxZ + 2);

		glTexCoord2f(0, 0);
		glVertex3d(minX - 2, maxY + 2.0, maxZ + 2);

		glNormal3d(0, 0, 1);
		glTexCoord2f(0.33, 0);
		glVertex3d(minX - 2, maxY + 2.0, minZ - 2);

		glNormal3d(0, 0, 1);
		glTexCoord2f(0.33, 1);
		glVertex3d(minX - 2, minY + 0.4, minZ - 2);

		glEnd();
		glBegin(GL_QUADS);

		glNormal3d(0, 0, 1);
		glTexCoord2f(0.33, 0);
		glVertex3d(minX - 2, maxY + 2.0, minZ - 2);

		glNormal3d(-1, 0, 1);
		glTexCoord2f(0.66, 0);
		glVertex3d(maxX + 2, maxY + 2.0, minZ - 2);

		glNormal3d(0, 0, 1);
		glTexCoord2f(0.66, 1);
		glVertex3d(maxX + 2, minY + 0.4, minZ - 2);

		glNormal3d(0, 0, 1);
		glTexCoord2f(0.33, 1);
		glVertex3d(minX - 2, minY + 0.4, minZ - 2);

		glEnd();
		glBegin(GL_QUADS);

		glNormal3d(-1, 0, 1);
		glTexCoord2f(0.66, 0);
		glVertex3d(maxX + 2, maxY + 2.0, minZ - 2);
		glNormal3d(-1, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3d(maxX + 2, maxY + 2.0, maxZ + 2);

		glTexCoord2f(1, 1);
		glVertex3d(maxX + 2, minY + 0.4, maxZ + 2);

		//glNormal3d(-1, 0, 1);
		glTexCoord2f(0.66, 1);
		glVertex3d(maxX + 2, minY + 0.4, minZ - 2);

		glEnd();


}



static void Render(GLenum mode) {
	GLint i;

	int v1, v2, v3;

	GLfloat Xinc = minX; //vertices[polygons[0].Vertices[0]].coord[0];
	GLfloat Yinc = minY; //vertices[polygons[0].Vertices[0]].coord[1];
	GLfloat Zinc = minZ; //vertices[polygons[0].Vertices[0]].coord[2];

	GLfloat Ydiff = 0.0;
	GLfloat Zdiff = 0.0;

	GLfloat texX;
	GLfloat texY;

	GLfloat scale = 0.1f;

	surround();

	glDisable( GL_TEXTURE_GEN_S);
	glDisable( GL_TEXTURE_GEN_T);


	glBindTexture(GL_TEXTURE_2D, textureIDSkin);
	for (i = 0; i < noOfFaces; i++) {
		if (mode == GL_SELECT) {
			glLoadName(i);
		}
		v1 = polygons[i].Vertices[0];
		v2 = polygons[i].Vertices[1];
		v3 = polygons[i].Vertices[2];

		glBegin(GL_TRIANGLES);

		glNormal3fv(normals[v1].normal);
		//	SphereMap( normals[v1].normal, &texX, &texY);
		//SphereMap(vertices[v1].coord, &texX, &texY);
		//	glTexCoord2f(texX*0.1,texY*0.1);
		//glTexCoord2f(texX, texY);
		Ydiff = (vertices[v1].coord[1] - Yinc) * scale;
//			Zdiff = (vertices[v1].coord[2] - Zinc)*scale;
//			if(normals[v1].normal[2] == 0.0 ){
//			Ydiff -= Zdiff;
//			//printf("%f,",normals[v1].normal[2]);
//			}
		//printf("%f ",normals[v1].normal[2]);
		glTexCoord2f((vertices[v1].coord[0] - Xinc) * scale, Ydiff);
		//glTexCoord2f(normals[v1].normal[0]*scale,normals[v1].normal[1]*scale);
		glVertex3fv(vertices[v1].coord);

		glNormal3fv(normals[v2].normal);
		//  SphereMap( normals[v2].normal,  &texX, &texY);
		//SphereMap(vertices[v2].coord, &texX, &texY);
		//	glTexCoord2f(texX*0.1,texY*0.1);
		//glTexCoord2f(texX, texY);
		Ydiff = (vertices[v2].coord[1] - Yinc) * scale;
//			Zdiff = (vertices[v2].coord[2] - Zinc)*scale;
//			if(normals[v2].normal[2] == 0.0 ){
//				Ydiff -= Zdiff;
//		//	printf("%f;",normals[v2].normal[2]);
//			}
		//	printf("%f ",normals[v2].normal[2]);
		glTexCoord2f((vertices[v2].coord[0] - Xinc) * scale, Ydiff);
		//glTexCoord2f(normals[v2].normal[0]*scale,normals[v2].normal[1]*scale);
		glVertex3fv(vertices[v2].coord);

		glNormal3fv(normals[v3].normal);
		// SphereMap( normals[v3].normal,  &texX, &texY);
		//SphereMap(vertices[v3].coord, &texX, &texY);
		//	glTexCoord2f(texX*0.1,texY*0.1);
		//glTexCoord2f(texX, texY);
		Ydiff = (vertices[v3].coord[1] - Yinc) * scale;
//			Zdiff = (vertices[v3].coord[2] - Zinc)*scale;
//			if(normals[v3].normal[2] == 0.0 ){
//				Ydiff -= Zdiff;
//			//printf("%f\n",normals[v3].normal[2]);
//			}
		//	printf(": %f %f %f | %f %f %f\n",vertices[v3].coord[0],vertices[v3].coord[1],vertices[v3].coord[2],normals[v3].normal[0],normals[v3].normal[1],normals[v3].normal[2]);
		glTexCoord2f((vertices[v3].coord[0] - Xinc) * scale, Ydiff);
		//glTexCoord2f(normals[v3].normal[0]*scale,normals[v3].normal[1]*scale);
		glVertex3fv(vertices[v3].coord);

		glEnd();

		//if(i%5==0)
		//	glutSwapBuffers();
	}

	//glutSwapBuffers();

	//free(image);

}





static void Draw(void) {

	double moveZ = -1 * origin[0].coord[2];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

	//surround();

	glTranslated(moveX, moveY, moveZ);

	glRotatef(yaw, 1, 0, 0);
	glRotatef(pitch, 0, 1, 0);

	glScalef(zoom, zoom, zoom);
	glCallList(TEAPOT_MATERIAL);

	Render(GL_RENDER);
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
		pitch += (RelX * 180);
		yaw += (RelY * 180);
	} else if (MiddleButtonDown) {

		moveX += RelX * 2 / zoom;
		moveY -= RelY * 2 / zoom;
	}

	glutPostRedisplay();
}

void selectLight(GLenum which, int value, char **label) {
	glEnable(which);
	switch (value) {
	case LIGHT_OFF:
		*label = "off";
		glDisable(which);
		break;
	case LIGHT_RED:
		*label = "red";
		glLightfv(which, GL_DIFFUSE, red_light);
		break;
	case LIGHT_WHITE:
		*label = "white";
		glLightfv(which, GL_DIFFUSE, white_light);
		break;
	case LIGHT_GREEN:
		*label = "green";
		glLightfv(which, GL_DIFFUSE, green_light);
		break;
	}
	glutPostRedisplay();
}

void changeLeftLight(int value) {
	selectLight(GL_LIGHT0, value, &left_light);
}

void changeRightLight(int value) {
	selectLight(GL_LIGHT1, value, &right_light);
}

void material(int dlist, GLfloat * ambient, GLfloat * diffuse, GLfloat * specular, GLfloat shininess) {
	glNewList(dlist, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glEndList();
}

char *
material_select(int object, int value) {
	glutPostRedisplay();
	switch (value) {
	case BRASS:
		material(object, brass_ambient, brass_diffuse, brass_specular, brass_shininess);
		return "brass";
	case RED_PLASTIC:
		material(object, red_plastic_ambient, red_plastic_diffuse, red_plastic_specular, red_plastic_shininess);
		return "red plastic";
	case EMERALD:
		material(object, emerald_ambient, emerald_diffuse, emerald_specular, emerald_shininess);
		return "emerald";
	case MATTE:
		material(object, white_ambient, white_diffuse, white_specular, white_shininess);
		return "slate";
	}
	return NULL; /* avoid bogus warning! */
}

void torus_select(int value) {
	torus_material = material_select(TORUS_MATERIAL, value);
}

void teapot_select(int value) {
	teapot_material = material_select(TEAPOT_MATERIAL, value);
}

void ico_select(int value) {
	ico_material = material_select(ICO_MATERIAL, value);
}

void main_menu_select(int value) {
	if (value == 666)
		exit(0);
	glShadeModel(shade_model = value);
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	int left_light_m, right_light_m, torus_m, teapot_m, ico_m;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Tri File Rendering");
	glutReshapeWindow(windW, windH);
	Init();
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutDisplayFunc(Draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

#define LIGHT_MENU_ENTRIES() \
    glutAddMenuEntry("Disable", LIGHT_OFF); \
    glutAddMenuEntry("Red", LIGHT_RED); \
    glutAddMenuEntry("White", LIGHT_WHITE); \
    glutAddMenuEntry("Green", LIGHT_GREEN);
#define MATERIAL_MENU_ENTRIES() \
    glutAddMenuEntry("Brass", BRASS); \
    glutAddMenuEntry("Red plastic", RED_PLASTIC); \
    glutAddMenuEntry("Emerald", EMERALD); \
    glutAddMenuEntry("Slate", MATTE);

	left_light_m = glutCreateMenu(changeLeftLight);
	LIGHT_MENU_ENTRIES()
	;
	right_light_m = glutCreateMenu(changeRightLight);
	LIGHT_MENU_ENTRIES()
	;
	torus_m = glutCreateMenu(torus_select);
	MATERIAL_MENU_ENTRIES()
	;
	teapot_m = glutCreateMenu(teapot_select);
	MATERIAL_MENU_ENTRIES()
	;
	ico_m = glutCreateMenu(ico_select);
	MATERIAL_MENU_ENTRIES()
	;

	glutCreateMenu(main_menu_select);
	glutAddMenuEntry("Smooth shading", GL_SMOOTH);
	glutAddMenuEntry("Flat shading", GL_FLAT);
	glutAddSubMenu("Left light", left_light_m);
	glutAddSubMenu("Right light", right_light_m);
	glutAddSubMenu("Material", teapot_m);
	glutAddMenuEntry("Quit", 666);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	changeLeftLight(LIGHT_WHITE);
	changeRightLight(LIGHT_WHITE);
	teapot_select(MATTE);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glutMainLoop();
	return 0;
}

