/*
 * CameraController.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include "Controller.h"
#include "../Display/Camera.h"
#include "../OperationData/CameraOperationData.h"

using namespace display;
using namespace operations_data;

namespace controllers {

class CameraController: public controllers::Controller {

private:
	Camera* mCamera;

public:
	CameraController(Camera* aCamera) : mCamera(aCamera){};
	virtual ~CameraController();

	bool performOperation(OperationData* aOperationData);

};

} /* namespace controllers */
#endif /* CAMERACONTROLLER_H_ */
