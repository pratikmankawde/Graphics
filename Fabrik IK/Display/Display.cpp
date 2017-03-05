/*
 * main.cpp
 *
 *  Created on: 06-Feb-2017
 *      Author: PprrATeekK
 */

#include <iostream>
#include "Display.h"

using namespace std;
using namespace models;
using namespace action_handlers;

namespace display {

Display* sDisplay;
MouseActionHandler* sMouseActionHandler;
KeyboardActionHandler* sKeyboardActionHandler;
bool moving = false;
void displayFunction() {

	sDisplay->renderScene();
}

void reshapeFunction(int windowWight, int windowsHeight) {

	sDisplay->changeSize(windowWight, windowsHeight);
}

void mouseMovementHandler(int aNewPosX, int aNewPosY) {

	Vec3<GLfloat> worldCord;
	if (sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::ANIMATING) {
		sDisplay->drawSelectionPlane(aNewPosX, aNewPosY);
		worldCord = sDisplay->screenToWorld(aNewPosX, aNewPosY);
		sMouseActionHandler->setOperationMode(ActionHandler::OperationMode::ANIMATING);

	} else if (sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::RIGGING) {
		worldCord = sDisplay->screenToWorld(aNewPosX, aNewPosY);
		sMouseActionHandler->setOperationMode(ActionHandler::OperationMode::RIGGING);

	} else if (sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::VIEWING) {
		worldCord.setVec3(aNewPosX, aNewPosY, 0.0);
		sMouseActionHandler->setOperationMode(ActionHandler::OperationMode::VIEWING);
	}

	sMouseActionHandler->handleMoveAction(worldCord.x(), worldCord.y(), worldCord.z());
	glutPostRedisplay();
}

void mouseClickHandler(int aButton, int aState, int aPosX, int aPosY) {

	Vec3<GLfloat> worldCord;
	if (sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::ANIMATING) {
		sDisplay->drawSelectionPlane(aPosX, aPosY);
		worldCord = sDisplay->screenToWorld(aPosX, aPosY);
		sMouseActionHandler->setOperationMode(ActionHandler::OperationMode::ANIMATING);

	} else if (sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::RIGGING) {
		worldCord = sDisplay->screenToWorld(aPosX, aPosY);
		sMouseActionHandler->setOperationMode(ActionHandler::OperationMode::RIGGING);

	} else if (sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::VIEWING) {
		worldCord.setVec3(aPosX, aPosY, 0.0);
		sMouseActionHandler->setOperationMode(ActionHandler::OperationMode::VIEWING);
	}

	sMouseActionHandler->handleClickAction(aButton, aState, worldCord.x(), worldCord.y(), worldCord.z());
	glutPostRedisplay();

	if(aState == GLUT_UP && sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::VIEWING) {
		sDisplay->getSelectionPlane()->updateSelectionPlane();
	}
	if(aState == GLUT_UP && sKeyboardActionHandler->getOperationMode() == ActionHandler::OperationMode::ANIMATING) {
		moving =false;
	}

}

void keyPressHandler(unsigned char aKey, int aMousePosX, int aMousePosY){
	//Vec3<GLfloat> worldCord = sDisplay->screenToWorld(aPosX, aPosY); might need to uncomment this to use proper positions
	int flag = glutGetModifiers();
	if (flag & GLUT_ACTIVE_SHIFT) {
		sKeyboardActionHandler->setOperationMode(ActionHandler::OperationMode::ANIMATING);
	} else if (flag & GLUT_ACTIVE_ALT) {
		sKeyboardActionHandler->setOperationMode(ActionHandler::OperationMode::RIGGING);
	} else if (flag & GLUT_ACTIVE_CTRL) {
		sKeyboardActionHandler->setOperationMode(ActionHandler::OperationMode::VIEWING);
	} else {
		sKeyboardActionHandler->setOperationMode(ActionHandler::OperationMode::ANIMATING);
	}
	sKeyboardActionHandler->handleNormalKeyPressEvent(aKey, aMousePosX, aMousePosY);
	glutPostRedisplay();
}

void
Display::addModel(Model* aModel) {
	mModels.push_back(aModel);
}

Vec3<GLfloat>
Display::screenToWorld(GLint mouseX, GLint mouseY) {

	GLint screenX, screenY;
	GLdouble posX, posY, posZ;

	Vec3<GLfloat> nearVertex, farVertex;

	glGetDoublev(GL_MODELVIEW_MATRIX, mModelview);
	glGetDoublev(GL_PROJECTION_MATRIX, mProjection);
	glGetIntegerv(GL_VIEWPORT, mViewport);

	screenX = mouseX;
	screenY = mViewport[1] + mViewport[3] - mouseY;  // Subtract The Current Mouse Y Coordinate

	glReadPixels(screenX, screenY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthvalue);//Reads the depth buffer

//	gluUnProject(screenX, screenY, 0.0f, mModelview, mProjection, mViewport, &posX, &posY, &posZ);
//	nearVertex[0]=posX;
//	nearVertex[1]=posY;
//	nearVertex[2]=posZ;
//
//	gluUnProject(screenX, screenY, 1, mModelview, mProjection, mViewport, &posX, &posY, &posZ);
//	farVertex[0]=posX;
//	farVertex[1]=posY;
//	farVertex[2]=posZ;
//
//	if(nearVertex[2] == farVertex[2])     // this means we have no solutions
//	   return Vec3<GLfloat>(0.0f);
//
//	GLdouble t = (nearVertex[2] - screenZ) / (nearVertex[2] - farVertex[2]);
//
//	// so here are the desired (x, y) coordinates
//	mWorldXYZ[0] = nearVertex[0] + (farVertex[0] - nearVertex[0]) * t,
//	mWorldXYZ[1] = nearVertex[1] + (farVertex[1] - nearVertex[1]) * t;
//
////	mWorldXYZ = getZCorrd(nearVertex, farVertex);
//
//	mWorldXYZ[2] = screenZ;

	gluUnProject( screenX, screenY, depthvalue, mModelview, mProjection, mViewport, &posX, &posY, &posZ);

	mWorldXYZ[0] = posX;
	mWorldXYZ[1] = posY;
	mWorldXYZ[2] = posZ;

//	cout<<"x:"<<mWorldXYZ[0] <<" y:"<< mWorldXYZ[1] <<" z:"<<mWorldXYZ[2]<<endl;

	return mWorldXYZ;
}

Vec3<GLfloat>
Display::getZCorrd(Vec3<GLdouble> nearVertex, Vec3<GLdouble> farVertex) {

	/* Vertices of plane */
	Vec3<GLdouble> vec1(0.0,1.0,0.0);
	Vec3<GLdouble> vec2(-1.0, -1.0, 0.0);
	Vec3<GLdouble> vec3(1.0, -1.0, 0.0);

	vec1.transformVertex(mModelview);
	cout<<"TVec x:"<<vec1[0] <<" y:"<< vec1[1] <<" z:"<<vec1[2]<<endl;
	vec2.transformVertex(mModelview);
	cout<<"TVec x:"<<vec2[0] <<" y:"<< vec2[1] <<" z:"<<vec2[2]<<endl;
	vec3.transformVertex(mModelview);
	cout<<"TVec x:"<<vec3[0] <<" y:"<< vec3[1] <<" z:"<<vec3[2]<<endl;

	Vec3<GLdouble> planeCoeficients;
	GLdouble offset;
	planeCoeficients.setPlane(vec1, vec1, vec1, &offset);
	cout<<"Plane x:"<<planeCoeficients[0] <<" y:"<< planeCoeficients[1] <<" z:"<<planeCoeficients[2]<<" offset:"<<offset<<endl;
	Vec3<GLdouble> aSlopes = farVertex-nearVertex;
	Vec3<GLfloat> intersectionPoint = planeCoeficients.getIntersectionPointwithLine(nearVertex, aSlopes, offset);
	return intersectionPoint;
}

void
Display::drawFloor() {

	GLfloat floorDepth = 0.0f;
	unsigned int cellSize = 8;

	glPushMatrix();
		GLuint gridSize = mScreenWidth/(cellSize*cellSize);
		float gridCenter = (gridSize * cellSize) / 2;
		glTranslatef(-gridCenter,-10.0f,-gridCenter);

		glBegin(GL_QUADS);
			for (GLuint x =0;x<gridSize;++x) {
				for (GLuint z =0;z<gridSize;++z)
				{
					if ((x+z)&0x00000001) //modulo 2
						glColor3f(1.0f,1.0f,1.0f); //white
					else
						glColor3f(0.3f,0.3f,0.3f); //black

					GLfloat vertexX = x*cellSize, vertexZ = z*cellSize;
					glVertex3f(vertexX, floorDepth, vertexZ);
					glVertex3f(vertexX+cellSize, floorDepth, vertexZ);
					glVertex3f(vertexX+cellSize, floorDepth, vertexZ+cellSize);
					glVertex3f(vertexX, floorDepth, vertexZ+cellSize);

				}
			}
		glEnd();
	glPopMatrix();

}

void
Display::drawSelectionPlane(int aNewPosX, int aNewPosY) {
	//Vec3<float>((float)aNewPosX, (float)aNewPosY, 0.0)
	if(!moving)
	mSelectionPlane->drawModel(mScreenWidth, mScreenHeight,mCamera.getCameraViewPoint());
	moving = true;
}

void
Display::drawDestination() {
	glPushMatrix();
		glColor3f(1.0,1.0,1.0);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glTranslatef(mWorldXYZ[0], mWorldXYZ[1], mWorldXYZ[2]);
		glRotatef(90.0,1,0,0);
		glutSolidSphere(0.5,10,10);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	glPopMatrix();
}

void
Display::renderModels() {

	drawDestination();
	if(moving) {
	mSelectionPlane->drawModel(mScreenWidth, mScreenHeight,mCamera.getCameraViewPoint());
	}
	for (Model* model : mModels) {
		model->drawModel();
	}
}

void
Display::changeSize(GLint windowWidth, GLint windowHeight) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (windowHeight == 0)
		windowHeight = 1;

	mScreenWidth=windowWidth;
	mScreenHeight=windowHeight;

	float ratio =  windowWidth * 1.0 / windowHeight;
	// Set the viewport to be the entire window
	glViewport(0, 0, windowWidth, windowHeight);

	// Use the Projection Matrix mode
	glMatrixMode(GL_PROJECTION);

    // Reset Matrix
	glLoadIdentity();

	// Set the correct perspective.
	gluPerspective(mCamera.getFieldOfView(), ratio, mCamera.getNear(), mCamera.getFar());

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(mCamera.getCameraPosition().x(), mCamera.getCameraPosition().y(), mCamera.getCameraPosition().z(),
				mCamera.getCameraViewPoint().x(), mCamera.getCameraViewPoint().y(),  mCamera.getCameraViewPoint().z(),
				mCamera.getCameraUpVector().x(), mCamera.getCameraUpVector().y(),  mCamera.getCameraUpVector().z());

}


void
Display::renderScene() {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	glLoadIdentity();

	// Set the camera
	//glTranslatef(0, 0, -mCamera.getFocusDistance());
	glTranslatef(mCamera.getCameraPosition().x(), mCamera.getCameraPosition().y(), -mCamera.getFocusDistance());
	glRotatef(mCamera.getCameraRotation().x(), 1, 0, 0);
	glRotatef(mCamera.getCameraRotation().y(), 0, 1, 0);

	drawFloor();
	renderModels();
	glutSwapBuffers();
	glFlush();
}

void
Display::setupScene() {
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Fabrik IK Solver");
	glClearColor(0.0,0.0,0.09,0.0);

    glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_MULTISAMPLE);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	// register callbacks
	glutDisplayFunc(displayFunction);
	glutReshapeFunc(reshapeFunction);
	glutIdleFunc(displayFunction);

	glutMouseFunc(mouseClickHandler);
	glutMotionFunc(mouseMovementHandler);
	glutKeyboardFunc(keyPressHandler);
	//glutSpecialFunc(processSpecialKeys);
}

void
Display::initDisplay(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	sDisplay = this;
	sMouseActionHandler = this->mMouseActionHandler;
	sKeyboardActionHandler = this->mKeyboardActionHandler;
	setupScene();
	glutMainLoop();
	// enter GLUT event processing cycle
}

void
Display::initData(Hierarchy* hier) {
	Rig* rig = new Rig();
	rig->addHierarchy(hier);
	ControllerFactory* controllerFactory =  new ControllerFactory(&mCamera, rig);
	OperationDataFactory* operationDataFactory = new OperationDataFactory();
	mMouseActionHandler->setControllerFactory(controllerFactory);
	mMouseActionHandler->setOperationDataFactory(operationDataFactory);
	mSelectionPlane = new PlaneModel();
}


}
