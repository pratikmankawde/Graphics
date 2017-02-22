/*
 * FabrikEval.cpp
 *
 *  Created on: 06-Feb-2017

 *      Author: PprrATeekK
 */

#include <iostream>

#include "FabrikIKEval.h"

using namespace std;

namespace evaluators {

bool
FabrikIKEval::evaluate(void* aArgs) {

	Vec3<float> destination = *((Vec3<float>*)aArgs);
	Vec3<float> dist = mHierarchy->getRootPosition() - destination;
//	for (size_t jointIndex = 0; jointIndex < mHierarchy->getJointCount(); ++jointIndex) {
//		const Vec3<float> position = mHierarchy->getJointAtIndex(jointIndex)->getPosition();
//		printPos(position);
//	}
//	cout<<"evaluating"<<endl;
	if (dist.getLenght() > mHierarchy->maxHierarchyLength()) {
       // cout<<"dest out of reach"<<endl;
     //   print(mHierarchy, destination);
		for (size_t index = 0; index < mHierarchy->getJointCount() - 1; ++index) {
			Joint* currentJoint = mHierarchy->getJointAtIndex(index);
			Joint* nextJoint = mHierarchy->getJointAtIndex(index+1);
			Vec3<float> diff = destination - currentJoint->getPosition();
			float lamda = mHierarchy->getJointDistanceFor(index) / diff.getLenght();
			nextJoint->setPosition((currentJoint->getPosition()*(1-lamda)) + (destination*lamda));
		}
	} else {
        //print(mHierarchy, destination);
		Vec3<float> root = mHierarchy->getRootPosition();
		dist = mHierarchy->getTipPosition() - destination;

		while (dist.getLenght() > mTolerance) {
			/* STAGE 1: FORWARD REACHING */
			/* Set the end effector as target */
			mHierarchy->getJointAtIndex(mHierarchy->getJointCount()-1)->setPosition(destination);
			for (int index = mHierarchy->getJointCount() - 2; index >= 0 ; --index) {
				Joint* currentJoint = mHierarchy->getJointAtIndex(index);
				Joint* nextJoint = mHierarchy->getJointAtIndex(index+1);
				Vec3<float> diff = nextJoint->getPosition() - currentJoint->getPosition(); // Updated distance
				float lamda = mHierarchy->getJointDistanceFor(index) / diff.getLenght();
				currentJoint->setPosition((nextJoint->getPosition()*(1-lamda)) + (currentJoint->getPosition()*lamda));
			}

			/* STAGE 2: BACKWARD REACHING */
			/* Set the root to its initial position. */
			mHierarchy->getJointAtIndex(0)->setPosition(root);
			for (size_t index = 0; index < mHierarchy->getJointCount() - 1; ++index) {
				Joint* currentJoint = mHierarchy->getJointAtIndex(index);
				Joint* nextJoint = mHierarchy->getJointAtIndex(index+1);
				Vec3<float> diff = nextJoint->getPosition() - currentJoint->getPosition(); // Updated distance
				float lamda = mHierarchy->getJointDistanceFor(index) / diff.getLenght();
				nextJoint->setPosition((currentJoint->getPosition()*(1-lamda)) + (nextJoint->getPosition()*lamda));
			}

			dist = mHierarchy->getTipPosition() - destination;
		}
	}
//
//	for (size_t jointIndex = 0; jointIndex < mHierarchy->getJointCount(); ++jointIndex) {
//			const Vec3<float> position = mHierarchy->getJointAtIndex(jointIndex)->getPosition();
//			printPos(position);
//	}
//	print(mHierarchy, destination);
	return true;
}


void
FabrikIKEval::print(Hierarchy* hierarchy, Vec3<float> dest) {

    const float* tip = hierarchy->getTipPosition().getArray();
    const float* des =  dest.getArray();
    cout<<"Tip: {"<<tip[0]<<", "<<tip[1]<<", "<<tip[2]<<"} dest: {"<<des[0]<<", "<<des[1]<<", "<<des[2]<<"}"<<endl;

}
void
FabrikIKEval::printPos(Vec3<float> aPos){
    const float* pos = aPos.getArray();
    cout<<"Pos: {"<<pos[0]<<", "<<pos[1]<<", "<<pos[2]<<"}"<<endl;
}

} /* namespace fabrik */
