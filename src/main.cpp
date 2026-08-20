#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<assimp/version.h>
#include<imgui.h>

#include"Camera.h"
#include"Model.h"
#include"Graphics/ShaderManager.h"
#include"Lighting/DirectionalLight.h"
#include"Lighting/PointLight.h"
#include"Lighting/SpotLight.h"
#include"UI/IMGUI/IMGUI_DebugUI.h"
#include"Rendering/ScreenQuad.h"
#include"Rendering/RenderPipeline.h"
#include"Graphics/Cubemap.h"
#include"Rendering/Skybox.h"

#include"GLBuffers/FBO.h"
#include<string>

#define DebugUI_Register_Object(obj) debugUI.RegisterObject(&obj, #obj)
//OpenGL 4.6.0
//GLFW 3.3

float screenWidth = 1920;
float screenHeight = 1080;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime);
void Mouse_callback(GLFWwindow* window, Camera& camera, float window_Width, float window_Height, bool enableCameraLook);


struct PerfStats
{
	float fps;
	float frameTimeMs;
	float fps1Low;
	float fps01Low;
};

PerfStats CalculatePerfStats(const std::vector<float>& frameTimes)
{
	PerfStats stats{};
	if (frameTimes.empty()) return stats;

	float latest = frameTimes.back();
	stats.fps = 1.0f / latest;
	stats.frameTimeMs = latest * 1000.0f;

	std::vector<float> sorted = frameTimes;
	std::sort(sorted.begin(), sorted.end());
	int n = (int)sorted.size();

	float p99 = sorted[(int)(n * 0.99f)];
	float p999 = sorted[(int)(n * 0.999f)];

	stats.fps1Low = 1.0f / p99;
	stats.fps01Low = 1.0f / p999;

	return stats;
}

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
	FBO frameBuffer(screenWidth, screenHeight);

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
		0, 2, 1, 0, 3, 2,
		// Back
		4, 5, 6, 4, 6, 7,
		// Left
		8, 10, 9, 8, 11, 10,
		// Right
		12, 13, 14, 12, 14, 15,
		// Top
		16, 17, 18, 16, 18, 19,
		// Bottom
		20, 22, 21, 20, 23, 22
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
	//Shader shader("Assets/Shaders/Default.vert.glsl", "Assets/Shaders/Default.frag.glsl");
	Shader shader("Default_Shaders/Default.shader");
	Shader lightShader("Assets/Shaders/Default.vert.glsl", "Assets/Shaders/LightSource.frag.glsl");
	ShaderManager::Register(shader, "default");
	ShaderManager::Register(lightShader, "light");

	//Model and mesh and mats
	Texture leaf("Assets/Textures/Leaf.jpeg", "diffuse");
	Texture container_Diffuse("Assets/Textures/Container.png", "diffuse");
	Texture container_Spec("Assets/Textures/Container_Specular.png", "specular");
	Texture board_Diffuse("Assets/Textures/CheckerBoard.jpg", "diffuse");
	Texture bag_Diffuse("Assets/Textures/Bag_Diffuse.jpg", "diffuse");
	Texture bag_Specular("Assets/Textures/Bag_Metalic.jpg", "specular");
	Texture c_Grass("Assets/Textures/C_Grass.jpg", "diffuse");
	Texture grass("Assets/Textures/grass.png", "diffuse");
	Texture windowTex("Assets/Textures/Window.png", "diffuse");


	//Skybox init
	std::vector<std::string> faces = {
		"Assets/Textures/Skybox/right.jpg",
		"Assets/Textures/Skybox/left.jpg",
		"Assets/Textures/Skybox/top.jpg",
		"Assets/Textures/Skybox/bottom.jpg",
		"Assets/Textures/Skybox/front.jpg",
		"Assets/Textures/Skybox/back.jpg"
	};
	Cubemap skyboxCubemap(faces);

	Shader skyboxShader("Default_Shaders/Skybox.shader");
	Skybox skybox(&skyboxCubemap, &skyboxShader);

	Material iron(&shader, { container_Diffuse, container_Spec }, 2.0f);
	Material leafMaterial(&shader, { leaf }, 32.0f);
	Material boardMaterial(&shader, { board_Diffuse }, 32.0f);
	Material bagMat(&shader, { bag_Diffuse, bag_Specular }, 8.0f);
	Material grassMat(&shader, { c_Grass }, 8.0f);
	Material grassM(&shader, { grass }, 8.0f);
	Material windowMat(&shader, { windowTex }, 16.0f, SurfaceType::Transparent);
	windowMat.cullMode = CullMode::None;
	grassM.cullMode = CullMode::None;
	//iron.shaderRef = &lightShader;
	Material lightMat(&lightShader, {}, 0.0f);
	Mesh cubeMesh(cubeVerts, cubeInds, iron);
	Mesh planeMesh(planeVerts, planeInds, grassM);


	Transform cubeTransform(glm::vec3(0.0f, 0.0f, 0.0f));

	Model cubeModel({cubeMesh}, cubeTransform);
	cubeModel.transform.position = glm::vec3(-2.5f, 0.0f, -1.0f);
	Model lightModel({ cubeMesh }, cubeTransform);
	lightModel.transform.scale = glm::vec3(0.2f);
	lightModel.meshes[0].SetMaterial(lightMat);
	
	Model grassMT({ planeMesh }, Transform(glm::vec3(0.0f, 0.0f, -2.0f)));

	Model windowModel({ planeMesh }, Transform(glm::vec3(1.0f, 0.0f, -2.0f)));
	Model windowModel2({ planeMesh }, Transform(glm::vec3(1.0f, 0.0f, -2.5f)));
	Model windowModel3({ planeMesh }, Transform(glm::vec3(1.0f, 0.0f, -3.0f)));
	windowModel.meshes[0].SetMaterial(windowMat);
	windowModel2.meshes[0].SetMaterial(windowMat);
	windowModel3.meshes[0].SetMaterial(windowMat);

	Transform backPackTransform(glm::vec3(1.0f, 0.0f, 0.0f));
	Model brickCubeModel("Assets/Models/BrickCube.fbx", Transform(glm::vec3(2.0f, 0.0f, -1.0f)));
	Model buildingModel("Assets/Models/House.fbx", Transform(glm::vec3(0.0f, 1.5f, 0.0f)));
	Model grassPlaneModel("Assets/Models/GrassGround.fbx", Transform(glm::vec3(0.0f, -0.5f, 0.0f)));
	//Model grassModel("Assets/Models/Grass.fbx", Transform(glm::vec3(0.0f, 0.0f, -2.0f)));
	grassPlaneModel.transform.scale = glm::vec3(0.005f);
	//grassPlaneModel.meshes[0].SetMaterial(grassMat);

	buildingModel.transform.scale = glm::vec3(0.005f);
	DebugUI_Register_Object(buildingModel);
	DebugUI_Register_Object(brickCubeModel);
	DebugUI_Register_Object(cubeModel);
	DebugUI_Register_Object(grassPlaneModel);
	DebugUI_Register_Object(windowModel);
	DebugUI_Register_Object(windowModel2);
	DebugUI_Register_Object(windowModel3);
	//DebugUI_Register_Object(grassModel);


	brickCubeModel.transform.scale = glm::vec3(0.005f);

	//Camera
	Transform cameraTransform(glm::vec3(0.0f, 0.0f, -3.0f));
	float fov = 60.0f;
	float clipNear = 0.01f;
	float clipFar = 1000.0f;

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
	DirectionalLight globalLight(directionalLights, lightTransform, glm::vec3(0.15f), glm::vec3(0.35f), glm::vec3(0.0f));

	Shader PostProcess("Default_Shaders/PostPass.shader");
	
	
	ScreenQuad screenQuad;


	RenderPipeline renderPipiline;
	renderPipiline.AddModel(windowModel);
	renderPipiline.AddModel(windowModel2);
	renderPipiline.AddModel(windowModel3);
	renderPipiline.AddModel(brickCubeModel);
	renderPipiline.AddModel(buildingModel);
	renderPipiline.AddModel(cubeModel);
	renderPipiline.AddModel(grassMT);
	renderPipiline.AddModel(grassPlaneModel);
	renderPipiline.SetCamera(&camera);
	std::cout << glGetString(GL_VERSION) << std::endl;


	std::vector<float> frameTimes;
	const int maxSamples = 1000;


	// Update
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		frameTimes.push_back(deltaTime);
		if (frameTimes.size() > maxSamples)
			frameTimes.erase(frameTimes.begin());

		if (fpsUpdateTimer >= 0.25f)
		{ // update every 0.25s
			fps = 1.0f / deltaTime;
			fpsUpdateTimer = 0.0f;
		}


		ProcessInput(window, camera, deltaTime);



		//Scene OR Pre Pass
		frameBuffer.Bind();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.BindToShader(skyboxShader, glm::mat4(1.0f), screenWidth / screenHeight);
		skybox.Draw();
		
		glEnable(GL_DEPTH_TEST);


		debugUI.BeginFrame();
		debugUI.RenderUI();
		

		
		glm::vec3 updateLightPos = glm::vec3(0.0f);
		updateLightPos.x = 1.0f * cos(glfwGetTime());
		updateLightPos.z = 1.0f * sin(glfwGetTime());


		glm::mat4 model = glm::mat4(1.0f);
		camera.BindToShader(shader, model, screenWidth / screenHeight);




		shader.Use();
		shader.SetVec3("viewPos", camera.transform.position);
		//brickCubeModel.Draw();
		renderPipiline.DrawAll();
		//renderPipiline.DrawOpaque(camera);
		//renderPipiline.DrawTransparent(camera);



		lightShader.Use();
		glm::mat4 lightModelMat = lightModel.transform.GetModelMatrix();
		lightModelMat = glm::translate(lightModelMat, updateLightPos);

		camera.BindToShader(lightShader, lightModelMat, screenWidth / screenHeight);
		lightShader.SetVec3("lightColor", lightColor);
		lightModel.Draw();
		//lightModel.transform.position = updateLightPos;

		frameBuffer.Unbind();


		// second pass
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Post pass Scene
		PostProcess.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBuffer.colorTex);
		PostProcess.SetInt("screenTex", 0);

		screenQuad.Draw();



		PerfStats stats = CalculatePerfStats(frameTimes);

		ImGui::SetNextWindowPos(ImVec2(1700, 850));
		ImGui::SetNextWindowBgAlpha(0.35f);
		ImGui::Begin("PerfOverlay", nullptr,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

		ImGui::Text("FPS: %.1f", stats.fps);
		ImGui::Text("Frame Time: %.2f ms", stats.frameTimeMs);
		ImGui::Text("1%% Low FPS: %.1f", stats.fps1Low);
		ImGui::Text("0.1%% Low FPS: %.1f", stats.fps01Low);

		ImGui::End();




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



