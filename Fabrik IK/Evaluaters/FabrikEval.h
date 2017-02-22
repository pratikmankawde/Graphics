/*
 * FabrikEval.h
 *
 *  Created on: 06-Feb-2017
 *      Author: PprrATeekK
 */

#include "../Elements/Hierarchy.h"
#include "../Elements/Vec3.h"

#ifndef FABRIKEVAL_H_
#define FABRIKEVAL_H_

using namespace elements;

namespace fabrik {

class FabrikEval {

private:
	float mTolerance;
public:

	FabrikEval() {
		mTolerance = 0.00001;
	};

	FabrikEval(float aTolerance) : mTolerance(aTolerance){};
	void solve(Hierarchy* hierarchy, const Vec3<float> destination);
    void print(Hierarchy* hierarchy, Vec3<float> dest);

};

} /* namespace fabrik */

#endif /* FABRIKEVAL_H_ */
