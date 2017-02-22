/*
 * RigMotionController.cpp
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#include "RigMotionController.h"

namespace controllers {

bool
RigMotionController::performOperation(OperationData* aOperationData) {
	mFabrikEvaluator->setActiveHierarchy(mRig->getHierarchy());

	if (aOperationData->getOpType() == RigMotionOperationData::RigMotionOperationType::IK_EVAL) {
		return mFabrikEvaluator->evaluate(aOperationData->getData());
	} else if (aOperationData->getOpType() == RigMotionOperationData::RigMotionOperationType::FK_EVAL) {
		return true;
	} else {
		return false;
	}
}

} /* namespace controllers */
