/*
 * CameraOperationData.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#ifndef CAMERAOPERATIONDATA_H_
#define CAMERAOPERATIONDATA_H_

#include "OperationData.h"

using namespace elements;

namespace operations_data {

class CameraOperationData: public OperationData {

public:

	enum CameraOperationType {
		TRANSLATE,
		ZOOM,
		ROTATE,
		RESET,
		FRAME,
	};

private:
	CameraOperationType mOpType;
	Vec3<float> mDiff;

public:

	CameraOperationData(){
		mOpType = FRAME;
		mDiff.setVec3(0.0f);
	};

	virtual ~CameraOperationData();

	void* getData() {
		return (void*)&mDiff;
	}
	int getOpType() {
		return mOpType;
	}

	void setData(void* aData) {
		mDiff = *(Vec3<float>*)aData;
	}
	void setOpType(int aOpType) {
		mOpType = (CameraOperationType)aOpType;
	}
};

} /* namespace operations_data */
#endif /* CAMERAOPERATIONDATA_H_ */
