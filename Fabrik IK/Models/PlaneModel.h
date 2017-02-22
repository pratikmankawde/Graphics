/*
 * PlaneModel.h
 *
 *  Created on: 20-Feb-2017
 *      Author: root
 */

#ifndef PLANEMODEL_H_
#define PLANEMODEL_H_

#include "Model.h"

namespace models {

class PlaneModel: public models::Model {

private:
	/* Make them weak pointer */
	GLfloat mScreenWidth;
	GLfloat mScreenHeight;
	Vec3<GLfloat> mCameraViewPoint;
	Vec3<GLfloat> mNormalVec;
	GLfloat mAngle;
	bool isDrawn;

public:
	PlaneModel();
	virtual ~PlaneModel();

	void generateVertices();
	void drawModel();
	void drawLine();
	void drawModel(GLfloat aScreenWidth, GLfloat aScreenHeight, Vec3<GLfloat> aCameraViewPoint);
	void updateSelectionPlane();
};

} /* namespace operations */
#endif /* PLANEMODEL_H_ */
