#include "FractalParticle.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::Particle::Particle()
{
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::Particle::~Particle()
{
}


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::ParticleBatch::ParticleBatch()
{
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::ParticleBatch::~ParticleBatch()
{
	delete[] mp_particles; // mp_particles �迭 ����
}


/* - ��ƼŬ ��ġ�� �ʱ�ȭ �ϴ� �Լ� (�ִ� ��ƼŬ ��, ������, �ؽ���) | Initialize the Particle batch (The maximum number of particles, Reduction rate, Texture) - */
void Fractal::ParticleBatch::Init(int newMaxParticles, float newDecayRate, GLTexture newTexture, std::function<void(Particle&, float)> NewUpdateFunc) // NewUpdateFunc = ParticleUpdateDefault
{
	maxParticles = newMaxParticles; // �ִ� ��ƼŬ �� ����
	mp_particles = new Particle[maxParticles]; // �ִ� ��ƼŬ �� ��ŭ �迭 �Ҵ�
	decayRate = newDecayRate; // ���� ���� ����
	texture = newTexture; // ��ƼŬ �ؽ��� ����
	mp_UpdateFunc = NewUpdateFunc; // ������Ʈ �Լ� �����Ϳ� �� �Ҵ�
}


/* - ��ƼŬ ��ġ�� �����ϴ� �Լ� (��Ÿ �ð�) | Update the particle location (Delta time) - */
void Fractal::ParticleBatch::UpdateBatch(float deltaTime)
{
	for (int i = 0; i < maxParticles; i++) // ��� ��ƼŬ Ž��
	{
		if (mp_particles[i].lifeTime > 0.0f) // ��ƼŬ�� ��ȿ�� ���
		{
			mp_UpdateFunc(mp_particles[i], deltaTime); // �Լ� �����͸� ����Ͽ� ��ƼŬ ��ġ ������Ʈ
			mp_particles[i].lifeTime -= decayRate * deltaTime; // ���� ������ ���� ���� �ð� ����
		}
	}
}


/* - ��ƼŬ ��ġ�� �׸��� �Լ� (��������Ʈ ��ġ) | Draw the particle batch (Sprite batch) - */
void Fractal::ParticleBatch::DrawBatch(Fractal::SpriteBatch* spriteBatch)
{
	static glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f); // �ؽ��� UV �����

	for (int i = 0; i < maxParticles; i++) // ��� ��ƼŬ Ž��
	{
		if (mp_particles[i].lifeTime > 0.0f) // ��ƼŬ�� ��ȿ�� ���
		{
			glm::vec4 destRect(mp_particles[i].position.x, mp_particles[i].position.y, mp_particles[i].width, mp_particles[i].width); // ��ƼŬ�� �׸��� ���� �簢�� �����
			spriteBatch->Draw(destRect, uvRect, texture.id, 0.0f, mp_particles[i].color); // ��������Ʈ ��ġ �����
		}
	}
}


/* - ��ƼŬ�� �߰��ϴ� �Լ� (�⺻ ��ƼŬ ��ġ, �⺻ ����, �⺻ ��, �⺻ ũ��) | Add the particle (Default particle position, Default velocity, Default color, Default size) - */
void Fractal::ParticleBatch::AddParticle(const glm::vec2& newPosition, const glm::vec2& newVelocity, const Vertex::ColorRGBA8& newColor, float newWidth)
{
	int particleIndex = FindFreeParticle(); // ����ִ� ��ƼŬ �迭 ��ȣ ã��

	// ��ƼŬ ����
	mp_particles[particleIndex].lifeTime = 1.0f;
	mp_particles[particleIndex].position = newPosition;
	mp_particles[particleIndex].velocity = newVelocity;
	mp_particles[particleIndex].color = newColor;
	mp_particles[particleIndex].width = newWidth;
}


/* - ����ִ� �迭�� �ִ� ��� ã�Ƽ� ��ġ�� ��ȯ�ϴ� �Լ� | If has an empty array then find the location - */
int Fractal::ParticleBatch::FindFreeParticle()
{
	// ������ ���ŵ� ��ƼŬ ���� ��ƼŬ ������ Ž��
	for (int i = lastFreeParticle; i < maxParticles; i++)
	{
		if (mp_particles[i].lifeTime <= 0.0f) // �ش� ��ƼŬ ���� �ð��� 0.0f �� ���
		{
			lastFreeParticle = i; // ���������� ���ŵ� ��ƼŬ ��ȣ ����
			return lastFreeParticle; // ����ִ� �迭 ��ȣ ��ȯ
		}
	}

	// 0 �� ��ƼŬ ���� ������ ���ŵ� ��ƼŬ ���� Ž��
	for (int i = 0; i < lastFreeParticle; i++)
	{
		if (mp_particles[i].lifeTime <= 0.0f) // �ش� ��ƼŬ ���� �ð��� 0.0f �� ���
		{
			lastFreeParticle = i; // ���������� ���ŵ� ��ƼŬ ��ȣ ����
			return lastFreeParticle; // ����ִ� �迭 ��ȣ ��ȯ
		}
	}

	return 0; // ����ִ� ��ƼŬ �迭�� �ϳ��� ���� ��� 0 ��ȯ (�����)
}


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::ParticleManager::ParticleManager()
{
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::ParticleManager::~ParticleManager()
{
	// ��� ��ġ ����
	for (auto& batchNum : mp_Batches)
	{
		delete batchNum;
	}
}


/* - ��ƼŬ ��ġ�� �߰��ϴ� �Լ� (��ƼŬ ��ġ) | Add the particle batch (Particle batch) - */
void Fractal::ParticleManager::AddParticleBatch(Fractal::ParticleBatch* particleBatch)
{
	mp_Batches.push_back(particleBatch); // ��ƼŬ ��ġ�� �迭 ���� �߰� | After adding a particle batch, the Fractal::ParticleManager becomes responsible for deallocation
}


/* - ��ƼŬ ��ġ�� �����ϴ� �Լ� (��Ÿ �ð�) | Update the particle location (Delta time) - */
void Fractal::ParticleManager::Update(float deltaTime)
{
	// ��� ��ƼŬ�� �ܿ� ���� �ð� ���� (for each)
	for (auto& batchNum : mp_Batches)
	{
		batchNum->UpdateBatch(deltaTime);
	}
}


/* - ��ƼŬ�� �׸��� �Լ� (��������Ʈ ��ġ) | Draw the particle (Sprite batch) - */
void Fractal::ParticleManager::Draw(Fractal::SpriteBatch* spriteBatch)
{
	// ��� ��ƼŬ�� ��������Ʈ ��ġ�� ����� ��� (for each)
	for (auto& batchNum : mp_Batches)
	{
		spriteBatch->Start();
		batchNum->DrawBatch(spriteBatch);
		spriteBatch->End();
		spriteBatch->RenderBatch();
	}
}
