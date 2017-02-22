/*
 * ControllerFactory.cpp
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#include "ControllerFactory.h"

namespace controllers {

ControllerFactory::~ControllerFactory() {
	// TODO Auto-generated destructor stub
}

Controller*
ControllerFactory::getControllerInstance(ControllerType aControllerType) {


	switch(aControllerType) {

		case CAMERA_CONTROLLER:
			return mCameraController;
			break;

		case RIG_MOTION_CONTROLLER:
			return mRigMotionController;
			break;

		default:
			return nullptr;
			break;

	}

}


} /* namespace controllers */
