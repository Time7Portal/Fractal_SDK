#include "FractalGame.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
MyGame::Game::Game()
{
	mp_Debug = Fractal::Debug::GetInstance(); // ���� ����� ���� ��� ������ ������ ���� �޾Ƶ�
	windowName = "FRACTAL ENGINE TEST RENDERER";
	screenWidth = 900;
	screenHeight = 600;
	gameTime = 0.00f;
	gameState = FractalState::PLAY;
	m_Camera.Init(screenWidth, screenHeight);
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
MyGame::Game::~Game()
{
	// ����ÿ� ����� �ܼ� �޸� ����
	mp_Debug->FreeInstance();
	mp_Debug = nullptr;
}


/* - ���α׷� ����� ó�� ȣ��Ǵ� �Լ� | First run - */
void MyGame::Game::Run()
{
	std::thread debugThread(&Fractal::Debug::ConsoleOpen, mp_Debug); // ������(�ܼ�) ������ ���� | Open Debug thread

	InitSystems(); // �ý��� �ʱ�ȭ | System Initialization

	mp_Debug->ConsoleGLInfo(); // �׷���ī�� ���� ��� | Show graphics card info

	GameLoop(); // ���� ���� ������

	debugThread.join(); // ������(�ܼ�) ������ �ݱ�
}


/* - �ý����� �ʱ�ȭ �ϴ� �Լ� | Initialize SDL and OpenGL Systems - */
void MyGame::Game::InitSystems()
{
	// �������� ���α׷� ��θ� �������� | Get program path
	programPath = mp_Debug->GetProgramPath();

	// SDL â ����� | Create SDL Window
	m_GameWindow.Create(windowName, screenWidth, screenHeight, Fractal::WINDOWED);

	// ���̴� �ʱ�ȭ | Shader initialization
	InitShaders();

	// ��������Ʈ ��ġ �ʱ�ȭ | Sprite Batch initialization
	m_SpriteBatch.Init();
	m_HudSpriteBatch.Init();

	// ��ƼŬ ��ġ �ʱ�ȭ
	std::string starParicleFilePath; // ��ƼŬ �̹��� ���� ��ġ
	starParicleFilePath.insert(0, programPath); // ���α׷� ��� �߰�
	starParicleFilePath.insert(starParicleFilePath.length(), "\\Texture\\Creepy_Star.png"); // �ش� �ؽ��� ���� ��� �߰�
	mp_StarParicleBatch = new Fractal::ParticleBatch;
	mp_StarParicleBatch->Init(1024, 0.1f, Fractal::ResourceManager::GetTexture(starParicleFilePath),
	[](Fractal::Particle& particle, float deltaTime) // ���� �ζ��� �Լ� (�� ��ƼŬ�� ������Ʈ�ϴ� �Լ� | Update the star particle)
	{
	particle.position += particle.velocity * deltaTime; // ���Ϳ� ���� ���� ��ġ ����
	particle.color.a = (GLubyte)(particle.lifeTime * 255.0f);
	particle.width = particle.lifeTime * 50.0f;
	});
	m_ParticleManager.AddParticleBatch(mp_StarParicleBatch); // ��ƼŬ ��ġ �߰�

	// Box2D �ʱ�ȭ (�߷� ���� ����� �� ����)
	b2Vec2 B2Gravity(0.0f, -9.8f); // Box2D �߷� ���ӵ�(����) �� ����
	m_PhysicsWorld = std::make_unique<b2World>(B2Gravity); // �߷��� ������ Box2D ���� ����
	b2BodyDef B2GroundBodyDef; // �߶� ������ �� ����
	B2GroundBodyDef.position.Set(0.0f, -10.0f); // �� ��ġ ����
	b2Body* mp_B2GroundBody = m_PhysicsWorld->CreateBody(&B2GroundBodyDef); // ���迡 �� ��ü �����ؼ� ������ ��ȯ
	b2PolygonShape B2GroundBox; // �� �ڽ� ���� ����
	B2GroundBox.SetAsBox(40.0f, 40.0f); // �� �ڽ� ���� ũ�� ����
	mp_B2GroundBody->CreateFixture(&B2GroundBox, 0.0f); // �� ��ü�� ������ �ڽ� ������ �������� ��ü

	b2BodyDef B2GroundBodyDef2; // �߶� ������ �� ����
	B2GroundBodyDef2.position.Set(0.0f, -100.0f); // �� ��ġ ����
	b2Body* mp_B2GroundBody2 = m_PhysicsWorld->CreateBody(&B2GroundBodyDef2); // ���迡 �� ��ü �����ؼ� ������ ��ȯ
	b2PolygonShape B2GroundBox2; // �� �ڽ� ���� ����
	B2GroundBox2.SetAsBox(500.0f, 10.0f); // �� �ڽ� ���� ũ�� ����
	mp_B2GroundBody2->CreateFixture(&B2GroundBox2, 0.0f); // �� ��ü�� ������ �ڽ� ������ �������� ��ü

	// Box2D ����� �ڽ� ����
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-50.0f, 40.0f);
	std::uniform_real_distribution<float> yPos(50.0f, 200.0f);
	std::uniform_int_distribution<int> color(100, 255);
	const int NUM_OF_BOX = 50;
	for (int count = 0; count < NUM_OF_BOX; count++)
	{
		Fractal::PhysicsBox m_NewBox;
		m_NewBox.Init(m_PhysicsWorld.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(40.0f, 40.0f), Fractal::Vertex::ColorRGBA8(255, 255, color(randGenerator), 255));
		mv_Boxes.push_back(m_NewBox); // ���� �迭�� �ֱ�
	}
}


/* - ���̴��� �ʱ�ȭ �ϴ� �Լ� | GLSL Shader initialization - */
void MyGame::Game::InitShaders()
{
	// ���̴� ���� ��ġ ã��
	std::string vertexShaderFilePath; // ���ؽ� ���̴� ���� ��ġ
	std::string fragmentShaderFilePath; // �����׸�Ʈ ���̴� ���� ��ġ

	// ���α׷� ��� �߰�
	vertexShaderFilePath.insert(0, programPath);
	fragmentShaderFilePath.insert(0, programPath);

	// �� ���̴� ���� ��� �߰�
	vertexShaderFilePath.insert(vertexShaderFilePath.length(), "\\Shader\\colorShading.vert");
	fragmentShaderFilePath.insert(fragmentShaderFilePath.length(), "\\Shader\\colorShading.frag");

	// ���̴� ���� ��ġ ���
	Fractal::Debug::GetInstance()->WriteConsoleMessage("Vertex shader file path : \n" + vertexShaderFilePath);
	Fractal::Debug::GetInstance()->WriteConsoleMessage("Fragment shader file path : \n" + fragmentShaderFilePath);

	m_ColorShaderProgram.CompileShader(vertexShaderFilePath, fragmentShaderFilePath); // ���̴� ������
	m_ColorShaderProgram.AddAttribute("vertexPosition"); // ���ؽ� ������ ��Ʈ����Ʈ �߰�
	m_ColorShaderProgram.AddAttribute("vertexColor"); // ���ؽ� �÷� ��Ʈ����Ʈ �߰�
	m_ColorShaderProgram.AddAttribute("vertexUV"); // ���ؽ� UV ��Ʈ����Ʈ �߰�
	m_ColorShaderProgram.LinkShader(); // ���̴� ��ŷ
}


/* - ������ ��ȯ�ϴ� �Լ� | Game Loop - */
void MyGame::Game::GameLoop()
{
	while (gameState != FractalState::EXIT) // gameState���°� EXIT ���°� �ƴϸ�
	{
		gameTime += 0.02f; // �ð� �귯��

		ProcessInput(); // ��ġ �Է� �ޱ�	

		m_Camera.Update(); // ī�޶� ���� ������Ʈ

		// ��� �Ѿ� ���� ������Ʈ (�����Ⱓ ����� ����)
		for (int i = 0; i < mv_Bullets.size(); i++)
		{
			if (mv_Bullets[i].Update() == true) // �����Ⱓ ����� �Ѿ� ����
			{
				mv_Bullets[i] = mv_Bullets.back();
				mv_Bullets.pop_back();
			}
		}

		m_ParticleManager.Update(0.2f); // ��� ��ƼŬ �ð� ����

		DrawGame(); // ���� �׸���

		// FPS ǥ��
		if (mp_Debug->showFps == 1)
		{
			m_GameWindow.CalculateFPS(); // FPS ���
			static std::string fpsTitle = ""; // FPS�� ���Ե� ������ ����
			static int frameCounter = 0;
			frameCounter++;
			if (frameCounter == 10)
			{
				fpsTitle.append("FRACTAL ENGINE TEST RENDERER | FPS : "); // FPS ǥ�ÿ� ���� �պκ�
				fpsTitle.append(std::to_string(m_GameWindow.GetFPS())); // FPS ǥ�ÿ� ���� �޺κ� (FPS �� �ֱ�)
				m_GameWindow.SetWindowName(fpsTitle); // ������ �̸� ���� | Set SDL Window name
				frameCounter = 0; // ������ ī���� �ʱ�ȭ
				fpsTitle = ""; // FPS ǥ�ÿ� ���� ����
			}
		}
	}
}


/* - ��ġ(���콺�� Ű���� ��) �Է��� �޴� �Լ� | Input keyboard & mouse & etc... - */
void MyGame::Game::ProcessInput()
{
	SDL_Event gameEvent; // ���� �̺�Ʈ

	while (SDL_PollEvent(&gameEvent)) // ���� �������� �̺�Ʈ�� ������ (1�̸� �̺�Ʈ ��ٸ��� 0�̸� ��Ȱ��ȭ) | Will keep looping until there are no more events to process
	{
		switch (gameEvent.type) // �̺�Ʈ Ÿ�Կ� ���� ����
		{
		case SDL_QUIT: // �ݱ� ��ư�� ��������
			gameState = FractalState::EXIT;
			Fractal::Debug::GetInstance()->WriteConsoleMessage("Program is shutting down...");
			Sleep(1000); // 1�ʰ� ��� ���(�ý��� ����ȭ)
			exit(0); // ���α׷� ���� ����
			break;

			// Ű���� �Է� �ޱ�
		case SDL_KEYDOWN: // Ű���带 ��������
			m_InputManager.PressKey(gameEvent.key.keysym.sym);
			std::cout << "Keyboard pressed : " << gameEvent.key.keysym.sym << std::endl;
			break;

		case SDL_KEYUP: // Ű���带 ������
			m_InputManager.ReleaseKey(gameEvent.key.keysym.sym);
			std::cout << "Keyboard released : " << gameEvent.key.keysym.sym << std::endl;
			break;

			// ���콺 �Է� �ޱ�
		case SDL_MOUSEMOTION: // ���콺�� �����϶�
			m_InputManager.SetMouseCoords(gameEvent.motion.x, gameEvent.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN: // ���콺�� ��������
			m_InputManager.PressKey(gameEvent.button.button);
			std::cout << "Mouse pressed : " << gameEvent.motion.x << "/" << gameEvent.motion.y << std::endl;
			break;

		case SDL_MOUSEBUTTONUP: // ���콺�� ������
			m_InputManager.ReleaseKey(gameEvent.button.button);
			std::cout << "Mouse released : " << gameEvent.motion.x << "/" << gameEvent.motion.y << std::endl;
			break;
		}
	}

	// Ű���� �Է¿� ���� ī�޶� �̵� �� ��
	if (m_InputManager.IsKeyPressed(SDLK_w))
	{
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec2(0.0f, -cameraMoveSpeed));
	}
	if (m_InputManager.IsKeyPressed(SDLK_s))
	{
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec2(0.0f, cameraMoveSpeed));
	}
	if (m_InputManager.IsKeyPressed(SDLK_a))
	{
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec2(cameraMoveSpeed, 0.0f));
	}
	if (m_InputManager.IsKeyPressed(SDLK_d))
	{
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec2(-cameraMoveSpeed, 0.0f));
	}
	if (m_InputManager.IsKeyPressed(SDLK_q))
	{
		m_Camera.SetScale(m_Camera.GetScale() + cameraZoomSpeed);
	}
	if (m_InputManager.IsKeyPressed(SDLK_e))
	{
		m_Camera.SetScale(m_Camera.GetScale() - cameraZoomSpeed);
	}
	if (m_InputManager.IsKeyPressed(SDL_BUTTON_LEFT)) // ���콺 ���� Ŭ���� ���
	{
		glm::vec2 mouseCoords = m_InputManager.GetMouseCoords();
		mouseCoords = m_Camera.ConvertScreenToWorld(mouseCoords); // ���콺 ��ġ�� ���� ��ǥ�� ��������
		std::cout << "Left / Drag : " << mouseCoords.x << "/" << mouseCoords.y << std::endl;

		// �Ѿ� �غ�
		glm::vec2 playerPosition(-9.0f, -28.0f);
		playerDirection = mouseCoords - playerPosition;
		playerDirection = glm::normalize(playerDirection);
		mv_Bullets.emplace_back(playerPosition, playerDirection, 4.0f, 100); // �Ѿ� �߻� (�ʱ���ġ, �÷��̾� ����, �ӷ�, �����Ⱓ)
	}
	if (m_InputManager.IsKeyPressed(SDL_BUTTON_RIGHT)) // ���콺 ������ Ŭ���� ���
	{
		glm::vec2 mouseCoords = m_InputManager.GetMouseCoords();
		mouseCoords = m_Camera.ConvertScreenToWorld(mouseCoords); // ���콺 ��ġ�� ���� ��ǥ�� ��������
		std::cout << "Right / Drag : " << mouseCoords.x << "/" << mouseCoords.y << std::endl;

		// ��ƼŬ �غ�
		glm::vec2 starPosition(mouseCoords.x - 16.0f, mouseCoords.y - 12.0f);
		MakeStarParticle(starPosition, 2);
	}
}


/* - ���� ȭ���� �׸���� �Լ� | Draws the game using the almighty OpenGL - */
void MyGame::Game::DrawGame()
{
	glClearDepth(1.0); // Set the base depth to 1.0
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // �⺻ ��� �÷� ���� (�׷���)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ����, ���� ���� Ŭ����

	glMatrixMode(GL_MODELVIEW); // ����(����)�� ���� ���
	glLoadIdentity();

	// ���̴� Į�� ��� | Enable the shader
	m_ColorShaderProgram.Use();

	// �ؽ��� Ȱ��ȭ | Using texture unit 0
	glActiveTexture(GL_TEXTURE0);

	// GLSL�� ���̴� Ÿ���� ���� | Set the constantly changine time variable
	GLuint timeLocation = m_ColorShaderProgram.GetUniformLocation("time");
	glUniform1f(timeLocation, gameTime); // ���� Ÿ���� GLSL�� �ѱ��

	// GLSL�� �ؽ��� ��ǥ�� ���� | Get the uniform location
	GLint textureLocation = m_ColorShaderProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation, 0); // �ش� �ؽ��� ��ġ�� ���° �ؽ��ĸ� ����Ű����

	// GLSL�� ī�޶� ��ġ�� ���� | Set the camera matrix
	GLint pLocation = m_ColorShaderProgram.GetUniformLocation("camMatrix");
	glm::mat4 cameraMatrix = m_Camera.GetCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_SpriteBatch.Start(); // ��������Ʈ �Է� ����

	static Fractal::GLTexture texture = Fractal::ResourceManager::GetTexture(programPath + "\\Texture\\Creepy_Face.png");
	Fractal::Vertex::ColorRGBA8 color; // ���ؽ� �÷� ����
	color = Fractal::Vertex::ColorRGBA8(255, 255, 255, 255); // ���ؽ� �÷� �Ҵ�

	// ĳ���� �̹��� �׸���
	glm::vec4 pos(-50.0f, -50.0f, 100.0f, 100.0f); // �簢�� (��ġ, ũ��)
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f); // �簢�� (UV ��ǥ)
	m_SpriteBatch.Draw(pos, uv, texture.id, 0.0f, color, playerDirection);

	// �Ѿ� �׸���
	for (int i = 0; i < mv_Bullets.size(); i++)
	{
		mv_Bullets[i].Draw(m_SpriteBatch);
	}

	// Box2D ���� �ڽ� ���� �׸���
	static Fractal::GLTexture boxTexture = Fractal::ResourceManager::GetTexture(programPath + "\\Texture\\Creepy_Toefl.png");
	for (auto& b : mv_Boxes)
	{
		glm::vec4 posRect;
		posRect.x = b.GetBody()->GetPosition().x - b.getDimensions().x / 2.0f;
		posRect.y = b.GetBody()->GetPosition().y - b.getDimensions().y / 2.0f;
		posRect.z = b.getDimensions().x;
		posRect.w = b.getDimensions().y;
		m_SpriteBatch.Draw(posRect, uv, boxTexture.id, 0.0f, b.getColor(), b.GetBody()->GetAngle());
	}
	m_PhysicsWorld->Step(5.0f / 60.0f, 6, 8);

	m_SpriteBatch.End(); // ��������Ʈ �Է� ��

	m_SpriteBatch.RenderBatch(); // ��������Ʈ ���� �׸��� | Draw sprite batches

	m_ParticleManager.Draw(&m_SpriteBatch); // ��������Ʈ ��ġ�� ��ƼŬ �׸���

	DrawHud(); // HUD �׸���

	glBindTexture(GL_TEXTURE_2D, 0); // ���ε�� �ؽ��� ���� | Unbind the texture

	m_ColorShaderProgram.Unuse(); // ���̴� Į�� ��� ���� | Disable the shader

	/*
	// �׽�Ʈ�� �ﰢ�� �׸��� ����
	glBegin(GL_TRIANGLES);
	glColor3f(0.5f, 0.9f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glVertex3f(1.0f, 1.0f, 0.5f);
	glVertex3f(1.0f, 0.0f, 0.5f);

	glColor3f(0.9f, 0.5f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-1.0f, -1.0f, 0.5f);
	glVertex3f(-1.0f, 0.0f, 0.5f);
	glEnd();
	// �׽�Ʈ�� �ﰢ�� �׸��� ��
	*/

	m_GameWindow.SwapBuffer(); // ���� ����
}


/* - HUD ȭ���� �׸��� �Լ� | Draw HUD like Fonts & UI & etc... - */
void MyGame::Game::DrawHud()
{

}


/* - �� ��ƼŬ�� ����� �Լ� | Make a Star Particle - */
void MyGame::Game::MakeStarParticle(const glm::vec2& position, int numParicles)
{
	// ���� �� ����
	static std::mt19937 randEngine((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 velocity(8.0f, 0.0f);
	Fractal::Vertex::ColorRGBA8 color(255, 255, 255, 255);

	for (int i = 0; i < numParicles; i++)
	{
		mp_StarParicleBatch->AddParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 20.0f);
	}
}
