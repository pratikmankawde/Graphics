/*
 * ControllerFactory.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#include "CameraController.h"
#include "Controller.h"
#include "RigMotionController.h"

#ifndef CONTROLLERFACTORY_H_
#define CONTROLLERFACTORY_H_

namespace controllers {

class ControllerFactory {

private:
	CameraController* mCameraController;
	RigMotionController* mRigMotionController;

public:
	ControllerFactory(Camera* aCamera, Rig* aRig) {
		mCameraController = new CameraController(aCamera);
		mRigMotionController= new RigMotionController(aRig);
	}

	virtual ~ControllerFactory();

	typedef enum ControllerType {
		CAMERA_CONTROLLER,
		RIG_MOTION_CONTROLLER
	}ControllerType;

	Controller* getControllerInstance(ControllerType aControllerType);
};

} /* namespace controllers */
#endif /* CONTROLLERFACTORY_H_ */
