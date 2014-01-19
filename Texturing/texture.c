#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>

extern void RenderEnvironment(void);
extern void LoadEnvironmentTexture(void);


float zRotation = 0.0;
float zoom = 1;

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


GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat left_light_position[] = { -40.0, 0.0, -40.0, 1.0 }, right_light_position[] = { 40.0, 0.0, -40.0, 1.0 };

unsigned char* skinImg;

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

Vertex* vertices;
Triangle* polygons;
Normals* normals;
Vertex* origin;

GLint noOfVertices = 0;
GLint noOfFaces = 0;

GLuint textureIDSkin;

float imageSize;
int imgWidth, imgHeight;



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

	cameraDistance = maxZ - (origin[0].coord[2]);

	cameraDistance = cameraDistance * 10;
	moveX = -1 * origin[0].coord[0];
	moveY = -1 * origin[0].coord[1];

	fscanf(coordinateReader, "%d\n", &noOfFaces);

	polygons = (Triangle*) malloc(sizeof(Triangle) * noOfFaces);
	normals = (Normals*) calloc(noOfVertices, sizeof(Normals));

	verticesStartPoint = ftell(coordinateReader);
	fseek(vertexReader, verticesStartPoint, SEEK_SET);
	fseek(coordinateReader, coordinateStartPoint, SEEK_SET);
	for (i = 0; i < noOfFaces; i++) {
		fscanf(vertexReader, "%d %d %d\n", &polygons[i].Vertices[0], &polygons[i].Vertices[1], &polygons[i].Vertices[2]);
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

static void LoadTextures(void) {

	loadTriFile("cow.tri");

	skinImg = SOIL_load_image("skin.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_AUTO);

	glGenTextures(1, &textureIDSkin);

	glBindTexture(GL_TEXTURE_2D, textureIDSkin);

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, skinImg);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	SOIL_free_image_data(skinImg);


	LoadEnvironmentTexture();

	imageSize = imgWidth * imgHeight;

}

static void Reshape(int width, int height) {
	windW = width;
	windH = height;
	glViewport(0, 0, windW, windH);
	// Set viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float) width / height, 0.001f, 130);

	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}





static void Render() {
	GLint i;

	int v1, v2, v3;

	GLfloat Xinc = minX;
	GLfloat Yinc = minY;

	GLfloat Ydiff = 0.0;

	GLfloat scale = 0.1f;

	glBindTexture(GL_TEXTURE_2D, textureIDSkin);
	for (i = 0; i < noOfFaces; i++) {
		v1 = polygons[i].Vertices[0];
		v2 = polygons[i].Vertices[1];
		v3 = polygons[i].Vertices[2];

		glBegin(GL_TRIANGLES);

		glNormal3fv(normals[v1].normal);

		Ydiff = (vertices[v1].coord[1] - Yinc) * scale;
		glTexCoord2f((vertices[v1].coord[0] - Xinc) * scale, Ydiff);
		glVertex3fv(vertices[v1].coord);

		glNormal3fv(normals[v2].normal);
		Ydiff = (vertices[v2].coord[1] - Yinc) * scale;
		glTexCoord2f((vertices[v2].coord[0] - Xinc) * scale, Ydiff);
		glVertex3fv(vertices[v2].coord);

		glNormal3fv(normals[v3].normal);
		Ydiff = (vertices[v3].coord[1] - Yinc) * scale;
		glTexCoord2f((vertices[v3].coord[0] - Xinc) * scale, Ydiff);
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

	gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

	glTranslated(moveX, moveY, moveZ);

	//glRotatef(yaw, 1, 0, 0);
	glRotatef(pitch, 0, 1, 0);

	glScalef(zoom, zoom, zoom);


	glPushMatrix();
	glRotatef(180,1,0,0);
	glScalef(1, 0.3, 1);
	glTranslated(0, -39.6, 0);
	RenderEnvironment();
	glPopMatrix();


	Render();


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


int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Texturing Cow");
	glutReshapeWindow(windW, windH);
	LoadTextures();
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutDisplayFunc(Draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH);


	glutMainLoop();
	return 0;
}

