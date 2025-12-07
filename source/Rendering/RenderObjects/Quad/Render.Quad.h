#ifndef OPENGL_QUAD_H
#define OPENGL_QUAD_H

#include "Utils/OpenGL/Shaders/Shader.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Quad {
public:
	Quad();
	~Quad();

	void Draw();

protected:
	GLuint VAO,VBO,EBO;

	void Init();
};





#endif
