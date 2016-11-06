#pragma once
#include <MIXER\SDL_mixer.h>
#include <map>
#include "FractalDebug.h"


namespace Fractal
{
	// 클래스 사용 전 미리 정의 | Defined the class before using
	class AudioMusic;
	class AudioEffect;


	class AudioManager
	{
	public:
		AudioManager(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~AudioManager(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		void Init(); // 오디오 엔진을 초기화 하는 함수 | Initialize the audio engine
		void Destroy(); // 오디오 엔진을 제거 하는 함수 | Remove the audio engine

		AudioMusic LoadAudioMusic(const std::string& filePath); // 배경음악을 로드 하는 함수 | Load the background music
		AudioEffect LoadAudioEffect(const std::string& filePath); // 효과음을 로드 하는 함수 | Load the Sound effect
		
	private:
		bool isInit = false; // 오디오 엔진 초기화 성공 여부 | Is audio engine initialized

		std::map<std::string, Mix_Chunk*> m_EffectMap; // 효과음 저장용 캐시 맵 | Cache maps for storing sound effects
		std::map<std::string, Mix_Music*> m_MusicMap; // 음악 저장용 캐시 맵 | Cache maps for storing background musics
	};


	class AudioMusic
	{
		friend class AudioManager; // AudioEngine 클래스에서 접근 허용하기 | Allow access from AudioEngine class
	public:
		void Play(int loops = 1); // 음악을 재생 하는 함수 (반복 수 -1 이면 무한) | Play the background music (-1 is infinite)
		void Pause(); // 음악을 일시정지 하는 함수 | Pause the background music
		void Resume();  // 음악을 재시작 하는 함수 | Resume the background music
		void Stop(); // 음악을 정지 하는 함수 | Stop the background music
		static void Volume(int newVolume = 50); // 음악 볼륨을 조정하는 함수 (볼륨) | Control the music volume (Volume)

	private:
		Mix_Music* mp_Music = nullptr; // SDL 음악 맴버 포인터 | SDL background music member pointer
	};


	class AudioEffect
	{
		friend class AudioManager; // AudioEngine 클래스에서 접근 허용하기 | Allow access from AudioEngine class
	public:
		void Play(int loops = 0); // 효과음을 재생 하는 함수 | Play the sound effect

	private:
		Mix_Chunk* mp_Chunk = nullptr; // SDL 효과음 맴버 포인터 | SDL sound effect member pointer
	};
}
