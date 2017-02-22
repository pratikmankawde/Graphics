/*
 * Operation.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#include "../Elements/Vec3.h"

#ifndef OPERATIONDATA_H_
#define OPERATIONDATA_H_

namespace operations_data {

class OperationData {
public:
	OperationData();
	virtual ~OperationData();
	virtual int getOpType() = 0;
	virtual void* getData() = 0;
	virtual void setData(void* aData) = 0;
	virtual void setOpType(int aOpType) = 0;
};

} /* namespace operations_data */
#endif /* OPERATIONDATA_H_ */
