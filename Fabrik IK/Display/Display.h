
#ifndef DISPLAY_H_
#define DISPLAY_H_


#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include "../ActionHandlers/KeyboardActionHandler.h"
#include "../ActionHandlers/MouseActionHandler.h"
#include "../Elements/Hierarchy.h"
#include "../Elements/Rig.h"
#include "../Elements/Vec3.h"
#include "../Models/HierarchyModel.h"
#include "../Models/Model.h"
#include "../Models/PlaneModel.h"

#include "Camera.h"

using namespace std;
using namespace models;
using namespace action_handlers;

namespace display {

class Display {

private:
	Vec3<GLfloat> mColor;
	GLint mScreenWidth, mScreenHeight;

	GLint mLastMouseX;
	GLint mLastMouseY;
	Vec3<GLfloat> mWorldXYZ;
	GLfloat depthvalue;
	// Models to draw
	vector<Model*> mModels;
	PlaneModel* mSelectionPlane;
	bool isSelectionPlaneDrawn;
	Camera mCamera;

	// State matrices
	GLint mViewport[4];
	GLdouble mModelview[16];
	GLdouble mProjection[16];

	void drawFloor();
	void renderModels();
	void drawDestination();

	MouseActionHandler* mMouseActionHandler;
	KeyboardActionHandler* mKeyboardActionHandler;

public:
	Display() {
		mScreenWidth = 1280;
		mScreenHeight = 720;
		mLastMouseX=0;
		mLastMouseY=0;
		mWorldXYZ.setVec3(0.0f);
		mMouseActionHandler = new MouseActionHandler;
		mKeyboardActionHandler = new KeyboardActionHandler;
		isSelectionPlaneDrawn = false;
	}

	void initData(Hierarchy* hier);
	void setTranslation(GLfloat xOffet, GLfloat yOffet, GLfloat zOffet);
	void setRotation(GLfloat xAngle, GLfloat yAngle, GLfloat zAngle);

	void setColor(GLfloat aR, GLfloat aG, GLfloat aB);
	void initDisplay(int argc, char **argv);
	void setupScene();
	void updateScene();
	void renderScene();
	void changeSize(GLint windowWidth, GLint windowHeight);

	void addModel(Model* aModel);
	void drawSelectionPlane(int aNewPosX, int aNewPosY);

	Vec3<GLfloat> screenToWorld(GLint mouseX, GLint mouseY);
	Vec3<GLfloat> getZCorrd(Vec3<GLdouble> nearVertex, Vec3<GLdouble> farVertex);
	const Camera& getCamera() const {
		return mCamera;
	}

	void setCamera(const Camera& camera) {
		mCamera = camera;
	}

	GLint getScreenWidth() const {
		return mScreenWidth;
	}

	void setScreenWidth(GLint screenWidth) {
		mScreenWidth = screenWidth;
	}

	GLint getScreenHeight() const {
		return mScreenHeight;
	}

	void setScreenHeight(GLint screenHeight) {
		mScreenHeight = screenHeight;
	}

	PlaneModel* getSelectionPlane() {
		return mSelectionPlane;
	}
};

}

#endif
