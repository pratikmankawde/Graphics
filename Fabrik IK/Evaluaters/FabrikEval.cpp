/*
 * FabrikEval.cpp
 *
 *  Created on: 06-Feb-2017

 *      Author: PprrATeekK
 */

#include <iostream>

#include "FabrikEval.h"

using namespace std;

namespace fabrik {

//void
//FabrikEval::print(Hierarchy* hierarchy, Vec3 dest) {
//
//    const float* tip = hierarchy->getTipPosition().getArray();
//    const float* des =  dest.getArray();
//    cout<<"Tip: {"<<tip[0]<<", "<<tip[1]<<", "<<tip[2]<<"} dest: {"<<des[0]<<", "<<des[1]<<", "<<des[2]<<"}"<<endl;
//
//}
//void
//printPos(Vec3 aPos){
//    const float* pos = aPos.getArray();
//    cout<<"Pos: {"<<pos[0]<<", "<<pos[1]<<", "<<pos[2]<<"}"<<endl;
//}

void
FabrikEval::solve(Hierarchy* hierarchy, const Vec3<float> destination) {

	Vec3<float> dist = hierarchy->getRootPosition() - destination;


	if (dist.getLenght() > hierarchy->maxHierarchyLength()) {
      //  cout<<"dest out of reach"<<endl;
     //   print(hierarchy, destination);
		for (size_t index = 0; index < hierarchy->getJointCount() - 1; ++index) {
			Joint* currentJoint = hierarchy->getJointAtIndex(index);
			Joint* nextJoint = hierarchy->getJointAtIndex(index+1);
			Vec3<float> diff = destination - currentJoint->getPosition();
			float lamda = hierarchy->getJointDistanceFor(index) / diff.getLenght();
			nextJoint->setPosition((currentJoint->getPosition()*(1-lamda)) + (destination*lamda));
		}
	} else {
    //    print(hierarchy, destination);
		Vec3<float> root = hierarchy->getRootPosition();
		dist = hierarchy->getTipPosition() - destination;

		while (dist.getLenght() > mTolerance) {
           // print(hierarchy, destination);
			/* STAGE 1: FORWARD REACHING */
			/* Set the end effector as target */
			hierarchy->getJointAtIndex(hierarchy->getJointCount()-1)->setPosition(destination);
			for (size_t index = hierarchy->getJointCount() - 2; index >= 0 ; --index) {
				Joint* currentJoint = hierarchy->getJointAtIndex(index);
				Joint* nextJoint = hierarchy->getJointAtIndex(index+1);
				Vec3<float> diff = nextJoint->getPosition() - currentJoint->getPosition(); // Updated distance
				float lamda = hierarchy->getJointDistanceFor(index) / diff.getLenght();
				currentJoint->setPosition((nextJoint->getPosition()*(1-lamda)) + (currentJoint->getPosition()*lamda));
			}

			/* STAGE 2: BACKWARD REACHING */
			/* Set the root to its initial position. */
			hierarchy->getJointAtIndex(0)->setPosition(root);
			for (size_t index = 0; index < hierarchy->getJointCount() - 1; ++index) {
				Joint* currentJoint = hierarchy->getJointAtIndex(index);
				Joint* nextJoint = hierarchy->getJointAtIndex(index+1);
				Vec3<float> diff = nextJoint->getPosition() - currentJoint->getPosition(); // Updated distance
				float lamda = hierarchy->getJointDistanceFor(index) / diff.getLenght();
				nextJoint->setPosition((currentJoint->getPosition()*(1-lamda)) + (nextJoint->getPosition()*lamda));
			}

			dist = hierarchy->getTipPosition() - destination;
		}

	}
//
//	for (size_t jointIndex = 0; jointIndex < hierarchy->getJointCount(); ++jointIndex) {
//			const Vec3 position = hierarchy->getJointAtIndex(jointIndex)->getPosition();
//			printPos(position);
//	}
//	print(hierarchy, destination);
}


} /* namespace fabrik */
