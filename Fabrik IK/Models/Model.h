#include <vector>
#include <GL/glut.h>

#include "../Elements/Vec3.h"

#ifndef MODEL_H_
#define MODEL_H_

using namespace std;
using namespace elements;

typedef Vec3<GLfloat> Vec3GLfloat;

namespace models {

class Model {

protected:

	GLfloat mDepth;
	Vec3<GLfloat> mCenter;
	vector<Vec3GLfloat> mVertices;
	vector<Vec3GLfloat> mNormals;
	Vec3<GLfloat> mColor;

public:

	typedef enum NORMAL_TYPE {
		PER_FACE,
		PER_VERTEX
	}NORMAL_TYPE;

	typedef enum CLEAR_TYPE {
		ONLY_VERTICES,
		ONLY_NORMALS,
		BOTH_VERTICES_AND_NORMALS
	}CLEAR_TYPE;

	Model() {
		mDepth = 0.0f;
		mCenter.setVec3(0.0f);
		mColor.setVec3(1.0f);
	}
	virtual ~Model() {

	}

	void addVertex(Vec3<GLfloat> aVertex);
	void addVertices(std::vector<Vec3GLfloat>* aVertices);

	virtual void generateVertices() = 0;
	virtual void drawModel() = 0;

	vector<Vec3GLfloat>& getNormals() { return mNormals; };
	vector<Vec3GLfloat>& getVertices() { return mVertices; };
	size_t getVertexCount() const { return mVertices.size(); };
	Vec3<GLfloat> getCenter() { return mCenter; };
	GLfloat getDepth() { return mDepth; };

	void calculatePerFaceNormal();
	void calculatePerVertexNormal();
	void calculateNormals(NORMAL_TYPE aNormalType);

	void clear(CLEAR_TYPE aClearType);

};

}
#endif /* MODELS_MODEL_H_ */
