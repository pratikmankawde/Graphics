#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>

#define MAXOBJS 100
#define MAXSELECT 100


GLint windW = 700, windH = 350;

GLuint selectBuf[MAXSELECT];

GLint vp[4];

float zoom = 1.0;
GLint noOfVertices = 63;
GLint numObjects = 63;
char *text;
float keyWidth = GLUT_SCREEN_WIDTH * 0.1;
float keyHeight = GLUT_SCREEN_HEIGHT * 0.14;
float xOrigin = -153.0;
float yOrigin = 40.0;
void* font;
int capsLockActive = 0;
int shiftActive = 0;
int altActive = 0;
int ctrlActive = 0;
int zoomedKey=-1;


// Key Colors
int keyR = 382;
int keyG = 522;
int keyB = 546;

/*
int keyR = 0;
int keyG = 0;
int keyB = 0;
*/


double moveX=0.0;
double moveY=0.0;

static int noOfButtons = 1;
static int fingerDist = 0;
int var=14;
int row=0;

struct Key {
	float v1[3];	//Lower left vertex
	float v2[3];	//lower right vertex
	float v3[3];	//upper right vertex
	float v4[3];	//upper left vertex
	unsigned char color[3];	//color of key
	char chN[11];	//char normal
	char chS[11];   //char with shift
} Vertex[MAXOBJS];


/****************Function to set values in key structure****************/
static void InitKeys(GLint i, float x, float y, GLfloat width, GLfloat height, char *chN, char *chS) {


	//x
	Vertex[i].v1[0] = x;
	Vertex[i].v2[0] = x + width;
	Vertex[i].v3[0] = x + width;
	Vertex[i].v4[0] = x;

	//y
	Vertex[i].v1[1] = y;
	Vertex[i].v2[1] = y;
	Vertex[i].v3[1] = y + height;
	Vertex[i].v4[1] = y + height;

	//z
	Vertex[i].v1[2] = 0;
	Vertex[i].v2[2] = 0;
	Vertex[i].v3[2] = 0;
	Vertex[i].v4[2] = 0;

	//Color


if(row==5)
	fingerDist=0;

	if(i!=0 && ++fingerDist%2==0){
	keyR+=41;
	keyG+=85;
	keyB+=120;
	}
	if(i!=0 && noOfButtons++ == var ){
		row=0;
		keyR = 382;
		keyG = 522;
		keyB = 546;

		noOfButtons = 0;
		--var;
		if(var==11){
		fingerDist=0;
		}
	}



	Vertex[i].color[0] = keyR;
	Vertex[i].color[1] = keyG;
	Vertex[i].color[2] = keyB;

	strcpy(Vertex[i].chN, chN);
	if (chS != NULL)
		strcpy(Vertex[i].chS, chS);
	else
		strcpy(Vertex[i].chS, "");

}




/****************Function to initialize key structure****************/
static void Init(void) {

	GLint i = 1;

	float xGap = 0.5;
	float yGap = 1;
	float startX = xOrigin;
	float startY = yOrigin;

	text = (char*) malloc(100 * sizeof(char));
	strcpy(text, "");

	InitKeys(0, xOrigin, yOrigin + 50, keyWidth * 15.3, keyHeight+30, text, "");

	//Row 1
	row=1;
	InitKeys(i++, startX, startY, keyWidth, keyHeight, "`", "~");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "1", "!");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "2", "@");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "3", "#");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "4", "$");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "5", "%");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "6", "^");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "7", "&");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "8", "*");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "9", "(");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "0", ")");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "[", "{");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "]", "}");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth * 2, keyHeight, "Backspace", NULL);

	//Row2
	row=2;
	InitKeys(i++, startX = xOrigin, startY -= keyHeight + yGap, keyWidth * 1.5, keyHeight, "Tab", NULL);
	InitKeys(i++, startX += keyWidth * 1.5 + xGap, startY, keyWidth, keyHeight, "'", "\"");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, ",", "<");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, ".", ">");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "P", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "Y", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "F", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "G", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "C", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "R", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "L", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "/", "?");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "=", "+");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth * 1.5, keyHeight, "\\", "|");

	//Row3
	row=3;
	InitKeys(i++, startX = xOrigin, startY -= keyHeight + yGap, keyWidth * 2, keyHeight, "Caps Lock", NULL);
	InitKeys(i++, startX += keyWidth * 2 + xGap, startY, keyWidth, keyHeight, "A", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "O", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "E", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "U", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "I", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "D", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "H", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "T", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "N", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "S", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "-", "_");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth * 2.03, keyHeight, "Enter", NULL);

	//Row4
	row=4;
	InitKeys(i++, startX = xOrigin, startY -= keyHeight + yGap, keyWidth * 2.5, keyHeight, "Shift", NULL);
	InitKeys(i++, startX += keyWidth * 2.5 + xGap, startY, keyWidth, keyHeight, ";", ":");
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "Q", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "J", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "K", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "X", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "B", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "M", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "W", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "V", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "Z", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth * 2.55, keyHeight, "Shift", NULL);

	//Row5
	row=5;
	InitKeys(i++, startX = xOrigin, startY -= keyHeight + yGap, keyWidth * 1.5, keyHeight, "Ctrl", NULL);
	InitKeys(i++, startX += keyWidth * 1.5 + xGap, startY, keyWidth, keyHeight, "Win", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth * 1.5, keyHeight, "Alt", NULL);
	InitKeys(i++, startX += keyWidth * 1.5 + xGap, startY, keyWidth * 6, keyHeight, "Space", NULL);
	InitKeys(i++, startX += keyWidth * 6 + xGap, startY, keyWidth * 1.5, keyHeight, "Alt Gr", NULL);
	InitKeys(i++, startX += keyWidth * 1.5 + xGap, startY, keyWidth, keyHeight, "Win", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth, keyHeight, "Menu", NULL);
	InitKeys(i++, startX += keyWidth + xGap, startY, keyWidth * 1.65, keyHeight, "Ctrl", NULL);

}

/****************Function to render characters****************/
static void renderChar(int x, int y, char *string) {

	//int len = (int) strlen(string);

	glRasterPos2f(x, y);

	if (windW <= 500)
		font = GLUT_BITMAP_TIMES_ROMAN_10;
	else if (windW <= 1000)
		font = GLUT_BITMAP_HELVETICA_12;
	else if (windW <= 1200)
		font = GLUT_BITMAP_HELVETICA_18;
	else
		font = GLUT_BITMAP_TIMES_ROMAN_24;

	if (zoom < 0.6)
		font = GLUT_BITMAP_TIMES_ROMAN_10;
	else if (zoom < 0.8)
		font = GLUT_BITMAP_HELVETICA_12;

	//for (; i < len; i++) {
	//	glutBitmapCharacter(font, string[i]);
	//}
	glutBitmapString(font, string);

}

/****************Function to render keys using OpenGL selection or render mode****************/
static void Render(GLenum mode) {
	GLint i;
	float colorVal = 0.3;

	for (i = 0; i <= noOfVertices; i++) {
		if (mode == GL_SELECT) {
			glLoadName(i);
		}

		glBegin(GL_POLYGON);

		glColor3ub(colorVal, colorVal, colorVal);
		glVertex3fv(Vertex[i].v1);

		glColor3f(colorVal, colorVal, colorVal);
		glVertex3fv(Vertex[i].v2);

		glColor3ubv(Vertex[i].color);
		glVertex3fv(Vertex[i].v3);

		glColor3ubv(Vertex[i].color);
		glVertex3fv(Vertex[i].v4);

		glEnd();

		glColor3f(1, 1, 1);
		renderChar(((Vertex[i].v1[0] + Vertex[i].v2[0]) / 2) - strlen(Vertex[i].chN) * 1.5, Vertex[i].v1[1] + keyHeight * 0.2, Vertex[i].chN);
		renderChar(((Vertex[i].v1[0] + Vertex[i].v2[0]) / 2) - strlen(Vertex[i].chS) * 1.5, Vertex[i].v1[1] + keyHeight * 0.6, Vertex[i].chS);
	}

	if(zoomedKey!=-1)
	{
		glBegin(GL_POLYGON);

					glColor3f(colorVal, colorVal, colorVal);
					glVertex3fv(Vertex[zoomedKey].v1);

					glColor3f(colorVal, colorVal, colorVal);
					glVertex3fv(Vertex[zoomedKey].v2);

					glColor3ubv(Vertex[zoomedKey].color);
					glVertex3fv(Vertex[zoomedKey].v3);

					glColor3ubv(Vertex[zoomedKey].color);
					glVertex3fv(Vertex[zoomedKey].v4);

					glEnd();

					glColor3f(1, 1, 1);
					renderChar(((Vertex[zoomedKey].v1[0] + Vertex[zoomedKey].v2[0]) / 2) - strlen(Vertex[zoomedKey].chN) * 1.5, Vertex[zoomedKey].v1[1] + keyHeight * 0.5, Vertex[zoomedKey].chN);
					renderChar(((Vertex[zoomedKey].v1[0] + Vertex[zoomedKey].v2[0]) / 2) - strlen(Vertex[zoomedKey].chS) * 1.5, Vertex[zoomedKey].v1[1] + keyHeight , Vertex[zoomedKey].chS);

					zoomedKey = -1;
	}




	//Render text on output bar
	glColor3f(1, 1, 1);
	renderChar(xOrigin + 2, Vertex[0].v1[1] + keyHeight+20 , text);

	Vertex[0].color[0] = 0.0;
	Vertex[0].color[1] = 0.0;
	Vertex[0].color[2] = 0.0;

}

/****************Function to reshape window****************/
static void Reshape(int width, int height) {
	windW = width;
	windH = height;
	glViewport(0, 0, windW, windH);
	glGetIntegerv(GL_VIEWPORT, vp);

}

/****************Function to detect object under mouse click using OpenGL selection mode****************/
static GLint DoSelect(GLint x, GLint y) {
	GLint hits;

	glSelectBuffer(MAXSELECT, selectBuf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(~0);

	glPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix(x, windH - y, 4, 4, vp);
	gluOrtho2D(-175, 175, -175, 175);
	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glTranslated(moveX,moveY,0);
	glScalef(zoom, zoom, 1);

	Render(GL_SELECT);

	glPopMatrix();

	hits = glRenderMode(GL_RENDER);
	if (hits <= 0) {
		return -1;
	}
	return selectBuf[(hits - 1) * 4 + 3];
}

/****************Function to recolor key when pressed or released, to get button pressing effect****************/
static void RecolorKey(GLint key, int pressed) {
	if(key==-1)
		return;

	// pressed is 0 on mouse click and 1 for released
	if(pressed == 0){
	keyR=Vertex[key].color[0];
	keyG=Vertex[key].color[1];
	keyB=Vertex[key].color[2];
	Vertex[key].color[0]=0;
	Vertex[key].color[1]=0;
	Vertex[key].color[2]=0;
	}
	else{
		Vertex[key].color[0]=keyR;
		Vertex[key].color[1]=keyG;
		Vertex[key].color[2]=keyB;
	}

}

/****************Function to get input value of pressed key****************/
static void processInput(GLint key) {

	if(key==-1)
		return;


	if (strcmp(Vertex[key].chN, "Caps Lock") == 0){
		capsLockActive ^= 1;
		shiftActive = 0;
	}
	else if (strcmp(Vertex[key].chN, "Alt") == 0 || strcmp(Vertex[key].chN, "Alt Gr") == 0)
		altActive ^= 1;

	else if (strcmp(Vertex[key].chN, "Shift") == 0)
		shiftActive ^= 1;

	else if (strcmp(Vertex[key].chN, "Ctrl") == 0)
		ctrlActive ^= 1;

	else if (strcmp(Vertex[key].chN, "Tab") == 0){
		strcat(text,"\t");
	}
	else if (strcmp(Vertex[key].chN, "Space") == 0){
		strcat(text, " ");
	}
	else if (strcmp(Vertex[key].chN, "Backspace") == 0)
		text[strlen(text) - 1] = '\0';

	else if(strcmp(Vertex[key].chN, "Enter") == 0)
		strcat(text, "\n");
	else if ((strcmp(Vertex[key].chN, "Win") == 0) || (strcmp(Vertex[key].chN, "Menu") == 0))
		return;

	else if (Vertex[key].chN[0] >= 65 && Vertex[key].chN[0] <= 90) {
		if (capsLockActive ^ shiftActive)
			strcat(text, Vertex[key].chN);
		else {
			Vertex[key].chN[0] += 32;
			strcat(text, Vertex[key].chN);
			Vertex[key].chN[0] -= 32;
		}
		shiftActive = 0;
	}

	else {
		if (shiftActive)
			strcat(text, Vertex[key].chS);
		else
			strcat(text, Vertex[key].chN);
		shiftActive = 0;
	}

}

/****************Function to zoom selected key****************/
static void zoomInKey(GLint h) {
	if(h==-1)
		return;


	float v[2];
	float *oldV;
	GLint i;

	v[0] = Vertex[h].v1[0] + Vertex[h].v2[0] + Vertex[h].v3[0] + Vertex[h].v4[0];
	v[1] = Vertex[h].v1[1] + Vertex[h].v2[1] + Vertex[h].v3[1] + Vertex[h].v4[1];
	v[0] /= 4;
	v[1] /= 4;

	for (i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			oldV = Vertex[h].v1;
			break;
		case 1:
			oldV = Vertex[h].v2;
			break;
		case 2:
			oldV = Vertex[h].v3;
			break;
		case 3:
			oldV = Vertex[h].v4;
			break;
		}
		oldV[0] = 2 * (oldV[0] - v[0]) + v[0];
		oldV[1] = 2 * (oldV[1] - v[1]) + v[1];
		oldV[2] = 1;
	}
	zoomedKey = h;


}

/****************Function to clear zoom of selected key****************/
static void zoomOutKey(GLint h) {

	if(h==-1)
	return;

	float v[2];
	float *oldV;
	GLint i;

	v[0] = Vertex[h].v1[0] + Vertex[h].v2[0] + Vertex[h].v3[0] + Vertex[h].v4[0];
	v[1] = Vertex[h].v1[1] + Vertex[h].v2[1] + Vertex[h].v3[1] + Vertex[h].v4[1];
	v[0] /= 4;
	v[1] /= 4;

	for (i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			oldV = Vertex[h].v1;
			break;
		case 1:
			oldV = Vertex[h].v2;
			break;
		case 2:
			oldV = Vertex[h].v3;
			break;
		case 3:
			oldV = Vertex[h].v4;
			break;
		}
		oldV[0] = 0.5 * (oldV[0] - v[0]) + v[0];
		oldV[1] = 0.5 * (oldV[1] - v[1]) + v[1];
		oldV[2] = 0;
	}

}

/****************Function to detect mouse events****************/
static void Mouse(int button, int state, int mouseX, int mouseY) {
	GLint key;
	static GLint oldKeyLeft, oldKeyRight;
	if (state == GLUT_DOWN) {
		key = DoSelect((GLint) mouseX, (GLint) mouseY);
		if (key != -1 && key != 0) {
			if (button == GLUT_LEFT_BUTTON) {
				RecolorKey(key, 0);
				oldKeyLeft = key;
			} else if (button == GLUT_RIGHT_BUTTON) {
				zoomInKey(key);
				oldKeyRight = key;
			}

		}else{
			oldKeyLeft=-1;
			oldKeyRight=-1;
		}
	}

	else if (state == GLUT_UP) {
		key = DoSelect((GLint) mouseX, (GLint) mouseY);
		if (key != -1 && key != 0) {
			if (button == GLUT_LEFT_BUTTON && key == oldKeyLeft) {
				processInput(key);
				RecolorKey(key, 1);
			} else if (button == GLUT_RIGHT_BUTTON && key == oldKeyRight) {
				zoomOutKey(key);
			}
			else if (button == GLUT_LEFT_BUTTON)
						RecolorKey(oldKeyLeft, 1);
			else if(button == GLUT_RIGHT_BUTTON)
						zoomOutKey(oldKeyRight);

		}
		else if (button == GLUT_LEFT_BUTTON){
				RecolorKey(oldKeyLeft, 1);
				oldKeyLeft=-1;
		}
		else if(button == GLUT_RIGHT_BUTTON){
				zoomOutKey(oldKeyRight);
				oldKeyRight=-1;
		}
	}
	glutPostRedisplay();
}

/****************Function to draw DS keyboard****************/
static void Draw(void) {
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-175, 175, -175, 175);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.2, 0.8, 0.3, 0.0);	//Background Color
	glClear(GL_COLOR_BUFFER_BIT);
	glTranslated(moveX,moveY,0);
	glScalef(zoom, zoom, 1);			//Scaling
	Render(GL_RENDER);					//Function rendering the keyboard
	glPopMatrix();
	glutSwapBuffers();
}

/****************Function to detect Keyboard inputs****************/

static void Key(unsigned char key, int x, int y) {

	switch (key) {
	case '+':
		zoom /= 0.90;
		glutPostRedisplay();
		break;
	case '-':
		zoom *= 0.90;
		glutPostRedisplay();
		break;
	case 's':
			moveY-=2.0;
			glutPostRedisplay();
			break;
	case 'w':
			moveY+=2.0;
			glutPostRedisplay();
			break;
	case 'a':
			moveX-=2.0;
			glutPostRedisplay();
			break;
	case 'd':
			moveX+=1.0;
			glutPostRedisplay();
			break;
	case 27:
		exit(0);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Dvorak Simplified Keyboard");
	Init();
	glutReshapeWindow(windW, windH);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutMouseFunc(Mouse);
	glutDisplayFunc(Draw);
	glEnable(GL_DEPTH_TEST);
	//	  glEnable(GL_CULL_FACE);
	//	  glEnable(GL_BLEND);
	//	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	  glEnable(GL_LINE_SMOOTH);
	glShadeModel(GL_SMOOTH);
	glutMainLoop();
	return 0;
}
