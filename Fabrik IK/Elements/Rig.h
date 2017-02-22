/*
 * Rig.h
 *
 *  Created on: 15-Feb-2017
 *      Author: PprrATeekK
 */
#include <vector>
#include "Hierarchy.h"

#ifndef ELEMENTS_RIG_H_
#define ELEMENTS_RIG_H_

using namespace std;

namespace elements {

class Rig {

private:
	vector<Hierarchy*> mHierarchies;

public:

	Rig();

	void addHierarchy(Hierarchy* aHierarchy) { mHierarchies.push_back(aHierarchy); };
	Hierarchy* getHierarchy() { return mHierarchies[0]; } ;
	virtual ~Rig();
};

} /* namespace elements */

#endif /* ELEMENTS_RIG_H_ */
