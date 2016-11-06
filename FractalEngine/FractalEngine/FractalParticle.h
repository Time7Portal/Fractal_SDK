#pragma once
#include <functional>
#include <GLM\glm.hpp>
#include "FractalVertex.h"
#include "FractalSpriteBatch.h"
#include "FractalGLTexture.h"


namespace Fractal
{
	class Particle
	{
	public:
		Particle(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~Particle(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		glm::vec2 position = glm::vec2(0.0f, 0.0f); // 파티클 위치 | Particle position
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f); // 파티클 속도 | Particle velocity

		Vertex::ColorRGBA8 color; // 파티클 색 | Particle color
		float lifeTime; // 파티클 생존 시간 | Particle life time
		float width; // 파티클 크기 | Particle size
	};


	/* - 기본 파티클 업데이트 인라인 함수 | Primary particle update inline function - */
	inline void ParticleUpdateDefault(Particle& particle, float deltaTime)
	{
		particle.position += particle.velocity * deltaTime; // 벡터에 따른 현재 위치 수정 | Modify current location according to velocity
	}


	class ParticleBatch
	{
	public:
		ParticleBatch(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~ParticleBatch(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Init(int newMaxParticles, float newDecayRate, GLTexture newTexture, std::function<void(Particle&, float)> NewUpdateFunc = ParticleUpdateDefault); // 파티클 배치를 초기화 하는 함수 (최대 파티클 수, 감소율, 텍스쳐) | Initialize the Particle batch (The maximum number of particles, Reduction rate, Texture)
		void UpdateBatch(float deltaTime); // 파티클 위치를 갱신하는 함수 (델타 시간) | Update the particle location (Delta time)
		void DrawBatch(Fractal::SpriteBatch* spriteBatch); // 파티클 배치를 그리는 함수 (스프라이트 배치) | Draw the particle batch (Sprite batch)
		void AddParticle(const glm::vec2& newPosition, const glm::vec2& newVelocity, const Vertex::ColorRGBA8& newColor, float newWidth); // 파티클을 추가하는 함수 (파티클 위치, 벡터, 색, 크기) | Add the particle (Particle position, velocity, color, size)

	private:
		int FindFreeParticle(); // 비어있는 배열이 있는 경우 찾아서 위치를 반환하는 함수 | If has an empty array then find the location

		std::function<void(Particle&, float)> mp_UpdateFunc; // 파티클 업데이트 함수 포인터 | Particle update function pointer
		Particle* mp_particles = nullptr; // 파티클 포인터 배열 | Particle array pointer
		int maxParticles = 0; // 최대 파티클 카운터 | Maximum particle counter
		int lastFreeParticle = 0; // 제거된 파티클 번호 | Removed particle number
		float decayRate = 0.1f; // 파티클 감쇠 비율 | Particle reduction ratio
		Fractal::GLTexture texture; // 파티클 텍스쳐 | Particle texture
	};


	class ParticleManager
	{
	public:
		ParticleManager(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~ParticleManager(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void AddParticleBatch(Fractal::ParticleBatch* particleBatch); // 파티클 배치를 추가하는 함수 (파티클 배치) | Add the particle batch (Particle batch)
		void Update(float deltaTime); // 파티클 위치를 갱신하는 함수 (델타 시간) | Update the particle location (Delta time)
		void Draw(Fractal::SpriteBatch* spriteBatch); // 파티클을 그리는 함수 (스프라이트 배치) | Draw the particle (Sprite batch)

	private:
		std::vector<ParticleBatch*> mp_Batches; // 파티클 배치 포인터 배열 | Particle batch pointer array
	};
}
