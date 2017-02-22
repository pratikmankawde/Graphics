/*
 * OperationFactory.cpp
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#include "OperationDataFactory.h"

namespace operations_data {

OperationDataFactory::OperationDataFactory() {
	mCameraOperationData = new  CameraOperationData();
	mRigOperationData = new RigMotionOperationData();
};

OperationData*
OperationDataFactory::getInstance(OperationDataType aOperationDataType) {

	switch (aOperationDataType) {

		case OperationDataType::CAMERA_OPERATION_DATA:
			return mCameraOperationData;
			break;

		case OperationDataType::RIG_OPERATION_DATA:
			return mRigOperationData;
			break;

		default:
			return nullptr;
	}

}

OperationDataFactory::~OperationDataFactory() {
	// TODO Auto-generated destructor stub
}

} /* namespace operations_data */
