#include "FractalGame.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
MyGame::Game::Game()
{
	mp_Debug = Fractal::Debug::GetInstance(); // 전역 사용을 위해 멤버 포인터 변수에 따로 받아둠
	windowName = "FRACTAL ENGINE TEST RENDERER";
	screenWidth = 900;
	screenHeight = 600;
	gameTime = 0.00f;
	gameState = FractalState::PLAY;
	m_Camera.Init(screenWidth, screenHeight);
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
MyGame::Game::~Game()
{
	// 종료시에 디버깅 콘솔 메모리 비우기
	mp_Debug->FreeInstance();
	mp_Debug = nullptr;
}


/* - 프로그램 실행시 처음 호출되는 함수 | First run - */
void MyGame::Game::Run()
{
	std::thread debugThread(&Fractal::Debug::ConsoleOpen, mp_Debug); // 디버깅용(콘솔) 쓰레드 열기 | Open Debug thread

	InitSystems(); // 시스템 초기화 | System Initialization

	mp_Debug->ConsoleGLInfo(); // 그래픽카드 정보 출력 | Show graphics card info

	GameLoop(); // 게임 루프 돌리기

	debugThread.join(); // 디버깅용(콘솔) 쓰레드 닫기
}


/* - 시스템을 초기화 하는 함수 | Initialize SDL and OpenGL Systems - */
void MyGame::Game::InitSystems()
{
	// 실행중인 프로그램 경로를 가져오기 | Get program path
	programPath = mp_Debug->GetProgramPath();

	// SDL 창 만들기 | Create SDL Window
	m_GameWindow.Create(windowName, screenWidth, screenHeight, Fractal::WINDOWED);

	// 셰이더 초기화 | Shader initialization
	InitShaders();

	// 스프라이트 배치 초기화 | Sprite Batch initialization
	m_SpriteBatch.Init();
	m_HudSpriteBatch.Init();

	// 파티클 배치 초기화
	std::string starParicleFilePath; // 파티클 이미지 파일 위치
	starParicleFilePath.insert(0, programPath); // 프로그램 경로 추가
	starParicleFilePath.insert(starParicleFilePath.length(), "\\Texture\\Creepy_Star.png"); // 해당 텍스쳐 파일 경로 추가
	mp_StarParicleBatch = new Fractal::ParticleBatch;
	mp_StarParicleBatch->Init(1024, 0.1f, Fractal::ResourceManager::GetTexture(starParicleFilePath),
	[](Fractal::Particle& particle, float deltaTime) // 람다 인라인 함수 (별 파티클을 업데이트하는 함수 | Update the star particle)
	{
	particle.position += particle.velocity * deltaTime; // 벡터에 따른 현재 위치 수정
	particle.color.a = (GLubyte)(particle.lifeTime * 255.0f);
	particle.width = particle.lifeTime * 50.0f;
	});
	m_ParticleManager.AddParticleBatch(mp_StarParicleBatch); // 파티클 배치 추가

	// Box2D 초기화 (중력 세계 만들고 땅 정의)
	b2Vec2 B2Gravity(0.0f, -9.8f); // Box2D 중력 가속도(벡터) 값 생성
	m_PhysicsWorld = std::make_unique<b2World>(B2Gravity); // 중력을 포함한 Box2D 세계 지정
	b2BodyDef B2GroundBodyDef; // 추락 방지용 땅 생성
	B2GroundBodyDef.position.Set(0.0f, -10.0f); // 땅 위치 지정
	b2Body* mp_B2GroundBody = m_PhysicsWorld->CreateBody(&B2GroundBodyDef); // 세계에 땅 몸체 생성해서 포인터 반환
	b2PolygonShape B2GroundBox; // 땅 박스 도형 생성
	B2GroundBox.SetAsBox(40.0f, 40.0f); // 땅 박스 도형 크기 지정
	mp_B2GroundBody->CreateFixture(&B2GroundBox, 0.0f); // 땅 몸체를 고정된 박스 형태의 도형으로 대체

	b2BodyDef B2GroundBodyDef2; // 추락 방지용 땅 생성
	B2GroundBodyDef2.position.Set(0.0f, -100.0f); // 땅 위치 지정
	b2Body* mp_B2GroundBody2 = m_PhysicsWorld->CreateBody(&B2GroundBodyDef2); // 세계에 땅 몸체 생성해서 포인터 반환
	b2PolygonShape B2GroundBox2; // 땅 박스 도형 생성
	B2GroundBox2.SetAsBox(500.0f, 10.0f); // 땅 박스 도형 크기 지정
	mp_B2GroundBody2->CreateFixture(&B2GroundBox2, 0.0f); // 땅 몸체를 고정된 박스 형태의 도형으로 대체

	// Box2D 적용된 박스 생성
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-50.0f, 40.0f);
	std::uniform_real_distribution<float> yPos(50.0f, 200.0f);
	std::uniform_int_distribution<int> color(100, 255);
	const int NUM_OF_BOX = 50;
	for (int count = 0; count < NUM_OF_BOX; count++)
	{
		Fractal::PhysicsBox m_NewBox;
		m_NewBox.Init(m_PhysicsWorld.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(40.0f, 40.0f), Fractal::Vertex::ColorRGBA8(255, 255, color(randGenerator), 255));
		mv_Boxes.push_back(m_NewBox); // 벡터 배열에 넣기
	}
}


/* - 셰이더를 초기화 하는 함수 | GLSL Shader initialization - */
void MyGame::Game::InitShaders()
{
	// 셰이더 파일 위치 찾기
	std::string vertexShaderFilePath; // 버텍스 셰이더 파일 위치
	std::string fragmentShaderFilePath; // 프레그먼트 셰이더 파일 위치

	// 프로그램 경로 추가
	vertexShaderFilePath.insert(0, programPath);
	fragmentShaderFilePath.insert(0, programPath);

	// 각 셰이더 파일 경로 추가
	vertexShaderFilePath.insert(vertexShaderFilePath.length(), "\\Shader\\colorShading.vert");
	fragmentShaderFilePath.insert(fragmentShaderFilePath.length(), "\\Shader\\colorShading.frag");

	// 셰이더 파일 위치 출력
	Fractal::Debug::GetInstance()->WriteConsoleMessage("Vertex shader file path : \n" + vertexShaderFilePath);
	Fractal::Debug::GetInstance()->WriteConsoleMessage("Fragment shader file path : \n" + fragmentShaderFilePath);

	m_ColorShaderProgram.CompileShader(vertexShaderFilePath, fragmentShaderFilePath); // 셰이더 컴파일
	m_ColorShaderProgram.AddAttribute("vertexPosition"); // 버텍스 포지션 어트리뷰트 추가
	m_ColorShaderProgram.AddAttribute("vertexColor"); // 버텍스 컬러 어트리뷰트 추가
	m_ColorShaderProgram.AddAttribute("vertexUV"); // 버텍스 UV 어트리뷰트 추가
	m_ColorShaderProgram.LinkShader(); // 셰이더 링킹
}


/* - 게임을 순환하는 함수 | Game Loop - */
void MyGame::Game::GameLoop()
{
	while (gameState != FractalState::EXIT) // gameState상태가 EXIT 상태가 아니면
	{
		gameTime += 0.02f; // 시간 흘러감

		ProcessInput(); // 장치 입력 받기	

		m_Camera.Update(); // 카메라 정보 업데이트

		// 모든 총알 정보 업데이트 (생존기간 만료시 제거)
		for (int i = 0; i < mv_Bullets.size(); i++)
		{
			if (mv_Bullets[i].Update() == true) // 생존기간 만료된 총알 제거
			{
				mv_Bullets[i] = mv_Bullets.back();
				mv_Bullets.pop_back();
			}
		}

		m_ParticleManager.Update(0.2f); // 모든 파티클 시간 갱신

		DrawGame(); // 게임 그리기

		// FPS 표시
		if (mp_Debug->showFps == 1)
		{
			m_GameWindow.CalculateFPS(); // FPS 계산
			static std::string fpsTitle = ""; // FPS가 포함된 윈도우 제목
			static int frameCounter = 0;
			frameCounter++;
			if (frameCounter == 10)
			{
				fpsTitle.append("FRACTAL ENGINE TEST RENDERER | FPS : "); // FPS 표시용 제목 앞부분
				fpsTitle.append(std::to_string(m_GameWindow.GetFPS())); // FPS 표시용 제목 뒷부분 (FPS 값 넣기)
				m_GameWindow.SetWindowName(fpsTitle); // 윈도우 이름 변경 | Set SDL Window name
				frameCounter = 0; // 프레임 카운터 초기화
				fpsTitle = ""; // FPS 표시용 제목 비우기
			}
		}
	}
}


/* - 장치(마우스나 키보드 등) 입력을 받는 함수 | Input keyboard & mouse & etc... - */
void MyGame::Game::ProcessInput()
{
	SDL_Event gameEvent; // 게임 이벤트

	while (SDL_PollEvent(&gameEvent)) // 현재 보류중인 이벤트를 폴링함 (1이면 이벤트 기다리기 0이면 비활성화) | Will keep looping until there are no more events to process
	{
		switch (gameEvent.type) // 이벤트 타입에 따른 실행
		{
		case SDL_QUIT: // 닫기 버튼이 눌렸을때
			gameState = FractalState::EXIT;
			Fractal::Debug::GetInstance()->WriteConsoleMessage("Program is shutting down...");
			Sleep(1000); // 1초간 잠시 대기(시스템 안정화)
			exit(0); // 프로그램 정상 종료
			break;

			// 키보드 입력 받기
		case SDL_KEYDOWN: // 키보드를 눌렀을때
			m_InputManager.PressKey(gameEvent.key.keysym.sym);
			std::cout << "Keyboard pressed : " << gameEvent.key.keysym.sym << std::endl;
			break;

		case SDL_KEYUP: // 키보드를 뗐을때
			m_InputManager.ReleaseKey(gameEvent.key.keysym.sym);
			std::cout << "Keyboard released : " << gameEvent.key.keysym.sym << std::endl;
			break;

			// 마우스 입력 받기
		case SDL_MOUSEMOTION: // 마우스가 움직일때
			m_InputManager.SetMouseCoords(gameEvent.motion.x, gameEvent.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN: // 마우스를 눌렀을때
			m_InputManager.PressKey(gameEvent.button.button);
			std::cout << "Mouse pressed : " << gameEvent.motion.x << "/" << gameEvent.motion.y << std::endl;
			break;

		case SDL_MOUSEBUTTONUP: // 마우스를 뗐을때
			m_InputManager.ReleaseKey(gameEvent.button.button);
			std::cout << "Mouse released : " << gameEvent.motion.x << "/" << gameEvent.motion.y << std::endl;
			break;
		}
	}

	// 키보드 입력에 따른 카메라 이동 및 줌
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
	if (m_InputManager.IsKeyPressed(SDL_BUTTON_LEFT)) // 마우스 왼쪽 클릭된 경우
	{
		glm::vec2 mouseCoords = m_InputManager.GetMouseCoords();
		mouseCoords = m_Camera.ConvertScreenToWorld(mouseCoords); // 마우스 위치의 월드 좌표계 가져오기
		std::cout << "Left / Drag : " << mouseCoords.x << "/" << mouseCoords.y << std::endl;

		// 총알 준비
		glm::vec2 playerPosition(-9.0f, -28.0f);
		playerDirection = mouseCoords - playerPosition;
		playerDirection = glm::normalize(playerDirection);
		mv_Bullets.emplace_back(playerPosition, playerDirection, 4.0f, 100); // 총알 발사 (초기위치, 플레이어 방향, 속력, 생존기간)
	}
	if (m_InputManager.IsKeyPressed(SDL_BUTTON_RIGHT)) // 마우스 오른쪽 클릭된 경우
	{
		glm::vec2 mouseCoords = m_InputManager.GetMouseCoords();
		mouseCoords = m_Camera.ConvertScreenToWorld(mouseCoords); // 마우스 위치의 월드 좌표계 가져오기
		std::cout << "Right / Drag : " << mouseCoords.x << "/" << mouseCoords.y << std::endl;

		// 파티클 준비
		glm::vec2 starPosition(mouseCoords.x - 16.0f, mouseCoords.y - 12.0f);
		MakeStarParticle(starPosition, 2);
	}
}


/* - 게임 화면을 그리기는 함수 | Draws the game using the almighty OpenGL - */
void MyGame::Game::DrawGame()
{
	glClearDepth(1.0); // Set the base depth to 1.0
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // 기본 배경 컬러 셋팅 (그레이)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 색상, 깊이 버퍼 클리어

	glMatrixMode(GL_MODELVIEW); // 직교(투영)뷰 렌더 모드
	glLoadIdentity();

	// 셰이더 칼라 사용 | Enable the shader
	m_ColorShaderProgram.Use();

	// 텍스쳐 활성화 | Using texture unit 0
	glActiveTexture(GL_TEXTURE0);

	// GLSL에 셰이더 타임을 정의 | Set the constantly changine time variable
	GLuint timeLocation = m_ColorShaderProgram.GetUniformLocation("time");
	glUniform1f(timeLocation, gameTime); // 게임 타임을 GLSL로 넘기기

	// GLSL에 텍스쳐 좌표를 정의 | Get the uniform location
	GLint textureLocation = m_ColorShaderProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation, 0); // 해당 텍스쳐 위치가 몇번째 텍스쳐를 가리키는지

	// GLSL에 카메라 위치를 정의 | Set the camera matrix
	GLint pLocation = m_ColorShaderProgram.GetUniformLocation("camMatrix");
	glm::mat4 cameraMatrix = m_Camera.GetCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_SpriteBatch.Start(); // 스프라이트 입력 시작

	static Fractal::GLTexture texture = Fractal::ResourceManager::GetTexture(programPath + "\\Texture\\Creepy_Face.png");
	Fractal::Vertex::ColorRGBA8 color; // 버텍스 컬러 정의
	color = Fractal::Vertex::ColorRGBA8(255, 255, 255, 255); // 버텍스 컬러 할당

	// 캐릭터 이미지 그리기
	glm::vec4 pos(-50.0f, -50.0f, 100.0f, 100.0f); // 사각형 (위치, 크기)
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f); // 사각형 (UV 좌표)
	m_SpriteBatch.Draw(pos, uv, texture.id, 0.0f, color, playerDirection);

	// 총알 그리기
	for (int i = 0; i < mv_Bullets.size(); i++)
	{
		mv_Bullets[i].Draw(m_SpriteBatch);
	}

	// Box2D 물리 박스 전부 그리기
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

	m_SpriteBatch.End(); // 스프라이트 입력 끝

	m_SpriteBatch.RenderBatch(); // 스프라이트 모음 그리기 | Draw sprite batches

	m_ParticleManager.Draw(&m_SpriteBatch); // 스프라이트 배치에 파티클 그리기

	DrawHud(); // HUD 그리기

	glBindTexture(GL_TEXTURE_2D, 0); // 바인드된 텍스쳐 비우기 | Unbind the texture

	m_ColorShaderProgram.Unuse(); // 셰이더 칼라 사용 안함 | Disable the shader

	/*
	// 테스트용 삼각형 그리기 시작
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
	// 테스트용 삼각형 그리기 끝
	*/

	m_GameWindow.SwapBuffer(); // 버퍼 스왑
}


/* - HUD 화면을 그리는 함수 | Draw HUD like Fonts & UI & etc... - */
void MyGame::Game::DrawHud()
{

}


/* - 별 파티클을 만드는 함수 | Make a Star Particle - */
void MyGame::Game::MakeStarParticle(const glm::vec2& position, int numParicles)
{
	// 랜덤 값 생성
	static std::mt19937 randEngine((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 velocity(8.0f, 0.0f);
	Fractal::Vertex::ColorRGBA8 color(255, 255, 255, 255);

	for (int i = 0; i < numParicles; i++)
	{
		mp_StarParicleBatch->AddParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 20.0f);
	}
}
