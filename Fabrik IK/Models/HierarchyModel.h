
#include "../Elements/Hierarchy.h"
#include "Model.h"

#ifndef MODELS_HIERARCHYMODEL_H_
#define MODELS_HIERARCHYMODEL_H_

using namespace elements;

namespace models {

class HierarchyModel : public Model {

private:
	Hierarchy* mHierarchy;

public:
	HierarchyModel(Hierarchy* aHierarchy) : mHierarchy(aHierarchy) {

		mColor.setVec3(1.0f);
	};

	void generateVertices();
	void drawModel();
};

}

#endif /* HIERARCHYMODEL_H_ */
