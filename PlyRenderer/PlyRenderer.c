#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>

#define GLEW_STATIC
#define	FILLED 1
#define	WIREFRAME 2
#define	POINTCLOUD 3

int callback_set;
float previous_time, current_time;

//Scaling Variable
float zoom = 1;

//Sign variable for pitch value change
int op = 1;

//Angles
float yaw = 1;
float lightPitch = 0, pitch = 0;

float cameraDistance = 2;

//Mouse Button ref
int LeftButtonDown = 0;
int MiddleButtonDown = 0;
int ShiftLeftButtonDown = 0;

//Variables for Change in mouse position
int oldX, oldY, newX = 1, newY = 1;

//Variable for displacement of mouse
float moveX = 0, moveY = 0;

//variable to store maximum value of Z coordinate in given dataset
double maxZ = -100.0;

//Boolean variables
GLenum linePoly = GL_FALSE;
GLenum pointDraw = GL_FALSE;
GLenum backColor = GL_FALSE;
GLenum withLight = GL_FALSE;
GLint usePerVertexNormals = GL_TRUE;
GLint automaticRotate = GL_TRUE;
//Window size
GLint windW = 800, windH = 600;

int shade_model = GL_SMOOTH;

//String for storing light names
char *left_light, *right_light, *front_light, fileName[30];

/****************************Definition of Structures****************************/

//For storing Vertices
typedef struct vertex {
	GLdouble coord[3];
} Vertex;

//For storing Normals
typedef struct Normals {
	GLdouble normal[3];
	int count;
} Normals;

//For storing Polygon faces
typedef struct Polygon {
	int* Vertices;
} Polygon;

GLint noOfFaces = 0;
GLint noOfVertices = 0;

Vertex* vertices;
Polygon* polygons;
Normals* normals;
Normals* normalsPerFragment = NULL;
Vertex* origin;

/******************************************Quaternion**********************************/

typedef struct Vector {
	float x, y, z;
} Vector;

//Structure to store quaternion
typedef struct Quaternion {
	float w, x, y, z;
} Quaternion;

// Dot product of two vectors
float dotProduct(Vector* vect1, Vector* vect2) {
	return (vect1->x * vect2->x + vect1->y * vect2->y + vect1->z * vect2->z);
}

//Create Quaternion using reference axis and angle of rotation
Quaternion createRotationQuaternion(float angle, Vector axis) {
	Quaternion quat;
	quat.w = cos(angle / 2.0);
	quat.x = axis.x * sin(angle / 2.0);
	quat.y = axis.y * sin(angle / 2.0);
	quat.z = axis.z * sin(angle / 2.0);
	return quat;
}

Quaternion multiplyQuaternion(Quaternion quat1, Quaternion quat2) {
	return (Quaternion ) { quat1.w * quat2.w - quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z,
		quat1.w * quat2.x + quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y, quat1.y
					* quat2.w + quat1.z * quat2.x + quat1.w * quat2.y - quat1.x * quat2.z, quat1.w * quat2.z + quat1.z * quat2.w + quat1.x * quat2.y - quat1.y * quat2.x } ;
		}

		Vector rotateVector(Quaternion quat, Vector* vect) {
			Quaternion tempQuat;
			tempQuat.w = 0;
			tempQuat.x = vect->x;
			tempQuat.y = vect->y;
			tempQuat.z = vect->z;

			tempQuat = multiplyQuaternion(quat, tempQuat);

			quat.x *= -1;
			quat.y *= -1;
			quat.z *= -1;

			tempQuat = multiplyQuaternion(tempQuat, quat);

			Vector temp = (Vector ) { tempQuat.x, tempQuat.y, tempQuat.z };

			return temp;
		}

		static GLdouble* createRotationMatrix(Quaternion quat) {

			GLdouble rtMat[16];
			Vector axes[3];
			Vector rotateAbtAxes[3];

			axes[0].x = 1;
			axes[0].y = 0;
			axes[0].z = 0;
			axes[1].x = 0;
			axes[1].y = 1;
			axes[1].z = 0;
			axes[2].x = 0;
			axes[2].y = 0;
			axes[2].z = 1;

			rotateAbtAxes[0] = rotateVector(quat, &axes[0]);
			rotateAbtAxes[1] = rotateVector(quat, &axes[1]);
			rotateAbtAxes[2] = rotateVector(quat, &axes[2]);

			int i, j, count = 0;
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					rtMat[count++] = dotProduct(&axes[j], &rotateAbtAxes[i]);
					//  printf("%f ",rtMat[count]);
				}
				rtMat[count++] = 0;
				//  printf("%f\n",rtMat[count]);
			}
			rtMat[count++] = 0;
//  printf("%f ",rtMat[count]);
			rtMat[count++] = 0;
			// printf("%f ",rtMat[count]);
			rtMat[count++] = 0;
			// printf("%f ",rtMat[count]);
			rtMat[count] = 1;
//  printf("%f\n",rtMat[count]);

			return rtMat;

		}

		static void rotateUsingQuaternion(float angle, int aboutX, int aboutY, int aboutZ) {

			Quaternion quat = createRotationQuaternion(angle, (Vector ) { aboutX, aboutY, aboutZ });
			glMultMatrixd(createRotationMatrix(quat));

		}

		/********************************************************************************/

		/***********************************Lighting*************************************/

		enum {
			LIGHT_OFF, LIGHT_RED, LIGHT_WHITE, LIGHT_GREEN
		} LightValues;

		GLfloat leftLightX = -10.0;
		GLfloat leftLightZ = 10.0;
		GLfloat rightLightX = 10.0;
		GLfloat rightLightZ = -10.0;

		GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 }, green_light[] = { 0.0, 1.0, 0.0, 1.0 }, white_light[] = { 1.0, 1.0, 1.0, 1.0 };

		GLfloat left_light_position[] = { 10.0, 0.0, -5.0, 0.0 }, right_light_position[] = { -10.0, 0.0, -5.0, 1.0 }, front_light_position[] = { 0.0, 0.0, -10.0, 1.0 };

		GLfloat white_ambient[] = { 0.2, 0.2, 0.2, 1.0 }, white_diffuse[] = { 0.5, 0.5, 0.5, 1.0 }, white_specular[] = { 0.3, 0.3, 0.3, 1.0 }, white_shininess = 65.0;

		/***********************************Lighting*************************************/

		void calculatePerFragmentNormal(int vertex1, int vertex2, int vertex3, int faceNo) {

			GLdouble va[3], vb[3], vr[4], Nfact;

			GLdouble *coord1 = vertices[vertex1].coord;
			GLdouble *coord2 = vertices[vertex2].coord;
			GLdouble *coord3 = vertices[vertex3].coord;

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
			Nfact = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

			normalsPerFragment[faceNo].normal[0] = vr[0] / Nfact;

			normalsPerFragment[faceNo].normal[1] = vr[1] / Nfact;

			normalsPerFragment[faceNo].normal[2] = vr[2] / Nfact;

		}

		void calculatePerVertexNormal(int vertex1, int vertex2, int vertex3) {
			/* calculate Vector1 and Vector2 */

			GLdouble va[3], vb[3], vr[4], Nfact;

			GLdouble *coord1 = vertices[vertex1].coord;
			GLdouble *coord2 = vertices[vertex2].coord;
			GLdouble *coord3 = vertices[vertex3].coord;

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
			Nfact = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

			normals[vertex1].normal[0] += vr[0] / Nfact;
			normals[vertex2].normal[0] += vr[0] / Nfact;
			normals[vertex3].normal[0] += vr[0] / Nfact;

			normals[vertex1].normal[1] += vr[1] / Nfact;
			normals[vertex2].normal[1] += vr[1] / Nfact;
			normals[vertex3].normal[1] += vr[1] / Nfact;

			normals[vertex1].normal[2] += vr[2] / Nfact;
			normals[vertex2].normal[2] += vr[2] / Nfact;
			normals[vertex3].normal[2] += vr[2] / Nfact;

			normals[vertex1].count += 1;
			normals[vertex2].count += 1;
			normals[vertex3].count += 1;

		}

		static void Normalize() {
			int i = 0;
			GLdouble normalizingFactor = 0.0;
			normalizingFactor = sqrt(origin[0].coord[0] * origin[0].coord[0] + origin[0].coord[1] * origin[0].coord[1] + origin[0].coord[2] * origin[0].coord[2]);
			maxZ /= normalizingFactor;
			for (i = 0; i < noOfVertices; i++) {

				vertices[i].coord[0] /= normalizingFactor;
				vertices[i].coord[1] /= normalizingFactor;
				vertices[i].coord[2] /= normalizingFactor;

				if (maxZ < vertices[i].coord[2])
					maxZ = vertices[i].coord[2];
			}

			origin[0].coord[0] /= normalizingFactor;
			origin[0].coord[1] /= normalizingFactor;
			origin[0].coord[2] /= normalizingFactor;

		}

		static void loadPlyFile() {

			FILE *vertexReader, *coordinateReader;
			int i = 0;
			int noOfProperties = 0;
			int coordinateStartPoint;
			int verticesStartPoint;
			char temp[100];
			long offset;
			char key[10];

			if (strlen(fileName) < 5)
				strcpy(fileName, "bunny.ply");

			if ((vertexReader = fopen(fileName, "r")) == NULL) {
				printf("File not found !");
				return;
			}
			coordinateReader = fopen(fileName, "r");

			fgets(temp, 100, coordinateReader); //line 1
			fgets(temp, 100, coordinateReader); //line 1
			offset = ftell(coordinateReader);
			fgets(temp, 100, coordinateReader); //Try to read comments
			while (strstr(temp, "comment")) {
				offset = ftell(coordinateReader);
				fgets(temp, 100, coordinateReader);
			}

			fseek(coordinateReader, offset, SEEK_SET);
			fscanf(coordinateReader, "%s %s %d\n", key, temp, &noOfVertices);

			fscanf(coordinateReader, "%s %s %s\n", key, temp, temp); //property
			while (strcmp(key, "property") == 0) {
				offset = ftell(coordinateReader);
				fscanf(coordinateReader, "%s %s %s\n", key, temp, temp); //Properties
				noOfProperties++;
			}
			fseek(coordinateReader, offset, SEEK_SET);
			fscanf(coordinateReader, "%s %s %d\n", key, temp, &noOfFaces);

			fgets(temp, 200, coordinateReader);

			fscanf(coordinateReader, "%s\n", temp);
			coordinateStartPoint = ftell(coordinateReader);

			vertices = (Vertex*) malloc(sizeof(Vertex) * noOfVertices);
			origin = (Vertex*) calloc(1, sizeof(Vertex));
			i = 0;

			while (i < noOfVertices) {

				fscanf(coordinateReader, "%lf %lf %lf", &vertices[i].coord[0], &vertices[i].coord[1], &vertices[i].coord[2]);
				fgets(temp, 200, coordinateReader);

				origin[0].coord[0] += vertices[i].coord[0];
				origin[0].coord[1] += vertices[i].coord[1];
				origin[0].coord[2] += vertices[i].coord[2];
				if (maxZ < vertices[i].coord[2])
					maxZ = vertices[i].coord[2];

				i++;
			}

			origin[0].coord[0] /= noOfVertices;
			origin[0].coord[1] /= noOfVertices;
			origin[0].coord[2] /= noOfVertices;

			Normalize();

			cameraDistance = maxZ - (origin[0].coord[2]);

			cameraDistance = cameraDistance * 5;
			moveX = -1 * origin[0].coord[0];
			moveY = -1 * origin[0].coord[1];

			polygons = (Polygon*) malloc(sizeof(Polygon) * noOfFaces);
			normals = (Normals*) calloc(noOfVertices, sizeof(Normals));

			verticesStartPoint = ftell(coordinateReader);

			fseek(vertexReader, verticesStartPoint, SEEK_SET);

			int noOfverticesForthisPolygon = 0;
			int vertexCount = 0;
			for (i = 0; i < noOfFaces; i++) {

				fscanf(vertexReader, "%d", &noOfverticesForthisPolygon);
				polygons[i].Vertices = (int*) malloc(noOfverticesForthisPolygon * sizeof(int));

				vertexCount = 0;
				while (vertexCount < noOfverticesForthisPolygon) {
					fscanf(vertexReader, " %d", &polygons[i].Vertices[vertexCount]);
					vertexCount++;
				}
				fgets(temp, 10, coordinateReader);

				calculatePerVertexNormal(polygons[i].Vertices[0], polygons[i].Vertices[1], polygons[i].Vertices[2]);
			}

			fclose(vertexReader);
			fclose(coordinateReader);

		}

		static void Reshape(int width, int height) {
			windW = width;
			windH = height;
			glViewport(0, 0, windW, windH);
			// Set viewing frustum
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45, (float) width / height, 0.001f, 80);

			// Modelview matrix reset
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		static void Render() {
			GLint i;

			int v1, v2, v3;

			if (usePerVertexNormals) {
				for (i = 0; i < noOfFaces; i++) {

					v1 = polygons[i].Vertices[0];
					v2 = polygons[i].Vertices[1];
					v3 = polygons[i].Vertices[2];

					glBegin(GL_TRIANGLES);

					glNormal3d(normals[v1].normal[0] / normals[v1].count, normals[v1].normal[1] / normals[v1].count, normals[v1].normal[2] / normals[v1].count);

					glVertex3dv(vertices[v1].coord);

					glNormal3d(normals[v2].normal[0] / normals[v2].count, normals[v2].normal[1] / normals[v2].count, normals[v2].normal[2] / normals[v2].count);

					glVertex3dv(vertices[v2].coord);

					glNormal3d(normals[v3].normal[0] / normals[v3].count, normals[v3].normal[1] / normals[v3].count, normals[v3].normal[2] / normals[v3].count);

					glVertex3dv(vertices[v3].coord);

					glEnd();

				}
			} else {
				for (i = 0; i < noOfFaces; i++) {

					v1 = polygons[i].Vertices[0];
					v2 = polygons[i].Vertices[1];
					v3 = polygons[i].Vertices[2];

					glBegin(GL_TRIANGLES);

					glNormal3d(normalsPerFragment[i].normal[0], normalsPerFragment[i].normal[1], normalsPerFragment[i].normal[2]);

					glVertex3dv(vertices[v1].coord);

					glVertex3dv(vertices[v2].coord);

					glVertex3dv(vertices[v3].coord);

					glEnd();

				}
			}

		}

		static void Draw(void) {

			double moveZ = -1 * origin[0].coord[2];

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_LIGHTING);
			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			GLdouble rollSquare = leftLightZ * leftLightZ - lightPitch * lightPitch;
			if (rollSquare < 0)
				rollSquare *= -1;
			GLdouble roll = sqrt(rollSquare);

			if (withLight) {
				left_light_position[0] = lightPitch;
				left_light_position[2] = op * roll;
				glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
			}

			gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

			rotateUsingQuaternion(yaw / 10, 1, 0, 0);
			rotateUsingQuaternion(pitch / 10, 0, 1, 0);

			glTranslated(moveX, moveY, moveZ);

			glScalef(zoom, zoom, zoom);
			glEnable(GL_LIGHTING);
			Render();
			glPopMatrix();
			glutSwapBuffers();

		}

		void animate(float dt) {

			if (automaticRotate) {
				if (lightPitch < op * leftLightZ)
					op = 1;
				else
					op = -1;

				lightPitch += (op * 0.065);
				pitch += (0.1);
				glutPostRedisplay();
			}
		}

		void timer(int ignored) {

			current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			animate(current_time - previous_time);
			previous_time = current_time;
			glutTimerFunc(20, timer, -1);

		}

		void visibility(int state) {
			if (state == GLUT_VISIBLE && !callback_set) {
				callback_set = 1;
				previous_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
				timer(-1);
			}
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
				withLight ^= 1;
				glutPostRedisplay();
				break;
			case 'n':
				usePerVertexNormals ^= 1;
				if (usePerVertexNormals == 0 && normalsPerFragment == NULL) {
					int i;
					normalsPerFragment = (Normals*) calloc(noOfFaces, sizeof(Normals));
					for (i = 0; i < noOfFaces; i++) {
						calculatePerFragmentNormal(polygons[i].Vertices[0], polygons[i].Vertices[1], polygons[i].Vertices[2], i);
					}
				}
				glutPostRedisplay();
				break;
			case 'w':
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
				cameraDistance += 100 * RelY;

			} else if (LeftButtonDown) {
				// Change Rotation
				pitch += (RelX * 180);
				yaw += (RelY * 180);
			} else if (MiddleButtonDown) {
				// Translate
				moveX += RelX * 2;
				moveY -= RelY * 2;
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

		void shadingSelect(int value) {
			glShadeModel(value);
			glutPostRedisplay();
		}

		void normalsSelect(int value) {

			usePerVertexNormals = value;
			if (usePerVertexNormals == 0 && normalsPerFragment == NULL) {
				int i;
				normalsPerFragment = (Normals*) calloc(noOfFaces, sizeof(Normals));
				for (i = 0; i < noOfFaces; i++) {
					calculatePerFragmentNormal(polygons[i].Vertices[0], polygons[i].Vertices[1], polygons[i].Vertices[2], i);
				}
			}
			glutPostRedisplay();
		}

		void changeRotation(int value) {

			if (value == 0)
				automaticRotate = 0;
			if (value == 1)
				automaticRotate = 1;
			if (value == 2)
				withLight = 1;
			if (value == 3)
				withLight = 0;

		}

		void changeLeftLight(int value) {
			selectLight(GL_LIGHT0, value, &left_light);
		}

		void changeRightLight(int value) {
			selectLight(GL_LIGHT1, value, &right_light);
		}

		void changeFrontLight(int value) {
			switch (value) {
			case LIGHT_OFF:
				glDisable(GL_LIGHT2);
				break;
			case LIGHT_RED:
				glLightfv(GL_LIGHT2, GL_AMBIENT, red_light);
				break;
			case LIGHT_WHITE:
				glLightfv(GL_LIGHT2, GL_AMBIENT, white_light);
				break;
			case LIGHT_GREEN:
				glLightfv(GL_LIGHT2, GL_AMBIENT, green_light);
				break;
			}
			glutPostRedisplay();
		}

		void main_menu_select(int value) {
			if (value == 666)
				exit(0);
			glShadeModel(shade_model = value);
			glutPostRedisplay();
		}

		static void Init() {

			loadPlyFile();

			int shadingMenuEntries, leftLightMenuEntries, rightLightMenuEntries, frontLightMenuEntries, normalMenuEntries, rotateMenuEntries;

#define SHADING_MENU_ENTRIES() \
    glutAddMenuEntry("Flat", GL_FLAT); \
    glutAddMenuEntry("Smooth", GL_SMOOTH);

#define NORMAL_MENU_ENTRIES() \
    glutAddMenuEntry("Per Pixel", 1); \
    glutAddMenuEntry("Per Fragment", 0);

#define ROTATE_MENU_ENTRIES() \
    glutAddMenuEntry("Start", 1); \
    glutAddMenuEntry("Stop", 0); \
    glutAddMenuEntry("With Light", 2); \
    glutAddMenuEntry("Without Light", 3);

#define LIGHT_MENU_ENTRIES() \
    glutAddMenuEntry("Disable", LIGHT_OFF); \
    glutAddMenuEntry("Red", LIGHT_RED); \
    glutAddMenuEntry("White", LIGHT_WHITE); \
    glutAddMenuEntry("Green", LIGHT_GREEN);

			rotateMenuEntries = glutCreateMenu(changeRotation);
			ROTATE_MENU_ENTRIES()
			;

			normalMenuEntries = glutCreateMenu(normalsSelect);
			NORMAL_MENU_ENTRIES()
			;

			shadingMenuEntries = glutCreateMenu(shadingSelect);
			SHADING_MENU_ENTRIES()
			;

			leftLightMenuEntries = glutCreateMenu(changeLeftLight);
			LIGHT_MENU_ENTRIES()
			;

			rightLightMenuEntries = glutCreateMenu(changeRightLight);
			LIGHT_MENU_ENTRIES()
			;

			frontLightMenuEntries = glutCreateMenu(changeFrontLight);
			LIGHT_MENU_ENTRIES()
			;

			glutCreateMenu(main_menu_select);
			glutAddSubMenu("Shading", shadingMenuEntries);
			glutAddSubMenu("Normals", normalMenuEntries);
			glutAddSubMenu("Rotate", rotateMenuEntries);
			glutAddSubMenu("Left light", leftLightMenuEntries);
			glutAddSubMenu("Right light", rightLightMenuEntries);
			glutAddSubMenu("Front light", frontLightMenuEntries);
			glutAddMenuEntry("Quit", 666);
			glutAttachMenu(GLUT_RIGHT_BUTTON);

			glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
			glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
			glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
			glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
			glLightfv(GL_LIGHT2, GL_POSITION, front_light_position);
			glLightfv(GL_LIGHT2, GL_AMBIENT, white_light);

			changeLeftLight(LIGHT_WHITE);
			changeRightLight(LIGHT_WHITE);
			changeFrontLight(LIGHT_WHITE);

			glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, white_specular);
			glMaterialf(GL_FRONT, GL_SHININESS, white_shininess);

			glEnable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_NORMALIZE);
			glEnable(GL_BLEND);

		}

		int main(int argc, char **argv) {

			if (argc != 2) {
				printf("Provide ply filename as argument.\nEx: ./PlyRenderer bunny.ply %d", argc);
				return 0;
			} else
				strcpy(fileName, argv[1]);

			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
			glutCreateWindow("Ply File Rendering");
			glutReshapeWindow(windW, windH);
			Init();
			glutReshapeFunc(Reshape);
			glutKeyboardFunc(Key);
			glutDisplayFunc(Draw);
			glutMouseFunc(mouse);
			glutMotionFunc(motion);
			glutVisibilityFunc(visibility);

			glutMainLoop();
			return 0;
		}

