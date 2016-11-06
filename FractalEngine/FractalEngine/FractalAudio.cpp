#include "FractalAudio.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::AudioManager::AudioManager()
{
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::AudioManager::~AudioManager()
{
	Destroy(); // Ŭ���� ����� �ͼ� ����
}


/* - ����� ������ �ʱ�ȭ �ϴ� �Լ� | Initialize the audio engine - */
void Fractal::AudioManager::Init()
{
	// ����� ������ �̹� �ʱ�ȭ �Ǿ��ִ� ���
	if (isInit == true)
	{
		std::cout << "[Console] The audio engine has already been initialized..." << std::endl;
	}
	else
	{
		// Ȯ���ں� ����� �ͼ� �ʱ�ȭ ���� "Mix_Init(�ʱ�ȭ�� Ȯ����)"
		if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG) == -1) // �ͼ� �ʱ�ȭ�� ������ ���
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred initialization of audio mixer!");
		}

		// ����� ���� ���� "Mix_OpenAudio(���� ����Ʈ, ��� ����, ����� ü��, �ִ� ���� ���� ��)"
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) // ����� ���� ������ ������ ���
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred initialization of audio mixer!");
		}

		// �ʱ�ȭ ���� ���� Ȱ��ȭ
		isInit = true;
	}
}


/* - ����� ������ ���� �ϴ� �Լ� | Remove the audio engine - */
void Fractal::AudioManager::Destroy()
{
	// �ͼ� �ʱ�ȭ�� �Ǿ��ִ� ���
	if (isInit)
	{
		// �ʱ�ȭ ���� ���� ���
		isInit = false;

		// ���� ĳ�ø� ���� ���� (for each)
		for (auto& ait_m : m_MusicMap)
		{
			Mix_FreeMusic(ait_m.second);
		}
		m_MusicMap.clear();

		// ȿ���� ĳ�ø� ���� ���� (for each)
		for (auto& ait_e : m_EffectMap)
		{
			Mix_FreeChunk(ait_e.second);
		}
		m_EffectMap.clear();

		// �ͼ� ���� �� ����
		Mix_CloseAudio();
		Mix_Quit();
	}
}


/* - ��������� �ε� �ϴ� �Լ� | Load the background music - */
Fractal::AudioMusic Fractal::AudioManager::LoadAudioMusic(const std::string& filePath)
{
	// ����� ���� ����
	AudioMusic m_Music;

	// ����� �����Ͱ� �̹� ĳ�̵Ǿ����� ã�� | Try to find the audio file in the cache
	auto ait = m_MusicMap.find(filePath);

	// ã�� ���� ��� | If it's not in the map
	if (ait == m_MusicMap.end())
	{
		// ûũ ������ ���� �ε�
		Mix_Music* newMusic = Mix_LoadMUS(filePath.c_str());

		// ���� �ε忡 ������ ���
		if (newMusic == nullptr)
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred loading of audio music!");
		}

		// ĳ���ϱ� | Insert it into the map
		std::cout << "[Console] Caching the new audio music..." << std::endl;
		m_Music.mp_Music = newMusic;
		m_MusicMap[filePath] = newMusic;
	}
	else // ã�� ��� ĳ�õ� ������ ��ȯ
	{
		std::cout << "[Console] Loading the cached audio music..." << std::endl;
		m_Music.mp_Music = ait->second;
	}

	return m_Music;
}


/* - ȿ������ �ε� �ϴ� �Լ� | Load the Sound effects - */
Fractal::AudioEffect Fractal::AudioManager::LoadAudioEffect(const std::string& filePath)
{
	// ����� ����Ʈ ����
	AudioEffect m_Effect;

	// ����� �����Ͱ� �̹� ĳ�̵Ǿ����� ã�� | Try to find the audio file in the cache
	auto ait = m_EffectMap.find(filePath);

	// ã�� ���� ��� | If it's not in the map
	if (ait == m_EffectMap.end())
	{
		// ûũ ������ ���� �ε�
		Mix_Chunk* newChunk = Mix_LoadWAV(filePath.c_str());

		// ���� �ε忡 ������ ���
		if (newChunk == nullptr)
		{
			Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred loading of audio effect!");
		}

		// ĳ���ϱ� | Insert it into the map
		std::cout << "[Console] Caching the new audio effect..." << std::endl;
		m_Effect.mp_Chunk = newChunk;
		m_EffectMap[filePath] = newChunk;
	}
	else // ã�� ��� ĳ�õ� ������ ��ȯ
	{
		std::cout << "[Console] Loading the cached audio effect..." << std::endl;
		m_Effect.mp_Chunk = ait->second;
	}

	return m_Effect;
}


/* - ������ ��� �ϴ� �Լ� (�ݺ� �� -1 �̸� ����) | Play the background music (-1 is infinite) - */
void Fractal::AudioMusic::Play(int loops)
{
	// ���� ���
	Mix_PlayMusic(mp_Music, loops);
}


/* - ������ �Ͻ����� �ϴ� �Լ� | Pause the background music - */
void Fractal::AudioMusic::Pause()
{
	Mix_PauseMusic();
}


/* - ������ ����� �ϴ� �Լ� | Resume the background music - */
void Fractal::AudioMusic::Resume()
{
	Mix_ResumeMusic();
}


/* - ������ ���� �ϴ� �Լ� | Stop the background music - */
void Fractal::AudioMusic::Stop()
{
	Mix_HaltMusic();
}


/* - ���� ������ �����ϴ� �Լ� (����) | Control the music volume (Volume) - */
void Fractal::AudioMusic::Volume(int newVolume)
{
	Mix_VolumeMusic(newVolume);
}


/* - ȿ������ ��� �ϴ� �Լ� | Play the sound effect - */
void Fractal::AudioEffect::Play(int loops)
{
	// ����� ����Ʈ ��� "Mix_PlayChannel(�Ҵ��� ü�� ��ȣ, ����, �ݺ� ��)"
	if (Mix_PlayChannel(0, mp_Chunk, loops) == -1) // ����� ����Ʈ ����� ������ ���
	{
		Fractal::Debug::GetInstance()->ConsoleError(std::string(Mix_GetError()) + " - Error occurred during the audio effect playback!");
	}
}
