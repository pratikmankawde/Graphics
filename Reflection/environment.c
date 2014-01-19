#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>

GLuint envImg;
GLuint textureIDEnvironment;
GLuint g_program;

extern void shaderAttachFromFile(GLuint, GLenum, const char *);


void LoadEnvironmentTexture(void) {


	//glEnable(GL_TEXTURE_CUBE_MAP)​;

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	envImg = SOIL_load_OGL_cubemap("img_right.JPG","img_left.JPG","img_top.JPG","img_bottom.JPG","img_front.JPG","img_back.JPG",SOIL_LOAD_RGB,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS);

	//glGenTextures(1, &textureIDEnvironment);
//	glBindTexture(GL_TEXTURE_2D, textureIDEnvironment);

	// Give the image to OpenGL
	//glTexImage2D( GL_TEXTURE_CUBE_MAP, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, envImg);
		//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (0 == envImg) {
	  printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}


	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		g_program = glCreateProgram();
		shaderAttachFromFile(g_program, GL_FRAGMENT_SHADER, "shader.fp");
		shaderAttachFromFile(g_program, GL_VERTEX_SHADER, "shader.vp");

		/* link the program and make sure that there were no errors */
		glLinkProgram(g_program);

		GLint result;

		glGetProgramiv(g_program, GL_LINK_STATUS, &result);
		if(result == GL_FALSE) {
			GLint length;
			char *log;

			/* get the program info log */
			glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &length);
			log = malloc(length);
			glGetProgramInfoLog(g_program, length, &result, log);

			/* print an error message and the info log */
			fprintf(stderr, "sceneInit(): Program linking failed: %s\n", log);
			free(log);

			/* delete the program */
			glDeleteProgram(g_program);

}
}

void RenderEnvironment(void){


	const float vertex=30.0f;

		 const float points[] = {
				  -vertex,  vertex, -vertex,  -vertex, -vertex, -vertex,   vertex, -vertex, -vertex,   vertex, -vertex, -vertex,   vertex,  vertex, -vertex,  -vertex,  vertex, -vertex,
				  	    -vertex, -vertex,  vertex,  -vertex, -vertex, -vertex,  -vertex,  vertex, -vertex,  -vertex,  vertex, -vertex,  -vertex,  vertex,  vertex,  -vertex, -vertex,  vertex,
				  	     vertex, -vertex, -vertex,   vertex, -vertex,  vertex,   vertex,  vertex,  vertex,   vertex,  vertex,  vertex,   vertex,  vertex, -vertex,   vertex, -vertex, -vertex,
				  	    -vertex, -vertex,  vertex,  -vertex,  vertex,  vertex,   vertex,  vertex,  vertex,   vertex,  vertex,  vertex,   vertex, -vertex,  vertex,  -vertex, -vertex,  vertex,
				  	    -vertex,  vertex, -vertex,   vertex,  vertex, -vertex,   vertex,  vertex,  vertex,   vertex,  vertex,  vertex,  -vertex,  vertex,  vertex,  -vertex,  vertex, -vertex,
				  	    -vertex, -vertex, -vertex,  -vertex, -vertex,  vertex,   vertex, -vertex, -vertex,   vertex, -vertex, -vertex,  -vertex, -vertex,  vertex,   vertex, -vertex,  vertex
		  };





	  unsigned int vbo = 0;
	  glGenBuffers (1, &vbo);
	  glBindBuffer (GL_ARRAY_BUFFER, vbo);
	  glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);

	  unsigned int vao = 0;
	  glGenVertexArrays(1, &vao);
	  glBindVertexArray(vao);
	  glEnableVertexAttribArray(0);
	  glBindBuffer(GL_ARRAY_BUFFER, vbo);
	  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);


	  glDepthMask (GL_FALSE);
	  glUseProgram (g_program);
	  glActiveTexture (GL_TEXTURE0);
	  glBindTexture (GL_TEXTURE_CUBE_MAP, envImg);
	 glBindVertexArray (vao);
	 glDrawArrays (GL_TRIANGLES, 0, 36);
	  glDepthMask (GL_TRUE);

}
