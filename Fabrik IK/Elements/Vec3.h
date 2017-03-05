/*
 * Vec3.h
 *
 *  Created on: 05-Feb-2017
 *      Author: PprrATeekK
 */

#include <math.h>
#include <GL/glut.h>

#ifndef VEC3_H_
#define VEC3_H_

namespace elements {

template<class T=float>
class Vec3 {

private:
	T vec3[3];

public:

	Vec3() {

	}

	Vec3(T* aVec3) {
		vec3[0] = aVec3[0];
		vec3[1] = aVec3[1];
		vec3[2] = aVec3[2];
	}

	Vec3(T aVal0, T aVal1, T aVal2) {
		vec3[0] = aVal0;
		vec3[1] = aVal1;
		vec3[2] = aVal2;
	}

	Vec3(T aVal) {
		vec3[0] = aVal;
		vec3[1] = aVal;
		vec3[2] = aVal;
	}

	void setVec3(T* aVec3) {
		vec3[0] = aVec3[0];
		vec3[1] = aVec3[1];
		vec3[2] = aVec3[2];
	}

	void setVec3(T aVal0, T aVal1, T aVal2) {
		vec3[0] = aVal0;
		vec3[1] = aVal1;
		vec3[2] = aVal2;
	}

	void setVec3(T aVal) {
		vec3[0] = aVal;
		vec3[1] = aVal;
		vec3[2] = aVal;
	}

	void setVec3(Vec3<T> aVec3) {
		vec3[0] = aVec3[0];
		vec3[1] = aVec3[1];
		vec3[2] = aVec3[2];
	}

	T& operator[](int index) {

//		if (index > 2 || index < 0) {
//			exit(0);
//		}
		return vec3[index];
	}


	Vec3<T> operator+(const Vec3<T>& aVec3) {
		Vec3<T> sum;
		sum.vec3[0] = vec3[0] + aVec3.vec3[0];
		sum.vec3[1] = vec3[1] + aVec3.vec3[1];
		sum.vec3[2] = vec3[2] + aVec3.vec3[2];
		return sum;
	}

	Vec3<T> operator+(const T aValue) const {
		Vec3<T> sum;
		sum.vec3[0] = vec3[0] + aValue;
		sum.vec3[1] = vec3[1] + aValue;
		sum.vec3[2] = vec3[2] + aValue;
		return sum;
	}

	Vec3<T> operator*(const T aValue) const {
		Vec3<T> product;
		product.vec3[0] = vec3[0] * aValue;
		product.vec3[1] = vec3[1] * aValue;
		product.vec3[2] = vec3[2] * aValue;
		return product;
	}

	Vec3<T>& operator+=(const Vec3<T>& aVec3) {
		this->vec3[0] = this->vec3[0] + aVec3.vec3[0];
		this->vec3[1] = this->vec3[1] + aVec3.vec3[1];
		this->vec3[2] = this->vec3[2] + aVec3.vec3[2];
		return *this;
	}

	Vec3<T>& operator+=(const T aValue) const {
		vec3[0] = vec3[0] + aValue;
		vec3[1] = vec3[1] + aValue;
		vec3[2] = vec3[2] + aValue;
		return *this;
	}

	Vec3<T>& operator*=(T aValue) const {
		vec3[0] = vec3[0] * aValue;
		vec3[1] = vec3[1] * aValue;
		vec3[2] = vec3[2] * aValue;
		return *this;
	}

	Vec3<T>& operator=(const Vec3<T>& aVec3){
		this->vec3[0] = aVec3.vec3[0];
		this->vec3[1] = aVec3.vec3[1];
		this->vec3[2] = aVec3.vec3[2];
		return *this;
	}

	Vec3<T>& operator=(const T aVal){
		this->vec3[0] = aVal;
		this->vec3[1] = aVal;
		this->vec3[2] = aVal;
		return *this;
	}

	Vec3<T> operator-(const Vec3<T> aVec3) const {
		Vec3<T> difference;
		difference.vec3[0] = vec3[0] - aVec3.vec3[0];
		difference.vec3[1] = vec3[1] - aVec3.vec3[1];
		difference.vec3[2] = vec3[2] - aVec3.vec3[2];
		return difference;
	}

	Vec3<T> operator-(const T aValue) const {
		Vec3<T> difference;
		difference.vec3[0] = vec3[0] - aValue;
		difference.vec3[1] = vec3[1] - aValue;
		difference.vec3[2] = vec3[2] - aValue;
		return difference;
	}

	T getLenght() const {
		T sqrtVal = sqrt(vec3[0]*vec3[0] + vec3[1]*vec3[1] + vec3[2]*vec3[2]);
		return sqrtVal;
	}

	T* getArray() {
		return vec3;
	}

	T x() { return vec3[0]; }
	T y() { return vec3[1]; }
	T z() { return vec3[2]; }

	Vec3<T> crossProduct(Vec3<T> aVec3)
	{
		Vec3<T> product;

	        //Cross product formula
		product[0] = (vec3[1] * aVec3[2]) - (vec3[2] * aVec3[1]);
		product[1] = (vec3[2] * aVec3[0]) - (vec3[0] * aVec3[2]);
		product[2] = (vec3[0] * aVec3[1]) - (vec3[1] * aVec3[0]);

		return product;
	}

	void normalize() {

	    T length = getLenght();

	    vec3[0] = vec3[0]/length;
	    vec3[1] = vec3[1]/length;
	    vec3[2] = vec3[2]/length;
	}

	float dotProduct(Vec3<T> vec1, Vec3<T> vec2) {

		return (vec1[0]*vec2[0]+vec1[1]*vec2[1]+vec1[2]*vec2[2]);

	}

	T getAngle(Vec3<T> aSecondVec3) {

		return acosf(dotProduct(vec3, aSecondVec3)/(getLenght()*aSecondVec3.getLenght())) * 180.0 / M_PI;

	}

	void getAngleAndAxisOfRotation(Vec3<T> aVec1, Vec3<T> aVec2, T* aAngle, Vec3<T>* aAxis) {

		aVec1.normalize();
		aVec2.normalize();

		aAxis->setVec3(aVec1.crossProduct(aVec2));

		*aAngle = aVec1.getAngle(aVec2);
	}

	/* column major post multiplication
	 * 	     ( m[0]   m[4]   m[8]    m[12] )	 ( v[0]	)
		     |m[1]   m[5]    m[9]   m[13]  |	 | v[1]	|
	  M(v) = |m[2]   m[6]   m[10]   m[14]  |  x	 | v[2]	|
		     (m[3]   m[7]   m[11]   m[15]  )	 ( v[3]	)
	 */

	void transformVertex(GLdouble* aMVMatrix) {
		Vec3<GLdouble> trans;
		trans[0] = aMVMatrix[0] * vec3[0] + aMVMatrix[1] * vec3[1] + aMVMatrix[2] * vec3[2] + aMVMatrix[3] * 1.0;
		trans[1] = aMVMatrix[4] * vec3[0] + aMVMatrix[5] * vec3[1] + aMVMatrix[6] * vec3[2] + aMVMatrix[7] * 1.0;
		trans[2] = aMVMatrix[8] * vec3[0] + aMVMatrix[9] * vec3[1] + aMVMatrix[10] * vec3[2] + aMVMatrix[11] * 1.0;
	  GLdouble w = aMVMatrix[12] * vec3[0] + aMVMatrix[13] * vec3[1] + aMVMatrix[14] * vec3[2] + aMVMatrix[15] * 1.0;
		vec3[0] = trans[0]/w;
		vec3[1] = trans[1]/w;
		vec3[2] = trans[2]/w;

	}

	void setPlane(Vec3<T> aVec1, Vec3<T> aVec2, Vec3<T> aVec3, T* offset) {

		Vec3<T> inPlaneVec1 = aVec2-aVec1;
		Vec3<T> inPlaneVec2 = aVec3-aVec1;
		*this = inPlaneVec1.crossProduct(inPlaneVec2);
		/* Equation of plane: ax+by+cz+d=0, d is our offset*/
		*offset = -(vec3[0]*aVec1[0] + vec3[1]*aVec1[1] + vec3[2]*aVec1[3]);
	}

	Vec3<GLfloat> getIntersectionPointwithLine (Vec3<T> aVec1, Vec3<T> aSlopes, T offset) {

		Vec3<GLfloat> intersectionPoint;
		T denominator = vec3[0]*aSlopes[0]+vec3[1]*aSlopes[1]+vec3[2]*aSlopes[2];
		T numerator = vec3[0]*aVec1[0]+vec3[1]*aVec1[1] + vec3[2]*aVec1[2]+offset;
		intersectionPoint[0] = aVec1[0] - (aSlopes[0] * numerator/denominator);
		intersectionPoint[1] = aVec1[1] - (aSlopes[1] * numerator/denominator);
		intersectionPoint[2] = aVec1[2] - (aSlopes[2] * numerator/denominator);
		return intersectionPoint;
	}
};

}

#endif /* VEC3_H_ */
