#include "../Elements/Vec3.h"
#include "HierarchyModel.h"
#include "JointModel.h"

using namespace elements;

namespace models {

void
HierarchyModel::generateVertices() {


	clear(CLEAR_TYPE::BOTH_VERTICES_AND_NORMALS);
	const float offset = 1.0f;

	for (size_t jointIndex = 0; jointIndex < mHierarchy->getJointCount() - 1; ++jointIndex) {

		Vec3<float> position = mHierarchy->getJointAtIndex(jointIndex)->getPosition();

		GLfloat* posArr = position.getArray();
		GLfloat* apexFloat = mHierarchy->getJointAtIndex(jointIndex+1)->getPosition().getArray();

		addVertex(Vec3<float>(posArr[0]+offset,posArr[1],posArr[2]));
		addVertex(Vec3<float>(posArr[0]-offset,posArr[1],posArr[2]));
		addVertex(Vec3<float>(apexFloat));
	}
}

void
HierarchyModel::drawModel() {

	//float offset = 1.0f;
	Joint* joint;
	for (size_t jointIndex = 0; jointIndex < mHierarchy->getJointCount() - 1 ; ++jointIndex) {
//		Vec3<GLfloat> position = mHierarchy->getJointAtIndex(jointIndex)->getPosition();
//		GLfloat* posFloat = position.getArray();
//		GLfloat* apexFloat = mHierarchy->getJointAtIndex(jointIndex+1)->getPosition().getArray();
//		glColor3f(red*jointIndex,green*jointIndex,blue*jointIndex);
//		glColor3fv(mColor.getArray());
//		glBegin(GL_TRIANGLES);
//			glVertex3f(posFloat[0]+offset, posFloat[1], posFloat[2]);
//			glVertex3f(posFloat[0]-offset, posFloat[1], posFloat[2]);
//			glVertex3fv(apexFloat);
//		glEnd();
		joint = mHierarchy->getJointAtIndex(jointIndex);
		joint->setApex(mHierarchy->getJointAtIndex(jointIndex+1)->getPosition());
		JointModel* jm = new JointModel(joint);
		jm->drawModel();
	}

}
}
