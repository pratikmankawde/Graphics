/*
 * JointModel.h
 *
 *  Created on: 17-Feb-2017
 *      Author: root
 */
#include "../Elements/Joint.h"
#include "Model.h"

#ifndef JOINTMODEL_H_
#define JOINTMODEL_H_

using namespace elements;
namespace models {

class JointModel: public models::Model {

private:
	Vec3<GLfloat> mPosition;
	Vec3<GLfloat> mApex;
	GLfloat mRadius;
	GLfloat mLenght;
	GLint mStacks;
	GLint mSlices;
	Joint* mJoint;

public:
	JointModel();
	JointModel(Joint* aJoint);
	virtual ~JointModel();

	void generateVertices();
	void drawModel();
	void drawCustomJoint();
};

} /* namespace operations */
#endif /* JOINTMODEL_H_ */
