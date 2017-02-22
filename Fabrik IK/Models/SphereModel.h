/*
 * SphereModel.h
 *
 *  Created on: 15-Feb-2017
 *      Author: PprrATeekK
 */

#include "Model.h"

#ifndef MODELS_SPHEREMODEL_H_
#define MODELS_SPHEREMODEL_H_

namespace models {

class SphereModel : public Model {

	GLdouble mRadius;
	GLint mSlices, mStacks;

public:
	SphereModel(GLdouble aRadius, GLint aSlices, GLint aStacks) : mRadius(aRadius), mSlices(aSlices), mStacks(aStacks) {};
	void generateVertices();
	virtual ~SphereModel();
	void drawModel();
	void setSize(int aRadius) { mRadius = aRadius; };
};

} /* namespace models */

#endif /* MODELS_SPHEREMODEL_H_ */
