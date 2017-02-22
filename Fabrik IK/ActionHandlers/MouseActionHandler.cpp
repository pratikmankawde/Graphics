#include "MouseActionHandler.h"


using namespace controllers;
using namespace operations_data;

namespace action_handlers {

bool
MouseActionHandler::handleMoveAction(int aNewPosX, int aNewPosY, int aNewPosZ) {

	mCurrentPosition[0]=aNewPosX;
	mCurrentPosition[1]=aNewPosY;
	mCurrentPosition[2]=aNewPosZ;

	mDiffPosition = mCurrentPosition - mLastPosition;

	if (mState != GLUT_DOWN) {
		return false;
	}

	switch(mButton)	{

		case GLUT_LEFT_BUTTON:

			if (mOperationMode == OperationMode::ANIMATING) {

				mController = mControllerFactory->getControllerInstance(ControllerFactory::ControllerType::RIG_MOTION_CONTROLLER);
				mOperationData = mOperationDataFactory->getInstance(OperationDataFactory::OperationDataType::RIG_OPERATION_DATA);
				mOperationData->setOpType(RigMotionOperationData::RigMotionOperationType::IK_EVAL);
				mOperationData->setData(&mCurrentPosition);
				mController->performOperation(mOperationData);

			} else if (mOperationMode == OperationMode::RIGGING) {

			} else if (mOperationMode == OperationMode::VIEWING) {
				mController = mControllerFactory->getControllerInstance(ControllerFactory::ControllerType::CAMERA_CONTROLLER);
				mOperationData = mOperationDataFactory->getInstance(OperationDataFactory::OperationDataType::CAMERA_OPERATION_DATA);
				mOperationData->setOpType(CameraOperationData::CameraOperationType::ROTATE);
				mOperationData->setData(&mDiffPosition);
				mController->performOperation(mOperationData);
			}

			break;

		case GLUT_MIDDLE_BUTTON:

			if (mOperationMode == OperationMode::ANIMATING) {

			} else if (mOperationMode == OperationMode::RIGGING) {

			} else if (mOperationMode == OperationMode::VIEWING) {

				mController = mControllerFactory->getControllerInstance(ControllerFactory::ControllerType::CAMERA_CONTROLLER);
				mOperationData = mOperationDataFactory->getInstance(OperationDataFactory::OperationDataType::CAMERA_OPERATION_DATA);
				mOperationData->setOpType(CameraOperationData::CameraOperationType::TRANSLATE);
				mOperationData->setData(&mDiffPosition);
				mController->performOperation(mOperationData);
			}

			break;

		case GLUT_RIGHT_BUTTON:

			if (mOperationMode == OperationMode::ANIMATING) {

			} else if (mOperationMode == OperationMode::RIGGING) {

			} else if (mOperationMode == OperationMode::VIEWING) {

				mController = mControllerFactory->getControllerInstance(ControllerFactory::ControllerType::CAMERA_CONTROLLER);
				mOperationData = mOperationDataFactory->getInstance(OperationDataFactory::OperationDataType::CAMERA_OPERATION_DATA);
				mOperationData->setOpType(CameraOperationData::CameraOperationType::ZOOM);
				mOperationData->setData(&mDiffPosition);
				mController->performOperation(mOperationData);
			}
			break;

		default:
			break;
	}

	mLastPosition = mCurrentPosition;

	return true;
}


bool
MouseActionHandler::handleClickAction(int aButton, int aState, int aPosX, int aPosY, int aPosZ) {

	mCurrentPosition[0]=aPosX;
	mCurrentPosition[1]=aPosY;
	mCurrentPosition[2]=aPosZ;

	mState = aState;

	switch(aButton)	{

		case GLUT_LEFT_BUTTON:
			mButton = ((aState == GLUT_DOWN) ? GLUT_LEFT_BUTTON : 0);

			if (mOperationMode == OperationMode::ANIMATING) {
				mController = mControllerFactory->getControllerInstance(ControllerFactory::ControllerType::RIG_MOTION_CONTROLLER);
				mOperationData = mOperationDataFactory->getInstance(OperationDataFactory::OperationDataType::RIG_OPERATION_DATA);
				mOperationData->setOpType(RigMotionOperationData::RigMotionOperationType::IK_EVAL);
				mOperationData->setData(&mCurrentPosition);
				mController->performOperation(mOperationData);

			} else if (mOperationMode == OperationMode::RIGGING) {

			} else if (mOperationMode == OperationMode::VIEWING) {

			}
			break;

		case GLUT_MIDDLE_BUTTON:
			mButton = ((aState == GLUT_DOWN) ? GLUT_MIDDLE_BUTTON : 0);
			break;

		case GLUT_RIGHT_BUTTON:
			mButton = ((aState == GLUT_DOWN) ? GLUT_RIGHT_BUTTON : 0);
			break;

		default:
			mButton = 0;
			break;
	}

	mLastPosition = mCurrentPosition;
	return true;
}

}
