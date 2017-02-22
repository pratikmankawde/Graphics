/*
 * RigMotionOperation.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#ifndef RIGMOTIONOPERATIONDATA_H_
#define RIGMOTIONOPERATIONDATA_H_

#include "OperationData.h"

using namespace elements;

namespace operations_data {

class RigMotionOperationData: public OperationData {

public:

	enum RigMotionOperationType {
		IK_EVAL,
		FK_EVAL,
		ADD_JOINT,
		REMOVE_JOINT,
		CONNECT_JOINTS,
		CONNECT_HIER
	};

private:
	RigMotionOperationType mOpType;
	Vec3<float> mDestination;

public:
	RigMotionOperationData() {
		mOpType = ADD_JOINT;
		mDestination.setVec3(0.0f);
	};
	virtual ~RigMotionOperationData();

	void* getData() {
		return (void*)&mDestination;
	}

	int getOpType() {
		return mOpType;
	}

	void setData(void* aData) {
		mDestination = *((Vec3<float>*)aData);
	}
	void setOpType(int aOpType) {
		mOpType = (RigMotionOperationType)aOpType;
	}
};

} /* namespace operations */
#endif /* RIGMOTIONOPERATIONDATA_H_ */
