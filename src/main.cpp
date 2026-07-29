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
	Texture leaf("Assets/Textures/Leaf.jpeg", GL_RGB);

	Shader shader("Shaders/Default.vert.glsl", "Shaders/Default.frag.glsl");
	Shader lightShader("Shaders/Default.vert.glsl", "Shaders/LightSource.frag.glsl");

	GLfloat planeVertices[] = {
		// positions           // normals        // colors        // texCoords
		-1.0f, -0.51f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f, // bottom-left
		 1.0f, -0.51f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f, // bottom-right
		 1.0f, -0.51f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f, // top-right
		-1.0f, -0.51f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f  // top-left
	};
	GLuint planeIndices[] = {
		0, 1, 2,   // first triangle
		2, 3, 0    // second triangle
	};

	GLfloat cubeVertices[] = {
		// positions        // normals       // colors        // texCoords
		// Front face
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f,

		// Back face
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f,

		// Left face
		-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f,

		// Right face
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f,

		 // Top face
		 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f,

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f
	};
	GLuint cubeIndices[] = {
		// Front
		0, 1, 2, 2, 3, 0,
		// Back
		4, 5, 6, 6, 7, 4,
		// Left
		8, 9,10,10,11, 8,
		// Right
		12,13,14,14,15,12,
		// Top
		16,17,18,18,19,16,
		// Bottom
		20,21,22,22,23,20
	};
	
	Mesh plane(planeVertices, sizeof(planeVertices), planeIndices, sizeof(planeIndices), GL_STATIC_DRAW);
	Mesh otherCube(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), GL_STATIC_DRAW);
	Mesh lightube(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), GL_STATIC_DRAW);

	glm::vec3 camera_Pos = glm::vec3(0.0f, 0.0f, 3.0f);
	float fov = 60.0f;
	float clipNear = 0.01f;
	float clipFar = 100.0f;

	Camera camera(camera_Pos, fov, clipNear, clipFar);
	camera.SetSpeed(1.0f);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(3.0f, 3.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Update
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window, camera, deltaTime);

		glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::vec3 updateLightPos = lightPos;
		updateLightPos.x = 1.0f + sin(glfwGetTime()) * 5.0f;
		updateLightPos.y = sin(glfwGetTime() / 2.0f) * 2.0f;

		shader.Use();
		shader.SetVec3("viewPos", camera.Position);
		shader.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.SetFloat("material.shininess", 32.0f);
		shader.SetVec3("light.position", lightPos);
		shader.SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));


		glm::mat4 model = glm::mat4(1.0f);
		camera.BindToShader(shader, model, screenWidth / screenHeight);
		leaf.Bind();

		plane.Draw();
		leaf.Unbind();
		brick.Bind();
		otherCube.Draw();
		brick.Unbind();

		lightShader.Use();
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		camera.BindToShader(lightShader, lightModel, screenWidth / screenHeight);
		lightShader.SetVec3("lightColor", lightColor);
		lightube.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	plane.Delete();
	otherCube.Delete();
	lightube.Delete();
	lightShader.Delete();
	brick.Delete();
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