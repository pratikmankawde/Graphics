/*
 * CameraController.cpp
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#include "CameraController.h"
#include "../OperationData/CameraOperationData.h"

using namespace operations_data;

namespace controllers {

CameraController::~CameraController() {
	// TODO Auto-generated destructor stub
}

bool
CameraController::performOperation(OperationData* aOperationData) {

	Vec3<float> data = *(Vec3<float>*)(aOperationData->getData());

	switch(aOperationData->getOpType()) {
		case CameraOperationData::CameraOperationType::ZOOM:
			mCamera->zoom(data.x()+data.y());
			break;
		case CameraOperationData::CameraOperationType::ROTATE:
			mCamera->rotateBy(data);
			break;
		case CameraOperationData::CameraOperationType::TRANSLATE:
			mCamera->translateBy(data);
			break;
		default:
			break;
	}
	return true;
}

} /* namespace controllers */
