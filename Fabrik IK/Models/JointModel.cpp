/*
 * JointModel.cpp
 *
 *  Created on: 17-Feb-2017
 *      Author: root
 */

#include "JointModel.h"
#include <math.h>

namespace models {

JointModel::JointModel() {
	mPosition.setVec3(-5.0, 0.0, 0.0);
	mApex.setVec3(0.0, 10.0, 0.0);
	mRadius = 1.0f;
	mLenght = 2*mRadius;
	mStacks = 5;
	mSlices = 10;
	mJoint = nullptr;
}

JointModel::JointModel(Joint* aJoint) {
	mJoint = aJoint;
	mPosition = mJoint->getPosition();
	mApex = mJoint->getApex();
	mLenght = (mApex-mPosition).getLenght();
	mRadius = mLenght / 4.0f;
	mStacks = 5;
	mSlices = 10;
}

void
JointModel::generateVertices() {

}

void
JointModel::drawModel() {

	Vec3<GLfloat>  directionVec = mApex - mPosition;
	Vec3<GLfloat> modelVec(0.0f, mLenght+mRadius, 0.0f);
	modelVec.normalize();
	directionVec.normalize();
	Vec3<GLfloat> normalVec = modelVec.crossProduct(directionVec);

	GLfloat angle = modelVec.getAngle(directionVec);

	glPushMatrix();
		glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
		glRotatef(angle,normalVec.x(),normalVec.y(),normalVec.z());
		drawCustomJoint();
//		glPushMatrix();
//			glColor3f(0.2,0.6,0.8);
//			glRotatef(-90,1,0,0);
//			glutWireCone(mRadius, jointLenght ,mSlices, 0);
//		glPopMatrix();
		glPushMatrix();
			glColor3f(0.8,0.2,0.2);
			glRotatef(90,1,0,0);
			glutWireSphere(mRadius, mSlices, mStacks);
		glPopMatrix();
	glPopMatrix();

}

void
JointModel::drawCustomJoint() {

	glColor3f(0.8,0.8,0.8);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(mRadius, mRadius, 0.0f);
		glVertex3f(0.0, mLenght, 0.0f);
	glEnd();

	glColor3f(0.8,0.8,0.8);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-mRadius, mRadius, 0.0f);
		glVertex3f(0.0, mLenght, 0.0f);
	glEnd();
	glColor3f(0.6,0.6,0.6);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, mRadius, mRadius);
		glVertex3f(0.0, mLenght, 0.0f);
	glEnd();
	glColor3f(0.6,0.6,0.6);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, mRadius, -mRadius);
		glVertex3f(0.0, mLenght, 0.0f);
	glEnd();

	glColor3f(0.7,0.7,0.7);
	glBegin(GL_QUADS);
		glVertex3f(mRadius, mRadius, 0.0);
		glVertex3f(0.0, mRadius, mRadius);
		glVertex3f(-mRadius, mRadius, 0.0);
		glVertex3f(0.0, mRadius, -mRadius);
	glEnd();
}

JointModel::~JointModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace operations */
