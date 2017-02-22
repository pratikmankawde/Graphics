/*
 * main.cpp
 *
 *  Created on: 06-Feb-2017
 *      Author: PprrATeekK
 */

#include <stdlib.h>

#include "Elements/Hierarchy.h"
#include "Models/HierarchyModel.h"
#include "Models/JointModel.h"
#include "Elements/Vec3.h"
#include "Display/Display.h"

Hierarchy* hierarchy;

void init() {
	float jointDisplacement = 2.0f;
	float jointPosition[3] = {0.0f, 0.0f, 0.0f};
	Vec3<float> jointPositionVec;
	jointPositionVec.setVec3(jointPosition);

	hierarchy = new Hierarchy();

	Joint j1(jointPositionVec);
	Joint j2(j1.getPosition() + jointDisplacement);
	Joint j3(j2.getPosition() + jointDisplacement);
	Joint j4(j3.getPosition() + jointDisplacement);
	Joint j5(j4.getPosition() + jointDisplacement);
	Joint j6(j5.getPosition() + jointDisplacement);
	Joint j7(j6.getPosition() + jointDisplacement);
	Joint j8(j7.getPosition() + jointDisplacement);
	Joint j9(j8.getPosition() + jointDisplacement);

	hierarchy->addJoint(j1);
	hierarchy->addJoint(j2);
	hierarchy->addJoint(j3);
	hierarchy->addJoint(j4);
	hierarchy->addJoint(j5);
	hierarchy->addJoint(j6);
	hierarchy->addJoint(j7);
	hierarchy->addJoint(j8);
	hierarchy->addJoint(j9);

	hierarchy->updateDistArray();
}


int main(int argc, char **argv) {
	init();
	HierarchyModel hier(hierarchy);
	//JointModel* jointModel = new JointModel();
	hier.generateVertices();
	display::Display display;
	display.addModel(&hier);
	//display.addModel(jointModel);
	display.initData(hierarchy);
	// init GLUT and create window
	display.initDisplay(argc, argv);
	return 1;
}
