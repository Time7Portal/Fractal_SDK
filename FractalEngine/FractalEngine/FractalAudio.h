#pragma once
#include <MIXER\SDL_mixer.h>
#include <map>
#include "FractalDebug.h"


namespace Fractal
{
	// Ŭ���� ��� �� �̸� ���� | Defined the class before using
	class AudioMusic;
	class AudioEffect;


	class AudioManager
	{
	public:
		AudioManager(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~AudioManager(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		void Init(); // ����� ������ �ʱ�ȭ �ϴ� �Լ� | Initialize the audio engine
		void Destroy(); // ����� ������ ���� �ϴ� �Լ� | Remove the audio engine

		AudioMusic LoadAudioMusic(const std::string& filePath); // ��������� �ε� �ϴ� �Լ� | Load the background music
		AudioEffect LoadAudioEffect(const std::string& filePath); // ȿ������ �ε� �ϴ� �Լ� | Load the Sound effect
		
	private:
		bool isInit = false; // ����� ���� �ʱ�ȭ ���� ���� | Is audio engine initialized

		std::map<std::string, Mix_Chunk*> m_EffectMap; // ȿ���� ����� ĳ�� �� | Cache maps for storing sound effects
		std::map<std::string, Mix_Music*> m_MusicMap; // ���� ����� ĳ�� �� | Cache maps for storing background musics
	};


	class AudioMusic
	{
		friend class AudioManager; // AudioEngine Ŭ�������� ���� ����ϱ� | Allow access from AudioEngine class
	public:
		void Play(int loops = 1); // ������ ��� �ϴ� �Լ� (�ݺ� �� -1 �̸� ����) | Play the background music (-1 is infinite)
		void Pause(); // ������ �Ͻ����� �ϴ� �Լ� | Pause the background music
		void Resume();  // ������ ����� �ϴ� �Լ� | Resume the background music
		void Stop(); // ������ ���� �ϴ� �Լ� | Stop the background music
		static void Volume(int newVolume = 50); // ���� ������ �����ϴ� �Լ� (����) | Control the music volume (Volume)

	private:
		Mix_Music* mp_Music = nullptr; // SDL ���� �ɹ� ������ | SDL background music member pointer
	};


	class AudioEffect
	{
		friend class AudioManager; // AudioEngine Ŭ�������� ���� ����ϱ� | Allow access from AudioEngine class
	public:
		void Play(int loops = 0); // ȿ������ ��� �ϴ� �Լ� | Play the sound effect

	private:
		Mix_Chunk* mp_Chunk = nullptr; // SDL ȿ���� �ɹ� ������ | SDL sound effect member pointer
	};
}
