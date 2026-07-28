#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Mesh.h"
#include"Texture.h"
#include"Camera.h"



float screenWidth = 1920;
float screenHeight = 1080;
//OpenGL 4.6.0
//GLFW 3.3
// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime);
void Mouse_callback(GLFWwindow* window, Camera& camera, float window_Width, float window_Height, bool enableCameraLook);

int main()
{
	//Fields
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	// Initialize GLFW 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window creation
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Renderer", NULL, NULL);
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
	glViewport(0, 0, screenWidth, screenHeight);
	// Resize frame buffer
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Texture brick("Assets/Textures/StoneBrick.jpg", GL_RGB);

	Shader shader("Shaders/Default_Vertex.glsl", "Shaders/Default_Fragment.glsl");

	// Base (square, centered at origin, y = 0)
	GLfloat pyramidVertices[] = {
		// positions           // colors       // texcoords
		-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // back-left
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // back-right
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // front-right
		-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // front-left

		// Apex (top point)
		 0.0f, 0.8f,  0.0f,    1.0f, 0.0f, 1.0f,  0.5f, 0.5f  // top
	};
	GLuint pyramidIndices[] = {
		// Base (two triangles)
		0, 1, 2,
		0, 2, 3,

		// Sides (four triangles)
		0, 1, 4, // back face
		1, 2, 4, // right face
		2, 3, 4, // front face
		3, 0, 4  // left face
	};


	
	Mesh pyramid(pyramidVertices, sizeof(pyramidVertices), pyramidIndices, sizeof(pyramidIndices), GL_STATIC_DRAW);
	shader.Use();
	glUniform1i(glGetUniformLocation(shader.ID, "tex0"), 0);

	//GLuint transformLoc = glGetUniformLocation(shader.ID, "model");

	glm::vec3 camera_Pos = glm::vec3(0.0f, 0.0f, 3.0f);
	float fov = 60.0f;
	float clipNear = 0.1f;
	float clipFar = 100.0f;


	Camera camera(camera_Pos, fov, clipNear, clipFar);
	camera.SetSpeed(1.0f);

	glEnable(GL_DEPTH_TEST);
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Update
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window, camera, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		shader.Use();
		brick.Bind();


		glm::mat4 model = glm::mat4(1.0f);
		camera.BindToShader(shader, model, 800.0f / 600.0f);

		pyramid.Draw();



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	pyramid.Delete();
	shader.Delete();
	
	glfwTerminate();
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
bool enableCameraLook = true;
void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
	//bool enableCameraLook = true;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboardInputs("FORWARD", deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboardInputs("BACKWARD", deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboardInputs("LEFT", deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboardInputs("RIGHT", deltaTime);

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		enableCameraLook = !enableCameraLook;
	}
	Mouse_callback(window, camera, screenWidth, screenHeight, enableCameraLook);

}

void Mouse_callback(GLFWwindow* window, Camera& camera, float window_Width, float window_Height, bool enableCameraLook)
{

	if (enableCameraLook == true)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		static bool firstMouse = true;
		static float lastX = window_Width / 2; // half of window width
		static float lastY = window_Height / 2; // half of window height

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		if (firstMouse)
		{
			lastX = (float)xPos;
			lastY = (float)yPos;
			firstMouse = false;
		}

		float xoffset = (float)xPos - lastX;
		float yoffset = lastY - (float)yPos; // reversed since y-coordinates go from bottom to top

		lastX = (float)xPos;
		lastY = (float)yPos;

		camera.ProcessMouseInputs(xoffset, yoffset, true);
	}
	else if (enableCameraLook == false)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
		
}