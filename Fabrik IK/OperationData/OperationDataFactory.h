/*
 * OperationFactory.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#ifndef OPERATIONDATAFACTORY_H_
#define OPERATIONDATAFACTORY_H_

#include "CameraOperationData.h"
#include "RigMotionOperationData.h"

namespace operations_data {

class OperationDataFactory {

private:
	CameraOperationData* mCameraOperationData;
	RigMotionOperationData* mRigOperationData;

public:

	enum OperationDataType {
		CAMERA_OPERATION_DATA,
		RIG_OPERATION_DATA,
	};

	OperationDataFactory();

	virtual ~OperationDataFactory();

	OperationData* getInstance(OperationDataType aOperationDataType);
};

} /* namespace operations_data */
#endif /* OPERATIONDATAFACTORY_H_ */
