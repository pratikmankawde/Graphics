/*
 * SphereModel.cpp
 *
 *  Created on: 15-Feb-2017
 *      Author: PprrATeekK
 */

#include "SphereModel.h"

namespace models {

void
SphereModel::generateVertices() {

}

void
SphereModel::drawModel() {
	glPushMatrix();
		glColor3f(1.0,1.0,1.0);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glRotatef(90,1,0,0);
		glutSolidSphere(mRadius, mSlices, mStacks);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	glPopMatrix();
}

SphereModel::~SphereModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace models */
