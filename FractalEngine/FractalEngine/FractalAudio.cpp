#include "FractalAudio.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::AudioManager::AudioManager()
{
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::AudioManager::~AudioManager()
{
	Destroy(); // 클래스 종료시 믹서 제거
}


/* - 오디오 엔진을 초기화 하는 함수 | Initialize the audio engine - */
void Fractal::AudioManager::Init()
{
	// 오디오 엔진이 이미 초기화 되어있는 경우
	if (isInit == true)
	{
		std::cout << "[Console] The audio engine has already been initialized..." << std::endl;
	}
	else
	{
		// 확장자별 오디오 믹서 초기화 시작 "Mix_Init(초기화될 확장자)"
		if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG) == -1) // 믹서 초기화에 실패한 경우
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred initialization of audio mixer!");
		}

		// 오디오 엔진 설정 "Mix_OpenAudio(샘플 레이트, 출력 형식, 오디오 체널, 최대 동시 음원 수)"
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) // 오디오 엔진 설정에 실패한 경우
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred initialization of audio mixer!");
		}

		// 초기화 성공 여부 활성화
		isInit = true;
	}
}


/* - 오디오 엔진을 제거 하는 함수 | Remove the audio engine - */
void Fractal::AudioManager::Destroy()
{
	// 믹서 초기화가 되어있는 경우
	if (isInit)
	{
		// 초기화 성공 여부 취소
		isInit = false;

		// 뮤직 캐시맵 전부 제거 (for each)
		for (auto& ait_m : m_MusicMap)
		{
			Mix_FreeMusic(ait_m.second);
		}
		m_MusicMap.clear();

		// 효과음 캐시맵 전부 제거 (for each)
		for (auto& ait_e : m_EffectMap)
		{
			Mix_FreeChunk(ait_e.second);
		}
		m_EffectMap.clear();

		// 믹서 제거 후 종료
		Mix_CloseAudio();
		Mix_Quit();
	}
}


/* - 배경음악을 로드 하는 함수 | Load the background music - */
Fractal::AudioMusic Fractal::AudioManager::LoadAudioMusic(const std::string& filePath)
{
	// 오디오 뮤직 정의
	AudioMusic m_Music;

	// 오디오 데이터가 이미 캐싱되었는지 찾기 | Try to find the audio file in the cache
	auto ait = m_MusicMap.find(filePath);

	// 찾지 못한 경우 | If it's not in the map
	if (ait == m_MusicMap.end())
	{
		// 청크 단위로 음원 로드
		Mix_Music* newMusic = Mix_LoadMUS(filePath.c_str());

		// 음원 로드에 실패한 경우
		if (newMusic == nullptr)
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred loading of audio music!");
		}

		// 캐싱하기 | Insert it into the map
		std::cout << "[Console] Caching the new audio music..." << std::endl;
		m_Music.mp_Music = newMusic;
		m_MusicMap[filePath] = newMusic;
	}
	else // 찾은 경우 캐시된 데이터 반환
	{
		std::cout << "[Console] Loading the cached audio music..." << std::endl;
		m_Music.mp_Music = ait->second;
	}

	return m_Music;
}


/* - 효과음을 로드 하는 함수 | Load the Sound effects - */
Fractal::AudioEffect Fractal::AudioManager::LoadAudioEffect(const std::string& filePath)
{
	// 오디오 이펙트 정의
	AudioEffect m_Effect;

	// 오디오 데이터가 이미 캐싱되었는지 찾기 | Try to find the audio file in the cache
	auto ait = m_EffectMap.find(filePath);

	// 찾지 못한 경우 | If it's not in the map
	if (ait == m_EffectMap.end())
	{
		// 청크 단위로 음원 로드
		Mix_Chunk* newChunk = Mix_LoadWAV(filePath.c_str());

		// 음원 로드에 실패한 경우
		if (newChunk == nullptr)
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred loading of audio effect!");
		}

		// 캐싱하기 | Insert it into the map
		std::cout << "[Console] Caching the new audio effect..." << std::endl;
		m_Effect.mp_Chunk = newChunk;
		m_EffectMap[filePath] = newChunk;
	}
	else // 찾은 경우 캐시된 데이터 반환
	{
		std::cout << "[Console] Loading the cached audio effect..." << std::endl;
		m_Effect.mp_Chunk = ait->second;
	}

	return m_Effect;
}


/* - 음악을 재생 하는 함수 (반복 수 -1 이면 무한) | Play the background music (-1 is infinite) - */
void Fractal::AudioMusic::Play(int loops)
{
	// 뮤직 재생
	Mix_PlayMusic(mp_Music, loops);
}


/* - 음악을 일시정지 하는 함수 | Pause the background music - */
void Fractal::AudioMusic::Pause()
{
	Mix_PauseMusic();
}


/* - 음악을 재시작 하는 함수 | Resume the background music - */
void Fractal::AudioMusic::Resume()
{
	Mix_ResumeMusic();
}


/* - 음악을 정지 하는 함수 | Stop the background music - */
void Fractal::AudioMusic::Stop()
{
	Mix_HaltMusic();
}


/* - 음악 볼륨을 조정하는 함수 (볼륨) | Control the music volume (Volume) - */
void Fractal::AudioMusic::Volume(int newVolume)
{
	Mix_VolumeMusic(newVolume);
}


/* - 효과음을 재생 하는 함수 | Play the sound effect - */
void Fractal::AudioEffect::Play(int loops)
{
	// 오디오 이펙트 재생 "Mix_PlayChannel(할당할 체널 번호, 음원, 반복 수)"
	if (Mix_PlayChannel(0, mp_Chunk, loops) == -1) // 오디오 이펙트 재생에 실패한 경우
	{
		Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred during the audio effect playback!");
	}
}
