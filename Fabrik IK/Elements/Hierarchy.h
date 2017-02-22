/*
 * Hierarchy.h
 *
 *  Created on: 06-Feb-2017
 *      Author: PprrATeekK
 */

#include <vector>

#include "Joint.h"

#ifndef HIERARCHY_H_
#define HIERARCHY_H_

using namespace std;

namespace elements {

class Hierarchy {

private:
	vector<Joint> joints;
	vector<float> distances;

public:

	void addJoint(Joint aJoint);
	void addJoints(vector<Joint>* aJoints);
    void setJointPosition(size_t index, const Vec3<float> aPosition);

	Joint* getJointAtIndex(int index);
	size_t getJointCount() const { return joints.size(); }

	float maxHierarchyLength() const;
	vector<Joint>* getJointHierarchy();

	Vec3<float> getRootPosition() { return joints[0].getPosition(); }
	Vec3<float> getTipPosition() { return joints[joints.size()-1].getPosition(); }

    void updateDistArray();
	float getJointDistanceFor(int index) const { return distances[index]; }
	
};


}
#endif /* HIERARCHY_H_ */
