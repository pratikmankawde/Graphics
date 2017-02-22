/*
 * Joint.h
 *
 *  Created on: 05-Feb-2017
 *      Author: PprrATeekK
 */

# include "Vec3.h"

#ifndef JOINT_H_
#define JOINT_H_

namespace elements {

class Joint {

private:

	Vec3<float> mPosition;
	Vec3<float> mApex;

public:
	Joint(const Vec3<float> aPosition) {
		mPosition = aPosition;
		mApex = mPosition + 1.0f;
	}

	void setPosition(const Vec3<float> aPosition){mPosition = aPosition;}
	Vec3<float> getPosition() {return mPosition;}

	void setApex(const Vec3<float> aApex){mApex = aApex;}
	Vec3<float> getApex() {return mApex;}

	void extendLenght();

	virtual ~Joint();
};

} /* namespace fabrik */
#endif /* JOINT_H_ */

