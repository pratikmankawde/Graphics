/*
 * FloorModel.h
 *
 *  Created on: 17-Feb-2017
 *      Author: root
 */

#ifndef FLOORMODEL_H_
#define FLOORMODEL_H_

#include "Model.h"

namespace models {

class FloorModel: public models::Model {

private:
	GLint mScreenWidth;

public:
	FloorModel();
	virtual ~FloorModel();

	void generateVertices();
	void drawModel();
};

} /* namespace operations */
#endif /* FLOORMODEL_H_ */
