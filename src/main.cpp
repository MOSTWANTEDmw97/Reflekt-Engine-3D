#include<iostream>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<assimp/version.h>
#include<imgui.h>

#include"UI/IMGUI/IMGUI_DebugUI.h"
#include"Graphics/ShaderManager.h"
#include"Rendering/Renderer.h"
#include "Rendering/RenderPipeline.h"


//Pre-Processor for debug ui string
#define DebugUI_Register_Object(obj) debugUI.RegisterObject(&obj, #obj)
//OpenGL 4.6.0
//GLFW 3.3

int screenWidth = 1920;
int screenHeight = 1080;

float depthMapSize = 4096;

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

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Reflekt Engine 3D", NULL, NULL);
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
	Shader shader("Default_Shaders/geometryPass.shader");
	Shader depthMapShader("Default_Shaders/DepthMap.shader");
	Shader PostProcess("Default_Shaders/PostPass.shader");
	Shader transparentLit("Default_Shaders/transparent_lit.shader");

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

	/*
	Material iron(&shader, { container_Diffuse, container_Spec }, 32.0f);
	Material leafMaterial(&shader, { leaf }, 32.0f);
	Material boardMaterial(&shader, { board_Diffuse }, 32.0f);
	Material bagMat(&shader, { bag_Diffuse, bag_Specular }, 8.0f);
	Material grassMat(&shader, { c_Grass }, 8.0f);
	Material grassM(&shader, { grass }, 8.0f);
	Material windowMat(&transparentLit, { windowTex }, 16.0f, SurfaceType::Transparent);
	*/
	// Iron material
	Material iron(&shader);
	iron.SetTexture("material.diffuse", &container_Diffuse, 0);
	iron.SetTexture("material.specular", &container_Spec, 1);
	iron.SetFloat("material.shininess", 128.0f);

	// Leaf material
	Material leafMaterial(&shader);
	leafMaterial.SetTexture("material.diffuse", &leaf, 0);
	leafMaterial.SetFloat("material.shininess", 32.0f);

	// Board material
	Material boardMaterial(&shader);
	boardMaterial.SetTexture("material.diffuse", &board_Diffuse, 0);
	boardMaterial.SetFloat("material.shininess", 32.0f);

	// Bag material
	Material bagMat(&shader);
	bagMat.SetTexture("material.diffuse", &bag_Diffuse, 0);
	bagMat.SetTexture("material.specular", &bag_Specular, 1);
	bagMat.SetFloat("material.shininess", 8.0f);
	//
	// Grass material (two variants)
	Material grassMat(&shader);
	grassMat.SetTexture("material.diffuse", &c_Grass, 0);
	grassMat.SetFloat("material.shininess", 8.0f);

	Material grassM(&shader);
	grassM.SetTexture("material.diffuse", &grass, 0);
	grassM.SetFloat("material.shininess", 8.0f);

	// Transparent window material
	Material windowMat(&transparentLit);
	windowMat.SetTexture("material.diffuse", &windowTex, 0);
	windowMat.SetFloat("material.shininess", 16.0f);
	// You can also set transparency flags via a property or uniform
	windowMat.surfaceType = SurfaceType::Transparent;
	windowMat.blendMode = BlendMode::Alpha;


	windowMat.cullMode = CullMode::None;
	grassM.cullMode = CullMode::None;
	//iron.shaderRef = &lightShader;
	//Material lightMat(&lightShader, {}, 0.0f);
	Mesh cubeMesh(cubeVerts, cubeInds);
	Mesh planeMesh(planeVerts, planeInds);


	Transform cubeTransform(glm::vec3(0.0f, 0.0f, 0.0f));

	Model cubeModel({cubeMesh});
	Model lightModel({ cubeMesh });
	
	Model grassMT({ planeMesh });

	Model windowModel({ planeMesh });
	Model windowModel2({ planeMesh });
	Model windowModel3({ planeMesh });

	Transform backPackTransform(glm::vec3(1.0f, 0.0f, 0.0f));
	Model brickCubeModel("Assets/Models/BrickCube.fbx");
	Model buildingModel("Assets/Models/House.fbx");
	Model grassPlaneModel("Assets/Models/GrassGround.fbx");
	//Model grassModel("Assets/Models/Grass.fbx", Transform(glm::vec3(0.0f, 0.0f, -2.0f)));
	//grassPlaneModel.meshes[0].SetMaterial(grassMat);

	//Instance transforms
	std::vector<glm::mat4> transforms;
	int instanceCount = 100;

	int gridSize = static_cast<int>(sqrt(instanceCount));

	for (int i = 0; i < instanceCount; i++)
	{
		int row = i / gridSize;   // Z axis
		int col = i % gridSize;   // X axis

		glm::vec3 pos = glm::vec3((float)col, 0.0f, (float)row);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

		// optional: random rotation for grass
		float angle = (rand() % 360);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		transforms.push_back(model);
	}


	//Camera
	Transform cameraTransform(glm::vec3(0.0f, 0.0f, -3.0f));
	float fov = 60.0f;
	float clipNear = 0.01f;
	float clipFar = 1000.0f;

	Camera camera(cameraTransform, fov, clipNear, clipFar, &skybox);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.0f, 0.0f, 0.5f);
	glm::vec3 lightDir = glm::vec3(1.0f, 0.45f, 0.25f);


	glm::vec3 pos = glm::vec3(-2.0f, 1.0f, -0.5f);
	glm::vec3 amb = glm::vec3(0.2f);
	glm::vec3 diff = glm::vec3(0.5f);
	glm::vec3 spec = glm::vec3(1.0f);
	float cons = 1.0f;
	float lin = 0.9f;
	float quad = 0.34f;

	Transform lightTransform(pos);


	LightManager lightManager;
	
	Transform tr(glm::vec3(1.0f));

	glm::vec3 lDIr =
		glm::vec3(0.0f,  0.0f, 0.5f);

	
	Light dirLight(
		lightManager,
		LightType::directionalLight,
		tr,
		lDIr,
		glm::vec3(0.25f),
		glm::vec3(0.4f),
		glm::vec3(1.25f),
		0.25f
	);

	
	Light pointLight(
		lightManager,
		LightType::pointLight,
		lightTransform,
		glm::vec3(0.0f, 3.0f, 0.0f),
		glm::vec3(0.2f),
		glm::vec3(0.5f),
		glm::vec3(1.0f),
		1.0f,
		glm::vec3(1.0, 0.7, 0.034)
	);
	
	ScreenQuad screenQuad;




	GameObject building;
	building.AddComponent<MeshRenderer>(buildingModel);
	building.transform.scale = glm::vec3(0.005f);
	building.transform.position = glm::vec3(0.0f, 0.0f, 5.0f);

	Shader randShad("Default_Shaders/default_Lit.shader");
	GameObject ironCube;
	ironCube.AddComponent<MeshRenderer>(cubeModel);
	ironCube.GetComponent<MeshRenderer>()->SetMaterial(iron);
	//ironCube.GetComponent<MeshRenderer>()->material->shaderRef = &randShad;
	ironCube.transform.position = glm::vec3(3.0f, 0.0f, 0.0f);

	GameObject ground;
	ground.AddComponent<MeshRenderer>(grassPlaneModel);
	ground.transform.position = glm::vec3(0.0f, -2.0f, 0.0f);
	ground.transform.scale = glm::vec3(0.01f);

	GameObject brickCube;
	brickCube.AddComponent<MeshRenderer>(brickCubeModel);
	brickCube.transform.scale = glm::vec3(0.005f);

	GameObject brickCube1;
	brickCube1.AddComponent<MeshRenderer>(brickCubeModel);
	brickCube1.GetComponent<MeshRenderer>()->material = new Material(windowMat);
	brickCube1.transform.scale = glm::vec3(0.005f);
	brickCube1.transform.position = glm::vec3(3.0f);
	brickCube1.GetComponent<MeshRenderer>()->material->surfaceType = SurfaceType::Transparent;

	GameObject brickCube2;
	brickCube2.AddComponent<MeshRenderer>(brickCubeModel);
	brickCube2.transform.scale = glm::vec3(0.005f);
	brickCube2.GetComponent<MeshRenderer>()->material = new Material(windowMat);
	brickCube2.transform.position = glm::vec3(5.0f);
	brickCube2.GetComponent<MeshRenderer>()->material->surfaceType = SurfaceType::Transparent;
	brickCube2.GetComponent<MeshRenderer>()->material->blendMode = BlendMode::PreMultiply;


	GameObject lightobj;
	//lightobj.AddComponent<LightComponent>(dirLight);

	GameObject pointLightObj;
	pointLightObj.AddComponent<LightComponent>(pointLight);

	camera.skybox = &skybox;

	GameObject windowObj;
	windowObj.AddComponent<MeshRenderer>(windowModel);
	windowObj.GetComponent<MeshRenderer>()->SetMaterial(windowMat);
	//windowObj.GetComponent<MeshRenderer>()->material->shaderRef = &randShad;
	windowObj.transform.position = glm::vec3(0.0f, 2.0f, -2.0f);

	GameObject windowObj1;
	windowObj1.AddComponent<MeshRenderer>(windowModel);
	windowObj1.GetComponent<MeshRenderer>()->SetMaterial(windowMat);
	windowObj1.transform.position = glm::vec3(4.0f);

	Scene scene;
	scene.SetCamera(&camera);
	scene.AddGameObject(ironCube);
	scene.AddGameObject(brickCube);
	scene.AddGameObject(windowObj);
	scene.AddGameObject(lightobj);
	scene.AddGameObject(pointLightObj);
	scene.AddGameObject(building);
	scene.AddGameObject(ground);
	scene.AddGameObject(windowObj1);
	scene.AddGameObject(brickCube1);
	scene.AddGameObject(brickCube2);
	

	std::vector<float> frameTimes;
	const int maxSamples = 1000;

	glm::vec3 shadowlightPosition(0.0f, 10.0f, -5.0f);

	float depthnear_plane = 1.0f, depthfar_plane = 30.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, depthnear_plane, depthfar_plane);


	glm::mat4 lightView = glm::lookAt(shadowlightPosition, glm::vec3(0.0f), glm::vec3(0, 1, 0));
	glm::mat4 flipX = glm::scale(glm::mat4(1.0f), glm::vec3(-1, 1, 1));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView * flipX;

	Renderer renderer(screenWidth, screenHeight);

	//DebugUI_Register_Object(brickCube);

	glEnable(GL_DEPTH_TEST);

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

		glClearColor(0.0, 0.0, 0.0, 1.0);

		
		ProcessInput(window, camera, deltaTime);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.Render(scene);
		//scene.activeCamera->BindToShader(*scene.activeCamera->skybox->shader, glm::mat4(1.0f), (float)screenWidth / (float)screenHeight);
		//scene.activeCamera->skybox->Draw();
		//scene.DrawAllGeometry((float)screenWidth, (float)screenHeight, &transparentLit);

		debugUI.BeginFrame();
		debugUI.RenderPerfMetrics(frameTimes);
		debugUI.RenderCameraPosition(camera.transform.position);
		debugUI.RenderUI();


		debugUI.EndFrame();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shader.Delete();
	lightManager.DeleteBuffer();

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


