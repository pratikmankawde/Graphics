/*
 * RigMotionController.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#ifndef RIGMOTIONCONTROLLER_H_
#define RIGMOTIONCONTROLLER_H_

#include "Controller.h"
#include "../Evaluators/FabrikIKEval.h"
#include "../Elements/Rig.h"
#include "../OperationData/RigMotionOperationData.h"

using namespace elements;
using namespace evaluators;
using namespace operations_data;

namespace controllers {

class RigMotionController: public controllers::Controller {

private:

	FabrikIKEval* mFabrikEvaluator;
	Rig* mRig;

public:

	RigMotionController(Rig* aRig) {
		mRig = aRig;
		mFabrikEvaluator =  new FabrikIKEval();
	}

	virtual ~RigMotionController() {

	}

	bool performOperation(OperationData* aOperationData);

};

} /* namespace controllers */
#endif /* RIGMOTIONCONTROLLER_H_ */
