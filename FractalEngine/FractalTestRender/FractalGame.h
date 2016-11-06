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
		Game(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~Game(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Run(); // ���α׷� ����� ó�� ȣ��Ǵ� �Լ� | First run

	private:
		void InitSystems(); // �ý����� �ʱ�ȭ �ϴ� �Լ� | Initialize SDL and OpenGL Systems
		void InitShaders(); // ���̴��� �ʱ�ȭ �ϴ� �Լ� | GLSL Shader initialization
		void GameLoop(); // ������ ��ȯ�ϴ� �Լ� | Game Loop
		void ProcessInput(); // ��ġ(���콺�� Ű���� ��) �Է��� �޴� �Լ� | Input keyboard & mouse & etc...
		void DrawGame(); // ���� ȭ���� �׸���� �Լ� | Draws the game using the almighty OpenGL
		void DrawHud(); // HUD ȭ���� �׸��� �Լ� | Draw HUD like Fonts & UI & etc...
		void MakeStarParticle(const glm::vec2& position, int numParicles); // �� ��ƼŬ�� ����� �Լ� | Make a Star Particle

		Fractal::Debug* mp_Debug; // �̱��� ����� �ܼ� ������ ������ ���� ������ | Pointer for Singleton Debug console Thread
		std::string programPath; // �������� ���α׷� ��θ� ���� | Get program path

		Fractal::Window m_GameWindow; // ����Ż ���� ���� ������ ���� | Fractal Engine Render Window Definition
		std::string windowName; // ������ ���� | Window Title
		int screenWidth; // ����� ���� �ػ� | Width Screen Resolution
		int screenHeight; // ����� ���� �ػ� | Height Screen Resolution

		FractalState gameState; // �̳� Ŭ���� ������ ���� ������Ʈ ���� | The Game State (PLAY, EXIT)

		Fractal::GLSLProgram m_ColorShaderProgram; // ���̴� ���� | Shader Definition

		Fractal::Camera2D m_Camera; // ī�޶� ���� | Camera Definition
		float cameraMoveSpeed = 2.0f; // ī�޶� �̵� �ӵ� | Camera Move Speed
		float cameraZoomSpeed = 0.02f; // ī�޶� �� �ӵ� | Camera Zoom Speed

		glm::vec2 playerDirection = glm::vec2(1.0f, 0.0f); // ĳ���� ����

		Fractal::SpriteBatch m_SpriteBatch; // ��������Ʈ ��ġ ���� | Sprite Batch Definition
		Fractal::SpriteBatch m_HudSpriteBatch; // HUD ǥ�ÿ� ��������Ʈ ��ġ ����

		Fractal::InputManager m_InputManager; // �Է� ������ ���� | Input Manager Definition

		Fractal::AudioManager m_AudioEngine; // ����� ����

		Fractal::ParticleManager m_ParticleManager; // ��ƼŬ ������ ����
		Fractal::ParticleBatch* mp_StarParicleBatch; // ��ƼŬ ��ġ ����

		Fractal::SpriteFont m_TTFont; // Ʈ��Ÿ�� ��Ʈ ����

		std::vector<Fractal::Bullet> mv_Bullets; // �Ѿ� ���� | Bullets Definition

		float gameTime; // ���� �ð� | The Game Time

		std::unique_ptr<b2World> m_PhysicsWorld; // Box2D �� �̿��� ���� ���� ����
		std::vector<Fractal::PhysicsBox> mv_Boxes; // Box2D �� ����� �ڽ� ����
	};
}
