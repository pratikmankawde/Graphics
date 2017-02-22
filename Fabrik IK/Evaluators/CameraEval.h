/*
 * CameraEval.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#ifndef CAMERAEVAL_H_
#define CAMERAEVAL_H_

#include "Evaluator.h"

namespace evaluators {

class CameraEval: public evaluators::Evaluator {
public:
	CameraEval();
	virtual ~CameraEval();

	bool evaluate(void* aArgs);
};

} /* namespace evaluators */
#endif /* CAMERAEVAL_H_ */
