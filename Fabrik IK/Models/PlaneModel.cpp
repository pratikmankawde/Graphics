/*
 * PlaneModel.cpp
 *
 *  Created on: 20-Feb-2017
 *      Author: root
 */

#include "PlaneModel.h"
#include <iostream>

using namespace std;
namespace models {

PlaneModel::PlaneModel(){

}

PlaneModel::~PlaneModel() {
	// TODO Auto-generated destructor stub
}

void
PlaneModel::generateVertices() {

}

void
PlaneModel::drawModel(GLfloat aScreenWidth, GLfloat aScreenHeight, Vec3<GLfloat> aCameraViewPoint) {

	mScreenWidth = aScreenWidth;
	mScreenHeight = aScreenHeight;
	mCameraViewPoint = aCameraViewPoint;
	//drawLine();
	drawModel();
}

void
PlaneModel::drawModel() {

	glPushMatrix();
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glColor4f(1.0,1.0,1.0,1.0);
		glTranslatef(mCameraViewPoint.x(),mCameraViewPoint.y(), mCameraViewPoint.z());
		glRotatef(mAngle,mNormalVec.x(),mNormalVec.y(),mNormalVec.z());
		float x = mScreenWidth/4.0;
		float y = mScreenHeight/4.0;
		glBegin(GL_QUADS);
			glVertex3f(x, y, 0.0);
			glVertex3f(x, -y, 0.0);
			glVertex3f(-x, -y, 0.0);
			glVertex3f(-x, y, 0.0);
		glEnd();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glPopMatrix();
	isDrawn = true;
	glutPostRedisplay();
}


void
PlaneModel::drawLine() {
	GLint mViewport[4];
	glGetIntegerv(GL_VIEWPORT, mViewport);
	GLfloat screenX = (GLfloat)mCameraViewPoint.x();
	GLfloat	screenY = (GLfloat)mViewport[1] + (GLfloat)mViewport[3] - (GLfloat)mCameraViewPoint.y();  // Subtract The Current Mouse Y Coordinate
	GLdouble posX, posY, posZ;
	GLdouble mModelview[16];
	GLdouble mProjection[16];
	Vec3<GLdouble> nearVertex, farVertex;

	glGetDoublev(GL_MODELVIEW_MATRIX, mModelview);
	glGetDoublev(GL_PROJECTION_MATRIX, mProjection);

	gluUnProject(screenX, screenY, 0.0f, mModelview, mProjection, mViewport, &posX, &posY, &posZ);
	nearVertex[0]=posX;
	nearVertex[1]=posY;
	nearVertex[2]=posZ;
	//cout<<"x:"<<nearVertex[0] <<" y:"<< nearVertex[1] <<" z:"<<nearVertex[2]<<endl;
	gluUnProject(screenX, screenY, 1.0f, mModelview, mProjection, mViewport, &posX, &posY, &posZ);
	farVertex[0]=posX;
	farVertex[1]=posY;
	farVertex[2]=posZ;
	Vec3<GLdouble> axis, direction(farVertex-nearVertex), model(0.0,0.0,-1.0);
	double angle;

	nearVertex.getAngleAndAxisOfRotation(direction, model, &angle, &axis);

	//cout<<"x:"<<farVertex[0] <<" y:"<< farVertex[1] <<" z:"<<farVertex[2]<<endl;
	glPushMatrix();
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		//glTranslatef(-10.0,0.0,0.0);
		glRotated(angle+90, axis[0],axis[1],axis[2]);
		glScalef(100.0, 10.0, 0.0);
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_QUADS);
			glVertex3f(1.0, 1.0, 0.0);
			glVertex3f(1.0, -1.0, 0.0);
			glVertex3f(-1.0, -1.0, 0.0);
			glVertex3f(-1.0, 1.0, 0.0);
		glEnd();
		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glPopMatrix();
}
void
PlaneModel::updateSelectionPlane() {
	GLdouble mModelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mModelview);
	Vec3<GLfloat>  directionVec(mModelview[2], mModelview[6], mModelview[10]);
	Vec3<GLfloat> modelVec(0.0f, 0.0, 1.0f);
	modelVec.getAngleAndAxisOfRotation(modelVec, directionVec, &mAngle, &mNormalVec);
}

} /* namespace operations */
