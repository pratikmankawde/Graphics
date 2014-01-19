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


	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	envImg = SOIL_load_OGL_cubemap("img_right.JPG","img_left.JPG","img_top.JPG","img_bottom.JPG","img_front.JPG","img_back.JPG",SOIL_LOAD_RGB,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS);

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
		shaderAttachFromFile(g_program, GL_VERTEX_SHADER, "shader.vp");
		shaderAttachFromFile(g_program, GL_FRAGMENT_SHADER, "shader.fp");

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

	//	glUseProgram(g_program);



}
}

void RenderEnvironment(void){


	//  glDepthMask (GL_FALSE);
	  glUseProgram(g_program);
	  glActiveTexture (GL_TEXTURE0);
	  glBindTexture (GL_TEXTURE_CUBE_MAP, envImg);

	  glutSolidCube(100.0);

	  glUseProgram(0);
	 // glDepthMask (GL_TRUE);

}
