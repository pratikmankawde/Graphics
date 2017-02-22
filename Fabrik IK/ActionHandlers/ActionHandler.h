/*
 * ActionHandler.h
 *
 *  Created on: 15-Feb-2017
 *      Author: PprrATeekK
 */
#include <GL/glut.h>

#include "../Controllers/ControllerFactory.h"
#include "../Controllers/Controller.h"
#include "../OperationData/OperationData.h"
#include "../OperationData/CameraOperationData.h"
#include "../OperationData/RigMotionOperationData.h"
#include "../OperationData/OperationDataFactory.h"

#include <iostream>

using namespace std;

#ifndef ACTIONHANDLERS_ACTIONHANDLER_H_
#define ACTIONHANDLERS_ACTIONHANDLER_H_

using namespace controllers;

namespace action_handlers {

class ActionHandler {

public:
	typedef enum OperationMode{
		ANIMATING,
		RIGGING,
		VIEWING
	}OperationMode;

protected:

	int mButton;
	int mKey;
	int mSpecialModifier;
	int mState;

	Controller* mController;
	ControllerFactory* mControllerFactory;
	OperationData* mOperationData;
	OperationDataFactory* mOperationDataFactory;
	OperationMode mOperationMode = OperationMode::ANIMATING;

public:

	virtual bool handleAction() { return true; };
	virtual bool revertAction() { return true; };
	virtual ~ActionHandler(){
	};

	void setOperationMode(ActionHandler::OperationMode aOperationMode) {
		mOperationMode = aOperationMode;
	};
	OperationMode getOperationMode() { return mOperationMode; }
};


} /* namespace actions_handlers */

#endif /* ACTIONHANDLERS_ACTIONHANDLER_H_ */
