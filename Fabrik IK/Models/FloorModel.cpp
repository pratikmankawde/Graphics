/*
 * FloorModel.cpp
 *
 *  Created on: 17-Feb-2017
 *      Author: root
 */

#include "FloorModel.h"

namespace models {

FloorModel::FloorModel() {
	mScreenWidth = 1280;
}

void
FloorModel::generateVertices() {

}

void
FloorModel::drawModel() {

	GLfloat floorDepth = 0.0f;
	unsigned int cellSize = 8;

	glPushMatrix();
	GLuint gridSize = mScreenWidth/(cellSize*cellSize);
	float gridCenter = (gridSize * cellSize) / 2;
	glTranslatef(-gridCenter,-10.0f,-gridCenter);

	glBegin(GL_QUADS);
		for (GLuint x =0;x<gridSize;++x) {
			for (GLuint z =0;z<gridSize;++z)
			{
				if ((x+z)&0x00000001) //modulo 2
					glColor3f(1.0f,1.0f,1.0f); //white
				else
					glColor3f(0.3f,0.3f,0.3f); //black

				GLfloat vertexX = x*cellSize, vertexZ = z*cellSize;
				glVertex3f(vertexX, floorDepth, vertexZ);
				glVertex3f(vertexX+cellSize, floorDepth, vertexZ);
				glVertex3f(vertexX+cellSize, floorDepth, vertexZ+cellSize);
				glVertex3f(vertexX, floorDepth, vertexZ+cellSize);

			}
		}
	glEnd();
	glPopMatrix();

}


FloorModel::~FloorModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace operations */
