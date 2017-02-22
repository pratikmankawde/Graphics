#include "Camera.h"

using namespace elements;
using namespace models;

namespace display {

Camera::Camera() : mFieldOfView(45.0f),
			mFar(500.0f),
			mNear(1.0f),
			mFocusDistance(50.0f),
			mPosition(0.0f,0.0f,mFocusDistance),
			mUpVector(0.0f,1.0f,0.0f),
			mViewPoint(0.0f,0.0f,0.0f),
			mRotation(0.0f,0.001f,0.0f){};

void
Camera::frameModel(Model& aModel) {

	mViewPoint = aModel.getCenter();
	mFocusDistance = aModel.getDepth();
	mPosition.setVec3(0.0);
	mPosition[2] = mViewPoint[2] + mFocusDistance;
}

void
Camera::resetToOrigin() {

	mPosition = 0.0f;
	mFocusDistance = -50.0f;
	mUpVector.setVec3(0.0f,1.0f,0.0f);
	mViewPoint.setVec3(0.0f,0.0f,mFocusDistance);
}

void
Camera::rotateBy(Vec3<GLfloat> aAngle, AXIS aAxis) {

}

void
Camera::rotateBy(Vec3<GLfloat> aAngle) {

	mRotation[0] += (float) 0.5f * aAngle.y();
	mRotation[1] += (float) 0.5f * aAngle.x();
}

void
Camera::translateBy(Vec3<GLfloat> aAmount) {

	mPosition[0] += (float) 0.01f * aAmount.x();
	mPosition[1] -= (float) 0.01f * aAmount.y();
	//mPosition[2] += (float) 0.5f * aAmount.z();

	mViewPoint[2] = mPosition[2] - mFocusDistance;
}

void
Camera::zoom(GLfloat aAmount) {

	mFocusDistance -= ((float) 0.5f * aAmount);
	//mPosition[2] = mViewPoint[2] + mFocusDistance;
}

}
