#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<assimp/version.h>

#include"Camera.h"
#include"Model.h"
#include"Lighting/DirectionalLight.h"
#include"Lighting/PointLight.h"
#include"Lighting/SpotLight.h"
#include"UI/IMGUI/IMGUI_DebugUI.h"


#define DebugUI_Register_Object(obj) debugUI.RegisterObject(&obj, #obj)
//OpenGL 4.6.0
//GLFW 3.3

float screenWidth = 1920;
float screenHeight = 1080;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime);
void Mouse_callback(GLFWwindow* window, Camera& camera, float window_Width, float window_Height, bool enableCameraLook);


int main()
{
	//Fields
	float currentFrame = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float fps = 1.0f / deltaTime;
	float fpsUpdateTimer = 0.0f;

	// Initialize GLFW 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	IMGUI_DebugUI debugUI;
	debugUI.Init(window);


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


	std::vector<Vertex> cubeVerts;
	std::vector<unsigned int> cubeInds;
	for (size_t i = 0; i < sizeof(cubeVertices) / sizeof(cubeVertices[0]); i += 11)
	{
		Vertex vertex;
		vertex.position = glm::vec3(cubeVertices[i], cubeVertices[i + 1], cubeVertices[i + 2]);
		vertex.normal = glm::vec3(cubeVertices[i + 3], cubeVertices[i + 4], cubeVertices[i + 5]);
		vertex.color = glm::vec3(cubeVertices[i + 6], cubeVertices[i + 7], cubeVertices[i + 8]);
		vertex.texCoords = glm::vec2(cubeVertices[i + 9], cubeVertices[i + 10]);
		cubeVerts.push_back(vertex);
	}
	cubeInds.assign(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));


	GLfloat planeVertices[] = {
		// positions          // normals        // colors        // texCoords
		// Bottom-left
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   0.0f, 0.0f,
		// Bottom-right
		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   1.0f, 0.0f,
		 // Top-right
		0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   1.0f, 1.0f,
		  // Top-left
	   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f,1.0f,1.0f,   0.0f, 1.0f,
	};

	GLuint planeIndices[] = {
		0, 1, 2,   // first triangle
		0, 2, 3    // second triangle
	};
	std::vector<Vertex> planeVerts;
	std::vector<unsigned int> planeInds;

	for (size_t i = 0; i < sizeof(planeVertices) / sizeof(planeVertices[0]); i += 11)
	{
		Vertex vertex;
		vertex.position = glm::vec3(planeVertices[i], planeVertices[i + 1], planeVertices[i + 2]);
		vertex.normal = glm::vec3(planeVertices[i + 3], planeVertices[i + 4], planeVertices[i + 5]);
		vertex.color = glm::vec3(planeVertices[i + 6], planeVertices[i + 7], planeVertices[i + 8]);
		vertex.texCoords = glm::vec2(planeVertices[i + 9], planeVertices[i + 10]);
		planeVerts.push_back(vertex);
	}

	planeInds.assign(planeIndices, planeIndices + sizeof(planeIndices) / sizeof(GLuint));


	//Shader
	Shader shader("Assets/Shaders/Default.vert.glsl", "Assets/Shaders/Default.frag.glsl");
	Shader lightShader("Assets/Shaders/Default.vert.glsl", "Assets/Shaders/LightSource.frag.glsl");


	//Model and mesh and mats
	Texture leaf("Assets/Textures/Leaf.jpeg", "diffuse");
	Texture container_Diffuse("Assets/Textures/Container.png", "diffuse");
	Texture container_Spec("Assets/Textures/Container_Specular.png", "specular");
	Texture board_Diffuse("Assets/Textures/CheckerBoard.jpg", "diffuse");
	Texture bag_Diffuse("Assets/Textures/Bag_Diffuse.jpg", "diffuse");
	Texture bag_Specular("Assets/Textures/Bag_Metalic.jpg", "specular");
	Texture c_Grass("Assets/Textures/C_Grass.jpg", "diffuse");
	Texture grass("Assets/Textures/grass.png", "diffuse");

	Material iron({ container_Diffuse, container_Spec }, 2.0f);
	Material leafMaterial({ leaf }, 32.0f);
	Material boardMaterial({ board_Diffuse }, 32.0f);
	Material bagMat({ bag_Diffuse, bag_Specular }, 8.0f);
	Material grassMat({ c_Grass }, 8.0f);
	Material grassM({ grass }, 8.0f);

	Mesh cubeMesh(cubeVerts, cubeInds, iron);
	Mesh planeMesh(planeVerts, planeInds, grassM);


	Transform cubeTransform(glm::vec3(0.0f, 0.0f, 0.0f));

	Model cubeModel({cubeMesh}, cubeTransform);
	cubeModel.transform.position = glm::vec3(-2.5f, 0.0f, -1.0f);
	Model lightModel({ cubeMesh }, cubeTransform);
	lightModel.transform.scale = glm::vec3(0.2f);
	Model grassMT({ planeMesh }, Transform(glm::vec3(0.0f, 0.0f, -2.0f)));


	Transform backPackTransform(glm::vec3(1.0f, 0.0f, 0.0f));
	Model brickCubeModel("Assets/Models/BrickCube.fbx", Transform(glm::vec3(2.0f, 0.0f, -1.0f)));
	Model buildingModel("Assets/Models/House.fbx", Transform(glm::vec3(0.0f, 1.5f, 0.0f)));
	Model grassPlaneModel("Assets/Models/GrassGround.fbx", Transform(glm::vec3(0.0f, -0.5f, 0.0f)));
	//Model grassModel("Assets/Models/Grass.fbx", Transform(glm::vec3(0.0f, 0.0f, -2.0f)));
	grassPlaneModel.transform.scale = glm::vec3(0.5f);
	//grassPlaneModel.meshes[0].SetMaterial(grassMat);

	buildingModel.transform.scale = glm::vec3(0.005f);
	DebugUI_Register_Object(buildingModel);
	DebugUI_Register_Object(brickCubeModel);
	DebugUI_Register_Object(cubeModel);
	DebugUI_Register_Object(grassPlaneModel);
	//DebugUI_Register_Object(grassModel);


	brickCubeModel.transform.scale = glm::vec3(0.005f);

	//Camera
	Transform cameraTransform(glm::vec3(0.0f, 0.0f, -3.0f));
	float fov = 60.0f;
	float clipNear = 0.01f;
	float clipFar = 100.0f;

	Camera camera(cameraTransform, fov, clipNear, clipFar);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.0f, 0.0f, 0.5f);
	glm::vec3 lightDir = glm::vec3(1.0f, 0.45f, 0.25f);

	//Light
	DirectionalLightManager directionalLights(0);
	PointLightManager pointLights(1);
	SpotLightManager spotLights(2);
 
	glm::vec3 pos = glm::vec3(-2.0f, 1.0f, -0.5f);
	glm::vec3 amb = glm::vec3(0.2f);
	glm::vec3 diff = glm::vec3(0.5f);
	glm::vec3 spec = glm::vec3(1.0f);
	float cons = 1.0f;
	float lin = 0.9f;
	float quad = 0.34f;
	lightModel.transform.position = pos;

	Transform lightTransform(pos);
	PointLight light(pointLights, lightTransform, amb, diff, spec, cons, lin, quad);
	DirectionalLight globalLight(directionalLights, lightTransform, glm::vec3(0.1f), glm::vec3(0.25f), glm::vec3(0.0f));

	glEnable(GL_DEPTH_TEST);
	std::cout << glGetString(GL_VERSION) << std::endl;


	// Update
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		fpsUpdateTimer += deltaTime;
		if (fpsUpdateTimer >= 0.25f)
		{ // update every 0.25s
			fps = 1.0f / deltaTime;
			fpsUpdateTimer = 0.0f;
		}


		ProcessInput(window, camera, deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		debugUI.BeginFrame();
		debugUI.RenderUI();
		

		
		glm::vec3 updateLightPos = glm::vec3(0.0f);
		updateLightPos.x = 1.0f * cos(glfwGetTime());
		updateLightPos.z = 1.0f * sin(glfwGetTime());

		shader.Use();
		shader.SetVec3("viewPos", camera.transform.position);
		shader.SetVec3("fogColor", glm::vec3(0.5f));
		shader.SetFloat("fogDensity", 0.01f);
		//iron.Apply(shader);
		//light.SetPosition(updateLightPos);

		glm::mat4 model = glm::mat4(1.0f);
		brickCubeModel.Draw(shader);
		buildingModel.Draw(shader);
		cubeModel.Draw(shader);
		//grassModel.Draw(shader);
		grassPlaneModel.Draw(shader);
		grassMT.Draw(shader);

		camera.BindToShader(shader, model, screenWidth / screenHeight);


		lightShader.Use();
		glm::mat4 lightModelMat = lightModel.transform.GetModelMatrix();
		lightModelMat = glm::translate(lightModelMat, updateLightPos);

		camera.BindToShader(lightShader, lightModelMat, screenWidth / screenHeight);
		lightShader.SetVec3("lightColor", lightColor);
		lightModel.Draw(lightShader);
		//lightModel.transform.position = updateLightPos;

		debugUI.EndFrame();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	lightShader.Delete();
	shader.Delete();
	pointLights.DeleteBuffer();
	directionalLights.DeleteBuffer();
	spotLights.DeleteBuffer();

	debugUI.Shutdown();
	glfwTerminate();
	
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
bool enableCameraLook = false;
void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
	//bool enableCameraLook = true;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.transform.position += camera.transform.Forward * 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.transform.position -= camera.transform.Forward * 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.transform.position -= camera.transform.Right * 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.transform.position += camera.transform.Right * 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		enableCameraLook = !enableCameraLook;
	}
	Mouse_callback(window, camera, screenWidth, screenHeight, enableCameraLook);
}

void Mouse_callback(GLFWwindow* window, Camera& camera,
	float window_Width, float window_Height,
	bool enableCameraLook)
{
	static bool cursorLocked = false;
	static bool firstMouse = true;
	static float lastX = window_Width / 2.0f;
	static float lastY = window_Height / 2.0f;
	static float yaw = 0.0f;
	static float pitch = 0.0f;

	if (enableCameraLook && !cursorLocked)
	{
		// Lock cursor once when enabling
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cursorLocked = true;
		firstMouse = true; // reset so camera doesn’t jump
	}
	else if (!enableCameraLook && cursorLocked)
	{
		// Unlock cursor once when disabling
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cursorLocked = false;
	}

	if (cursorLocked)
	{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		if (firstMouse)
		{
			lastX = (float)xPos;
			lastY = (float)yPos;
			firstMouse = false;
			return; // skip movement on first frame
		}

		float xoffset = (float)xPos - lastX ;
		float yoffset = (float)yPos - lastY;

		lastX = (float)xPos;
		lastY = (float)yPos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		glm::quat newRot = glm::quat(glm::radians(glm::vec3(pitch, yaw, 0.0f)));
		camera.SetRotation(newRot);
	}
}



