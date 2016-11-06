#pragma once
#include <Windows.h>
#include <thread> // http://msdn.microsoft.com/ko-kr/library/vstudio/hh920526(v=vs.120).aspx/
#include <vector>
#include <ctime>
#include <random>
#include <SDL\SDL.h> // http://www.libsdl.org/
#include <GL\glew.h> // http://glew.sourceforge.net/
#include <GLM\gtx\rotate_vector.hpp>
#include <BOX2D\Box2D.h> // http://box2d.org/
#include <FractalEngine\FractalWindow.h>
#include <FractalEngine\FractalDebug.h>
#include <FractalEngine\FractalInputManager.h>
#include <FractalEngine\FractalCamera2D.h>
#include <FractalEngine\FractalGLSL.h>
#include <FractalEngine\FractalResourceManager.h>
#include <FractalEngine\FractalSprite.h>
#include <FractalEngine\FractalSpriteBatch.h>
#include <FractalEngine\FractalSpriteFont.h>
#include <FractalEngine\FractalGLTexture.h>
#include <FractalEngine\FractalBullet.h>
#include <FractalEngine\FractalAudio.h>
#include <FractalEngine\FractalParticle.h>
#include <FractalEngine\FractalPhysicsBox.h>


enum class FractalState { PLAY, EXIT };

namespace MyGame
{
	class Game
	{
	public:
		Game(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~Game(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Run(); // 프로그램 실행시 처음 호출되는 함수 | First run

	private:
		void InitSystems(); // 시스템을 초기화 하는 함수 | Initialize SDL and OpenGL Systems
		void InitShaders(); // 셰이더를 초기화 하는 함수 | GLSL Shader initialization
		void GameLoop(); // 게임을 순환하는 함수 | Game Loop
		void ProcessInput(); // 장치(마우스나 키보드 등) 입력을 받는 함수 | Input keyboard & mouse & etc...
		void DrawGame(); // 게임 화면을 그리기는 함수 | Draws the game using the almighty OpenGL
		void DrawHud(); // HUD 화면을 그리는 함수 | Draw HUD like Fonts & UI & etc...
		void MakeStarParticle(const glm::vec2& position, int numParicles); // 별 파티클을 만드는 함수 | Make a Star Particle

		Fractal::Debug* mp_Debug; // 싱글톤 디버그 콘솔 스레드 생성을 위한 포인터 | Pointer for Singleton Debug console Thread
		std::string programPath; // 실행중인 프로그램 경로를 저장 | Get program path

		Fractal::Window m_GameWindow; // 프렉탈 엔진 렌더 윈도우 정의 | Fractal Engine Render Window Definition
		std::string windowName; // 윈도우 제목 | Window Title
		int screenWidth; // 모니터 넓이 해상도 | Width Screen Resolution
		int screenHeight; // 모니터 높이 해상도 | Height Screen Resolution

		FractalState gameState; // 이넘 클래스 형태의 게임 스테이트 정의 | The Game State (PLAY, EXIT)

		Fractal::GLSLProgram m_ColorShaderProgram; // 셰이더 정의 | Shader Definition

		Fractal::Camera2D m_Camera; // 카메라 정의 | Camera Definition
		float cameraMoveSpeed = 2.0f; // 카메라 이동 속도 | Camera Move Speed
		float cameraZoomSpeed = 0.02f; // 카메라 줌 속도 | Camera Zoom Speed

		glm::vec2 playerDirection = glm::vec2(1.0f, 0.0f); // 캐릭터 방향

		Fractal::SpriteBatch m_SpriteBatch; // 스프라이트 배치 정의 | Sprite Batch Definition
		Fractal::SpriteBatch m_HudSpriteBatch; // HUD 표시용 스프라이트 배치 정의

		Fractal::InputManager m_InputManager; // 입력 관리자 정의 | Input Manager Definition

		Fractal::AudioManager m_AudioEngine; // 오디오 정의

		Fractal::ParticleManager m_ParticleManager; // 파티클 관리자 정의
		Fractal::ParticleBatch* mp_StarParicleBatch; // 파티클 배치 정의

		Fractal::SpriteFont m_TTFont; // 트루타입 폰트 정의

		std::vector<Fractal::Bullet> mv_Bullets; // 총알 정의 | Bullets Definition

		float gameTime; // 게임 시각 | The Game Time

		std::unique_ptr<b2World> m_PhysicsWorld; // Box2D 를 이용한 물리 세계 정의
		std::vector<Fractal::PhysicsBox> mv_Boxes; // Box2D 가 적용된 박스 예제
	};
}
