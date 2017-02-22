#include "../Elements/Vec3.h"
#include "../Models/Model.h"
#include <GL/glut.h>


#ifndef CAMERA_H_
#define CAMERA_H_

using namespace elements;
using namespace models;

namespace display {

class Camera {

private:

	GLfloat mFieldOfView;
	GLfloat mFar;
	GLfloat mNear;
	GLfloat mFocusDistance;

	Vec3<GLfloat> mPosition;
	Vec3<GLfloat> mUpVector;
	Vec3<GLfloat> mViewPoint;
	Vec3<GLfloat> mRotation;

public:

	typedef enum AXIS {
		X_AXIS,
		Y_AXIS,
		Z_AXIS
	}AXIS;

	Camera();

	Vec3<GLfloat> getCameraPosition() const { return mPosition; };
	void setCameraPosition(Vec3<GLfloat> aPosition) { mPosition = aPosition; };

	Vec3<GLfloat> getCameraViewPoint() const {return mViewPoint; };
	void setCameraViewPoint(Vec3<GLfloat> aViewPoint) { mViewPoint = aViewPoint; };

	Vec3<GLfloat> getCameraUpVector() const {return mUpVector; };
	void setCameraUpVector(Vec3<GLfloat> aUpVector) { mUpVector = aUpVector; };

	Vec3<GLfloat> getCameraRotation() const {return mRotation; };

	void frameModel(Model& aModel);
	void resetToOrigin();

	void rotateBy(Vec3<GLfloat> aAngle, AXIS aAxis);
	void rotateBy(Vec3<GLfloat> aAngle);
	void translateBy(Vec3<GLfloat> aAmount);
	void zoom(GLfloat aAmount);

	GLfloat getFar() const {
		return mFar;
	}

	void setFar(GLfloat far) {
		mFar = far;
	}

	GLfloat getFieldOfView() const {
		return mFieldOfView;
	}

	void setFieldOfView(GLfloat fieldOfView) {
		mFieldOfView = fieldOfView;
	}

	GLfloat getFocusDistance() const {
		return mFocusDistance;
	}

	void setFocusDistance(GLfloat focusDistance) {
		mFocusDistance = focusDistance;
	}

	GLfloat getNear() const {
		return mNear;
	}

	void setNear(GLfloat near) {
		mNear = near;
	}
};

}

#endif
