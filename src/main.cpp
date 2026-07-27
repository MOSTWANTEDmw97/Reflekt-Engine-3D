#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Mesh.h"
#include"Texture.h"

//OpenGL 4.6.0
//GLFW 3.3

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Initialize GLFW 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "Renderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	// Resize frame buffer
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Texture brick("Assets/Textures/StoneBrick.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	//Texture pebble("Assets/Textures/Pebble.jpeg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);

	Shader shader("Shaders/Default_Vertex.glsl", "Shaders/Default_Fragment.glsl");
	GLfloat firstTriangle[] = {
	   -0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // left 
	   -0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // right
	   -0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f  // top 
	};
	GLfloat secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 
		0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f
	};
	GLuint triangleIndices[] = {
		0, 1, 2
	};

	Mesh triangle(firstTriangle, sizeof(firstTriangle), triangleIndices, sizeof(triangleIndices), GL_STATIC_DRAW);
	Mesh triangle2(secondTriangle, sizeof(secondTriangle), triangleIndices, sizeof(triangleIndices), GL_STATIC_DRAW);
	
	shader.Use();
	glUniform1i(glGetUniformLocation(shader.ID, "tex0"), 0);

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Update
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.Use();
		brick.Bind();


		triangle.Draw();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		brick.Unbind();

		brick.Bind();
		triangle2.Draw();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	triangle.Delete();
	shader.Delete();
	
	glfwTerminate();
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

}