/*
 * FabrikEval.h
 *
 *  Created on: 06-Feb-2017
 *      Author: PprrATeekK
 */

#include "../Elements/Hierarchy.h"
#include "../Elements/Vec3.h"
#include "Evaluator.h"

#ifndef FABRIKEVAL_H_
#define FABRIKEVAL_H_

using namespace elements;

namespace evaluators {

class FabrikIKEval : public Evaluator {

private:
	float mTolerance;
	Hierarchy* mHierarchy;
public:

	FabrikIKEval() {
		mTolerance = 0.00001;
		mHierarchy = nullptr;
	};

	FabrikIKEval(float aTolerance, Hierarchy* aHierarchy) : mTolerance(aTolerance), mHierarchy(aHierarchy){};
	void setActiveHierarchy(Hierarchy* aHierarchy) { mHierarchy = aHierarchy; };

	bool evaluate(void* aArgs);
    void print(Hierarchy* hierarchy, Vec3<float> dest);
    void printPos(Vec3<float> aPos);

    ~FabrikIKEval() {

    }
};

} /* namespace fabrik */

#endif /* FABRIKEVAL_H_ */
