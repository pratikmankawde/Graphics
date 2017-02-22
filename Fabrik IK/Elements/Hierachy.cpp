/*
 * Hierachy.cpp
 *
 *  Created on: 06-Feb-2017
 *      Author: PprrATeekK
 */

#include "Hierarchy.h"

using namespace std;

namespace elements {


vector<Joint>*
Hierarchy::getJointHierarchy() {
	return &joints;
}

void
Hierarchy::addJoint(Joint aJoint) {
	joints.push_back(aJoint);
}

void
Hierarchy::updateDistArray(){

	for (size_t jointIndex = 0; jointIndex < joints.size() - 1; ++jointIndex) {
		float dist = (joints[jointIndex+1].getPosition() - joints[jointIndex].getPosition()).getLenght();
		distances.push_back(dist);
	}

}

void
Hierarchy::addJoints(vector<Joint>* aJoints) {

	joints.insert(joints.end(), aJoints->begin(), aJoints->end());
	/* To-Do : Update distances vector in this case */
}

Joint*
Hierarchy::getJointAtIndex(int index) {

	return &(joints[index]);
}

void
Hierarchy::setJointPosition(size_t index, const Vec3<float> aPosition) {
    joints[index].setPosition(aPosition);
}

float
Hierarchy::maxHierarchyLength() const {
	float length = 0.0;

	for (float dist : distances) {
		length += dist;
	}
	return length;
}

}
