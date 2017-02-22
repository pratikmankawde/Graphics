#include "Model.h"


namespace models {


void
Model::addVertex(Vec3<GLfloat> aVertex) {

	mVertices.push_back(aVertex);
}
void
Model::addVertices(std::vector<Vec3GLfloat>* aVertices) {

	mVertices.insert(mVertices.end(), aVertices->begin(), aVertices->end());
}

void
Model::calculatePerFaceNormal() {

}

void
Model::calculatePerVertexNormal() {

}

//void calculatePerFragmentNormal(int vertex1, int vertex2, int vertex3, int faceNo) {
//
//	GLdouble va[3], vb[3], vr[4], Nfact;
//
//	GLdouble *coord1 = vertices[vertex1].coord;
//	GLdouble *coord2 = vertices[vertex2].coord;
//	GLdouble *coord3 = vertices[vertex3].coord;
//
//	va[0] = coord2[0] - coord1[0];
//	va[1] = coord2[1] - coord1[1];
//	va[2] = coord2[2] - coord1[2];
//
//	vb[0] = coord3[0] - coord1[0];
//	vb[1] = coord3[1] - coord1[1];
//	vb[2] = coord3[2] - coord1[2];
//
//	/* cross product */
//	vr[0] = va[1] * vb[2] - va[2] * vb[1];
//	vr[1] = va[2] * vb[0] - va[0] * vb[2];
//	vr[2] = va[0] * vb[1] - va[1] * vb[0];
//
//	/* normalization factor */
//	Nfact = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);
//
//	normalsPerFragment[faceNo].normal[0] = vr[0] / Nfact;
//
//	normalsPerFragment[faceNo].normal[1] = vr[1] / Nfact;
//
//	normalsPerFragment[faceNo].normal[2] = vr[2] / Nfact;
//
//}
//
//void calculatePerVertexNormal(int vertex1, int vertex2, int vertex3) {
//	/* calculate Vector1 and Vector2 */
//
//	GLdouble va[3], vb[3], vr[4], Nfact;
//
//	GLdouble *coord1 = vertices[vertex1].coord;
//	GLdouble *coord2 = vertices[vertex2].coord;
//	GLdouble *coord3 = vertices[vertex3].coord;
//
//	va[0] = coord2[0] - coord1[0];
//	va[1] = coord2[1] - coord1[1];
//	va[2] = coord2[2] - coord1[2];
//
//	vb[0] = coord3[0] - coord1[0];
//	vb[1] = coord3[1] - coord1[1];
//	vb[2] = coord3[2] - coord1[2];
//
//	/* cross product */
//	vr[0] = va[1] * vb[2] - va[2] * vb[1];
//	vr[1] = va[2] * vb[0] - va[0] * vb[2];
//	vr[2] = va[0] * vb[1] - va[1] * vb[0];
//
//	/* normalization factor */
//	Nfact = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);
//
//	normals[vertex1].normal[0] += vr[0] / Nfact;
//	normals[vertex2].normal[0] += vr[0] / Nfact;
//	normals[vertex3].normal[0] += vr[0] / Nfact;
//
//	normals[vertex1].normal[1] += vr[1] / Nfact;
//	normals[vertex2].normal[1] += vr[1] / Nfact;
//	normals[vertex3].normal[1] += vr[1] / Nfact;
//
//	normals[vertex1].normal[2] += vr[2] / Nfact;
//	normals[vertex2].normal[2] += vr[2] / Nfact;
//	normals[vertex3].normal[2] += vr[2] / Nfact;
//
//	normals[vertex1].count += 1;
//	normals[vertex2].count += 1;
//	normals[vertex3].count += 1;
//
//}
//
//static void Normalize() {
//	int i = 0;
//	GLdouble normalizingFactor = sqrt(origin[0].coord[0] * origin[0].coord[0] + origin[0].coord[1] * origin[0].coord[1] + origin[0].coord[2] * origin[0].coord[2]);
//
//	if(normalizingFactor<0.000001)
//		return;
//
//	maxZ /= normalizingFactor;
//	for (i = 0; i < noOfVertices; i++) {
//
//		vertices[i].coord[0] /= normalizingFactor;
//		vertices[i].coord[1] /= normalizingFactor;
//		vertices[i].coord[2] /= normalizingFactor;
//
//		if (maxZ < vertices[i].coord[2])
//			maxZ = vertices[i].coord[2];
//	}
//
//	origin[0].coord[0] /= normalizingFactor;
//	origin[0].coord[1] /= normalizingFactor;
//	origin[0].coord[2] /= normalizingFactor;
//
//}

void
Model::calculateNormals(NORMAL_TYPE aNormalType) {

}

void
Model::clear(CLEAR_TYPE aClearType) {

	switch(aClearType) {

	case CLEAR_TYPE::ONLY_VERTICES :
		mVertices.clear();
		break;
	case CLEAR_TYPE::ONLY_NORMALS :
		mNormals.clear();
		break;
	case CLEAR_TYPE::BOTH_VERTICES_AND_NORMALS:
		mVertices.clear();
		mNormals.clear();
		break;
	}
}

}
