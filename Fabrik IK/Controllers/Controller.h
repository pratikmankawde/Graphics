/*
 * Controller.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */
#include "../OperationData/OperationData.h"

#ifndef CONTROLLER_H_
#define CONTROLLER_H_


using namespace operations_data;

namespace controllers {

class Controller {

protected:
	;

public:
	Controller() {} ;

	virtual ~Controller() {

	}
	virtual bool performOperation(OperationData* aOperationData) = 0;
	void updateOperationData();
};

} /* namespace controllers */
#endif /* CONTROLLER_H_ */
