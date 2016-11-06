#include "FractalParticle.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::Particle::Particle()
{
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::Particle::~Particle()
{
}


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::ParticleBatch::ParticleBatch()
{
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::ParticleBatch::~ParticleBatch()
{
	delete[] mp_particles; // mp_particles 배열 삭제
}


/* - 파티클 배치를 초기화 하는 함수 (최대 파티클 수, 감소율, 텍스쳐) | Initialize the Particle batch (The maximum number of particles, Reduction rate, Texture) - */
void Fractal::ParticleBatch::Init(int newMaxParticles, float newDecayRate, GLTexture newTexture, std::function<void(Particle&, float)> NewUpdateFunc) // NewUpdateFunc = ParticleUpdateDefault
{
	maxParticles = newMaxParticles; // 최대 파티클 수 저장
	mp_particles = new Particle[maxParticles]; // 최대 파티클 수 만큼 배열 할당
	decayRate = newDecayRate; // 감쇠 비율 저장
	texture = newTexture; // 파티클 텍스쳐 저장
	mp_UpdateFunc = NewUpdateFunc; // 업데이트 함수 포인터에 값 할당
}


/* - 파티클 위치를 갱신하는 함수 (델타 시간) | Update the particle location (Delta time) - */
void Fractal::ParticleBatch::UpdateBatch(float deltaTime)
{
	for (int i = 0; i < maxParticles; i++) // 모든 파티클 탐색
	{
		if (mp_particles[i].lifeTime > 0.0f) // 파티클이 유효한 경우
		{
			mp_UpdateFunc(mp_particles[i], deltaTime); // 함수 포인터를 사용하여 파티클 위치 업데이트
			mp_particles[i].lifeTime -= decayRate * deltaTime; // 감쇠 비율에 따른 생존 시간 감소
		}
	}
}


/* - 파티클 배치를 그리는 함수 (스프라이트 배치) | Draw the particle batch (Sprite batch) - */
void Fractal::ParticleBatch::DrawBatch(Fractal::SpriteBatch* spriteBatch)
{
	static glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f); // 텍스쳐 UV 만들기

	for (int i = 0; i < maxParticles; i++) // 모든 파티클 탐색
	{
		if (mp_particles[i].lifeTime > 0.0f) // 파티클이 유효한 경우
		{
			glm::vec4 destRect(mp_particles[i].position.x, mp_particles[i].position.y, mp_particles[i].width, mp_particles[i].width); // 파티클을 그리기 위한 사각형 만들기
			spriteBatch->Draw(destRect, uvRect, texture.id, 0.0f, mp_particles[i].color); // 스프라이트 배치 만들기
		}
	}
}


/* - 파티클을 추가하는 함수 (기본 파티클 위치, 기본 벡터, 기본 색, 기본 크기) | Add the particle (Default particle position, Default velocity, Default color, Default size) - */
void Fractal::ParticleBatch::AddParticle(const glm::vec2& newPosition, const glm::vec2& newVelocity, const Vertex::ColorRGBA8& newColor, float newWidth)
{
	int particleIndex = FindFreeParticle(); // 비어있는 파티클 배열 번호 찾기

	// 파티클 생성
	mp_particles[particleIndex].lifeTime = 1.0f;
	mp_particles[particleIndex].position = newPosition;
	mp_particles[particleIndex].velocity = newVelocity;
	mp_particles[particleIndex].color = newColor;
	mp_particles[particleIndex].width = newWidth;
}


/* - 비어있는 배열이 있는 경우 찾아서 위치를 반환하는 함수 | If has an empty array then find the location - */
int Fractal::ParticleBatch::FindFreeParticle()
{
	// 이전에 제거된 파티클 부터 파티클 끝까지 탐색
	for (int i = lastFreeParticle; i < maxParticles; i++)
	{
		if (mp_particles[i].lifeTime <= 0.0f) // 해당 파티클 생존 시간이 0.0f 인 경우
		{
			lastFreeParticle = i; // 마지막으로 제거된 파티클 번호 갱신
			return lastFreeParticle; // 비어있는 배열 번호 반환
		}
	}

	// 0 번 파티클 부터 이전에 제거된 파티클 까지 탐색
	for (int i = 0; i < lastFreeParticle; i++)
	{
		if (mp_particles[i].lifeTime <= 0.0f) // 해당 파티클 생존 시간이 0.0f 인 경우
		{
			lastFreeParticle = i; // 마지막으로 제거된 파티클 번호 갱신
			return lastFreeParticle; // 비어있는 배열 번호 반환
		}
	}

	return 0; // 비어있는 파티클 배열이 하나도 없는 경우 0 반환 (덮어쓰기)
}


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::ParticleManager::ParticleManager()
{
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::ParticleManager::~ParticleManager()
{
	// 모든 배치 제거
	for (auto& batchNum : mp_Batches)
	{
		delete batchNum;
	}
}


/* - 파티클 배치를 추가하는 함수 (파티클 배치) | Add the particle batch (Particle batch) - */
void Fractal::ParticleManager::AddParticleBatch(Fractal::ParticleBatch* particleBatch)
{
	mp_Batches.push_back(particleBatch); // 파티클 배치를 배열 끝에 추가 | After adding a particle batch, the Fractal::ParticleManager becomes responsible for deallocation
}


/* - 파티클 위치를 갱신하는 함수 (델타 시간) | Update the particle location (Delta time) - */
void Fractal::ParticleManager::Update(float deltaTime)
{
	// 모든 파티클의 잔여 생존 시간 갱신 (for each)
	for (auto& batchNum : mp_Batches)
	{
		batchNum->UpdateBatch(deltaTime);
	}
}


/* - 파티클을 그리는 함수 (스프라이트 배치) | Draw the particle (Sprite batch) - */
void Fractal::ParticleManager::Draw(Fractal::SpriteBatch* spriteBatch)
{
	// 모든 파티클을 스프라이트 배치로 만들어 출력 (for each)
	for (auto& batchNum : mp_Batches)
	{
		spriteBatch->Start();
		batchNum->DrawBatch(spriteBatch);
		spriteBatch->End();
		spriteBatch->RenderBatch();
	}
}
