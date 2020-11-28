#include <iostream>
#include <functional>
#include "OpenGLUtils/OpenGLUtils.h"

// Lesson Getting Familar with using lighting materials
//still a wip
int main() {
	const int initialScreenWidth = 2560;
	const int initialScreenHeight = 1440;

	glm::mat4 projection;
	glm::mat4 view;

	glm::vec3 cameraPos(0.0f, 0.0f, 5.0f);
	FlyingFPSCamera fpsCamera(cameraPos, glm::vec3(0.0f, 1.0f, 0.0f), initialScreenWidth, initialScreenHeight, 90.0f, -90.0f);
	//FlyingCamera fpsCamera(cameraPos, initialScreenWidth, initialScreenHeight, 90.0f, -90.0f);

	// setting up callback listeners since OpenGL allows for only one registered callback function at a time
	framebufferResizeEvent += std::bind(&FlyingFPSCamera::WindowResizeEvent, &fpsCamera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	framebufferResizeEvent += [&projection, &fpsCamera](GLFWwindow* window, int width, int height) {
		projection = (glm::mat4)glm::perspectiveFov(glm::radians(fpsCamera.FOV()), (float)width, (float)height, 0.1f, 100.0f);
	};

	mouseMovementEvent += std::bind(&FlyingFPSCamera::MouseMovementEvent, &fpsCamera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	mouseScrollEvent += std::bind(&FlyingFPSCamera::MouseScrollEvent, &fpsCamera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	mouseScrollEvent += [&projection, &fpsCamera](GLFWwindow* window, double xOffset, double yOffset) {
		projection = glm::perspectiveFov(glm::radians(fpsCamera.FOV()), (float)fpsCamera.Width(), (float)fpsCamera.Height(), 0.1f, 1000.0f);
	};

	InitGLFW(3, 3);

	//Create a window for glfw
	GLFWwindow* window = glfwCreateWindow(initialScreenWidth, initialScreenHeight, "LearnOpenGL Model Loading", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	else {
		glfwMakeContextCurrent(window);
		//setup necessary callback functions
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_Movement_Callback);
		glfwSetScrollCallback(window, mouse_Scroll_Callback);
	}

	//Initialize GLAD (load all OpenGL function pointers)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD!\n";
		return -1;
	}

	//set openGL to take into consideration the depth buffer before drawing a pixel
	glEnable(GL_DEPTH_TEST);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	//Declare and compile shaders
	Shader shader("res/shaders/BasicShaders/TransformationShaders/MVP.vert", "res/shaders/BasicShaders/SimpleTexture.frag");
	Shader flatColorShader("res/shaders/BasicShaders/TransformationShaders/MVPInstancing.vert", "res/shaders/BasicShaders/FlatColor.frag");
	Shader shaderInstancing("res/shaders/BasicShaders/TransformationShaders/MVPInstancing.vert", "res/shaders/Lighting/Phong-Cumulative.frag");
	//Shader shaderInstancing("res/shaders/BasicShaders/TransformationShaders/MVPInstancing.vert", "res/shaders/BasicShaders/simpleTexture.frag");

	Material lightingMaterial(shaderInstancing);
	Material unlitMaterial(shader);
	Material flatColorMaterial(flatColorShader);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[36][8] = {
		// positions              //normals                    // texture Coords
		{-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f},
		{ 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f},
		{ 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f},
		{-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f},
		{-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f},

		{-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 0.0f},
		{ 0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    1.0f, 0.0f },
		{ 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    1.0f, 1.0f},
		{ 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    1.0f, 1.0f},
		{-0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 1.0f},
		{-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 0.0f},

		{-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
		{-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
		{-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
		{-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
		{-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
		{-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f},

		{ 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
		{ 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
		{ 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
		{ 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
		{ 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
		{ 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f},

		{-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f},
		{ 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f},
		{ 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f},
		{ 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f},
		{-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f},
		{-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f},

		{-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f},
		{ 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f},
		{ 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f},
		{ 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f},
		{-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f},
		{-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f}
	};

	std::vector<unsigned int> cubeIndices(36);
	for (unsigned int i = 0; i < 36; i++) {
		cubeIndices[i] = i;
	}

	float planeVertices[6][5] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		{ 5.0f, -0.5f,  5.0f,  1.0f, 0.0f},
		{-5.0f, -0.5f,  5.0f,  0.0f, 0.0f},
		{-5.0f, -0.5f, -5.0f,  0.0f, 1.0f},

		{ 5.0f, -0.5f,  5.0f,  1.0f, 0.0f},
		{-5.0f, -0.5f, -5.0f,  0.0f, 1.0f},
		{ 5.0f, -0.5f, -5.0f,  1.0f, 1.0f}
	};

	std::vector<unsigned int> planeIndices(6);
	for (unsigned int i = 0; i < 6; i++) {
		planeIndices[i] = i;
	}

	std::vector<Vertex> vertices;
	for (int i = 0; i < 36; i++) {
		Vertex vertex;
		vertex.Position.x = cubeVertices[i][0];
		vertex.Position.y = cubeVertices[i][1];
		vertex.Position.z = cubeVertices[i][2];

		vertex.Normal.x = cubeVertices[i][3];
		vertex.Normal.y = cubeVertices[i][4];
		vertex.Normal.z = cubeVertices[i][5];

		vertex.TexCoord.x = cubeVertices[i][6];
		vertex.TexCoord.y = cubeVertices[i][7];

		vertices.push_back(vertex);
	}

	Texture texture;
	texture.type = TextureType::DIFFUSE;
	texture.id = loadTexture("res/Textures/woodenContainer.png");
	texture.path = "res/Textures";


	Mesh cubeMesh(vertices, cubeIndices, { texture });
	Mesh lightMesh(vertices, cubeIndices, {});

	vertices.clear();
	for (int i = 0; i < 6; i++) {
		Vertex vertex;
		vertex.Position.x = planeVertices[i][0];
		vertex.Position.y = planeVertices[i][1];
		vertex.Position.z = planeVertices[i][2];

		vertex.TexCoord.x = planeVertices[i][3];
		vertex.TexCoord.y = planeVertices[i][4];

		vertices.push_back(vertex);
	}

	texture.type = TextureType::DIFFUSE;
	texture.id = loadTexture("res/Textures/container.jpg");
	texture.path = "res/Textures";

	Mesh planeMesh(vertices, planeIndices, { texture });

	float quadVertices[6][5] =
	{
		{-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
		{ 0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
		{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},

		{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
		{-0.5f,  0.5f,  0.5f,  0.0f, 1.0f},
		{-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
	};

	vertices.clear();
	for (int i = 0; i < 6; i++)
	{
		Vertex vertex;
		vertex.Position.x = quadVertices[i][0];
		vertex.Position.y = quadVertices[i][1];
		vertex.Position.z = quadVertices[i][2];

		vertex.TexCoord.x = quadVertices[i][3];
		vertex.TexCoord.y = quadVertices[i][4];

		vertices.push_back(vertex);
	}

	TextureProps grassProps{ "res/Textures/grass.png", true, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
	texture.type = TextureType::DIFFUSE;
	texture.id = loadTexture(grassProps);
	texture.path = "res/Textures";

	Mesh vegetationMesh(vertices, planeIndices, { texture });

	lightMesh.SetMaterial(flatColorMaterial);
	cubeMesh.SetMaterial(lightingMaterial);
	cubeMesh.SetUniformContainer("material");
	planeMesh.SetMaterial(lightingMaterial);
	vegetationMesh.SetMaterial(lightingMaterial);

	//Model  backpackModel((char*)"res/Models/Asteroid/rock.obj");
	Model  backpackModel((char*)"res/Models/NanoSuit/nanosuit.obj");
	backpackModel.SetMaterial(lightingMaterial);
	backpackModel.SetUniformContainerName("material");

	float deltaTime = 0;
	float lastFrame = (float)glfwGetTime();

	lightingMaterial.AddUniform("view", UniformTypes::MAT4, glm::mat4(1.0f));
	lightingMaterial.AddUniform("projection", UniformTypes::MAT4, glm::mat4(2.0f));

	Renderer::Init();

	std::vector<glm::mat4> nanoSuitTransforms;
	int rows = 100;
	int cols = 25;
	nanoSuitTransforms.resize(rows * cols);
	for (int x = 0; x < rows; x++)
	{
		for (int z = 0; z < cols; z++)
		{
			int index = z + x * cols;
			nanoSuitTransforms[index] = glm::translate(glm::mat4(1.0f), glm::vec3(x * 10.0f, 0.0f, z * 5.0f));
			nanoSuitTransforms[index] = glm::rotate(nanoSuitTransforms[index], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}


	int numberOfPointLights = 4;
	float shininess = 32.0f;
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  10.0f,  -3.0f),
		glm::vec3(2.3f,  10.0f, -4.0f),
		glm::vec3(4.0f,  10.0f, 12.0f),
		glm::vec3(0.0f,  10.0f, -1.0f)
	};
	glm::vec3 pointLightColors[] =
	{
		glm::vec3(0.5f,  0.8f,  0.2f),
		glm::vec3(0.8f,  0.8f,  0.8f),
		glm::vec3(0.8f,  0.8f,  0.8f),
		glm::vec3(0.8f,  0.8f,  0.8f)
	};

	std::vector<glm::mat4> pointLightTransforms(numberOfPointLights);
	for (int light = 0; light < numberOfPointLights; light++)
	{
		pointLightTransforms[light] = glm::translate(glm::mat4(1.0f), pointLightPositions[light]);
		//flatColorMaterial.AddUniform("pointLights[" + std::to_string(light) + "].position", UniformTypes::FLOAT3, pointLightPositions[light]);
	}


	projection = glm::perspectiveFov(glm::radians(fpsCamera.FOV()), (float)fpsCamera.Width(), (float)fpsCamera.Height(), 0.1f, 1000.0f);
	while (!glfwWindowShouldClose(window)) {
		//check input
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window);
		processCameraInput(window, fpsCamera, deltaTime);

		Renderer::BeginScene(projection, fpsCamera);

		lightingMaterial.GetShader()->Bind();
		int useInstancing = 1;
		lightingMaterial.AddUniform("useInstancing", UniformTypes::INT, useInstancing);
		flatColorMaterial.AddUniform("useInstancing", UniformTypes::INT, useInstancing);
		


		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = fpsCamera.GenerateViewMatrix();

		glm::mat4 model(1.0f);

		//Renderer::Draw(backPackMeshPtrs, nanoSuitTransforms);
		Renderer::Draw(backpackModel.GetMeshes(), nanoSuitTransforms);

		

		lightingMaterial.AddUniform("viewPos", UniformTypes::FLOAT3, fpsCamera.Position());
		lightingMaterial.AddUniform("numberPointLightsPassed", UniformTypes::INT, numberOfPointLights);
		lightingMaterial.AddUniform("material.shininess", UniformTypes::FLOAT, shininess);

		//setting up directional light properties
		lightingMaterial.AddUniform("directionalLight.direction", UniformTypes::FLOAT3, glm::vec3(-0.2f, -1.0f, 0.3f));
		lightingMaterial.AddUniform("directionalLight.ambientStrength", UniformTypes::FLOAT3, glm::vec3(0.2f, 0.2f, 0.2f));
		lightingMaterial.AddUniform("directionalLight.diffuseStrength", UniformTypes::FLOAT3, glm::vec3(0.5f, 0.5f, 0.5f));
		lightingMaterial.AddUniform("directionalLight.specularStrength", UniformTypes::FLOAT3, glm::vec3(0.5f, 0.5f, 0.5f));

		// point AddUniformlight 1
		lightingMaterial.AddUniform("pointLights[0].position", UniformTypes::FLOAT3, pointLightPositions[0]);
		lightingMaterial.AddUniform("pointLights[0].ambientStrength", UniformTypes::FLOAT3 , glm::vec3(0.05f, 0.05f, 0.05f));
		lightingMaterial.AddUniform("pointLights[0].diffuseStrength", UniformTypes::FLOAT3, pointLightColors[0]);
		lightingMaterial.AddUniform("pointLights[0].specularStrength", UniformTypes::FLOAT3, glm::vec3(1.0f, 1.0f, 1.0f));
		lightingMaterial.AddUniform("pointLights[0].constant", UniformTypes::FLOAT,  1.0f);
		lightingMaterial.AddUniform("pointLights[0].linear", UniformTypes::FLOAT, 0.09f);
		lightingMaterial.AddUniform("pointLights[0].quadratic", UniformTypes::FLOAT, 0.032f);
		// point light 2
		lightingMaterial.AddUniform("pointLights[1].position", UniformTypes::FLOAT3, pointLightPositions[1]);
		lightingMaterial.AddUniform("pointLights[1].ambientStrength", UniformTypes::FLOAT3, glm::vec3(0.05f, 0.05f, 0.05f));
		lightingMaterial.AddUniform("pointLights[1].diffuseStrength", UniformTypes::FLOAT3, pointLightColors[1]);
		lightingMaterial.AddUniform("pointLights[1].specularStrength", UniformTypes::FLOAT3, glm::vec3(1.0f, 1.0f, 1.0f));
		lightingMaterial.AddUniform("pointLights[1].constant", UniformTypes::FLOAT, 1.0f);
		lightingMaterial.AddUniform("pointLights[1].linear", UniformTypes::FLOAT, 0.09f);
		lightingMaterial.AddUniform("pointLights[1].quadratic", UniformTypes::FLOAT, 0.032f);
		//// point light 3
		lightingMaterial.AddUniform("pointLights[2].position", UniformTypes::FLOAT3, pointLightPositions[2]);
		lightingMaterial.AddUniform("pointLights[2].ambientStrength", UniformTypes::FLOAT3, glm::vec3(0.05f, 0.05f, 0.05f));
		lightingMaterial.AddUniform("pointLights[2].diffuseStrength", UniformTypes::FLOAT3, pointLightColors[2]);
		lightingMaterial.AddUniform("pointLights[2].specularStrength", UniformTypes::FLOAT3, glm::vec3(1.0f, 1.0f, 1.0f));
		lightingMaterial.AddUniform("pointLights[2].constant", UniformTypes::FLOAT, 1.0f);
		lightingMaterial.AddUniform("pointLights[2].linear", UniformTypes::FLOAT, 0.09f);
		lightingMaterial.AddUniform("pointLights[2].quadratic", UniformTypes::FLOAT, 0.032f);
		//// point light 4
		lightingMaterial.AddUniform("pointLights[3].position", UniformTypes::FLOAT3, pointLightPositions[3]);
		lightingMaterial.AddUniform("pointLights[3].ambientStrength", UniformTypes::FLOAT3, glm::vec3(0.05f, 0.05f, 0.05f));
		lightingMaterial.AddUniform("pointLights[3].diffuseStrength", UniformTypes::FLOAT3, pointLightColors[3]);
		lightingMaterial.AddUniform("pointLights[3].specularStrength", UniformTypes::FLOAT3, glm::vec3(1.0f, 1.0f, 1.0f));
		lightingMaterial.AddUniform("pointLights[3].constant", UniformTypes::FLOAT, 1.0f);
		lightingMaterial.AddUniform("pointLights[3].linear", UniformTypes::FLOAT, 0.09f);
		lightingMaterial.AddUniform("pointLights[3].quadratic", UniformTypes::FLOAT, 0.032f);


		flatColorMaterial.AddUniform("FlatColor", UniformTypes::FLOAT3, pointLightColors[0]);
		Renderer::Draw(lightMesh, pointLightTransforms);

		Renderer::EndScene();

		// check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}