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
		Particle(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~Particle(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		glm::vec2 position = glm::vec2(0.0f, 0.0f); // ��ƼŬ ��ġ | Particle position
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f); // ��ƼŬ �ӵ� | Particle velocity

		Vertex::ColorRGBA8 color; // ��ƼŬ �� | Particle color
		float lifeTime; // ��ƼŬ ���� �ð� | Particle life time
		float width; // ��ƼŬ ũ�� | Particle size
	};


	/* - �⺻ ��ƼŬ ������Ʈ �ζ��� �Լ� | Primary particle update inline function - */
	inline void ParticleUpdateDefault(Particle& particle, float deltaTime)
	{
		particle.position += particle.velocity * deltaTime; // ���Ϳ� ���� ���� ��ġ ���� | Modify current location according to velocity
	}


	class ParticleBatch
	{
	public:
		ParticleBatch(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~ParticleBatch(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Init(int newMaxParticles, float newDecayRate, GLTexture newTexture, std::function<void(Particle&, float)> NewUpdateFunc = ParticleUpdateDefault); // ��ƼŬ ��ġ�� �ʱ�ȭ �ϴ� �Լ� (�ִ� ��ƼŬ ��, ������, �ؽ���) | Initialize the Particle batch (The maximum number of particles, Reduction rate, Texture)
		void UpdateBatch(float deltaTime); // ��ƼŬ ��ġ�� �����ϴ� �Լ� (��Ÿ �ð�) | Update the particle location (Delta time)
		void DrawBatch(Fractal::SpriteBatch* spriteBatch); // ��ƼŬ ��ġ�� �׸��� �Լ� (��������Ʈ ��ġ) | Draw the particle batch (Sprite batch)
		void AddParticle(const glm::vec2& newPosition, const glm::vec2& newVelocity, const Vertex::ColorRGBA8& newColor, float newWidth); // ��ƼŬ�� �߰��ϴ� �Լ� (��ƼŬ ��ġ, ����, ��, ũ��) | Add the particle (Particle position, velocity, color, size)

	private:
		int FindFreeParticle(); // ����ִ� �迭�� �ִ� ��� ã�Ƽ� ��ġ�� ��ȯ�ϴ� �Լ� | If has an empty array then find the location

		std::function<void(Particle&, float)> mp_UpdateFunc; // ��ƼŬ ������Ʈ �Լ� ������ | Particle update function pointer
		Particle* mp_particles = nullptr; // ��ƼŬ ������ �迭 | Particle array pointer
		int maxParticles = 0; // �ִ� ��ƼŬ ī���� | Maximum particle counter
		int lastFreeParticle = 0; // ���ŵ� ��ƼŬ ��ȣ | Removed particle number
		float decayRate = 0.1f; // ��ƼŬ ���� ���� | Particle reduction ratio
		Fractal::GLTexture texture; // ��ƼŬ �ؽ��� | Particle texture
	};


	class ParticleManager
	{
	public:
		ParticleManager(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~ParticleManager(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void AddParticleBatch(Fractal::ParticleBatch* particleBatch); // ��ƼŬ ��ġ�� �߰��ϴ� �Լ� (��ƼŬ ��ġ) | Add the particle batch (Particle batch)
		void Update(float deltaTime); // ��ƼŬ ��ġ�� �����ϴ� �Լ� (��Ÿ �ð�) | Update the particle location (Delta time)
		void Draw(Fractal::SpriteBatch* spriteBatch); // ��ƼŬ�� �׸��� �Լ� (��������Ʈ ��ġ) | Draw the particle (Sprite batch)

	private:
		std::vector<ParticleBatch*> mp_Batches; // ��ƼŬ ��ġ ������ �迭 | Particle batch pointer array
	};
}
