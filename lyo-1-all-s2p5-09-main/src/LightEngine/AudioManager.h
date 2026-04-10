#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>


struct ma_engine;
struct ma_sound;

typedef ma_sound  ma_sound_group;

using SoundHandle = size_t;
using SoundGroupHandle = size_t;


class AudioManager
{
private:

	ma_engine* m_engine;

	std::vector<ma_sound*> m_sounds;
	std::vector<ma_sound_group*> m_soundsGroups;

	AudioManager();
	~AudioManager();

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

public:

	static AudioManager& getInstance()
	{
		static AudioManager instance;
		return instance;
	}

	int load(const char* _path);
	int load(const char* _path, SoundGroupHandle _grpHandle);

	void play(SoundHandle _handle, uint64_t _framedelay);
	void stop(SoundHandle _handle, uint64_t _framedelay);

	void setVolume(SoundHandle _handle, float _volume);
	void setGroupVolume(SoundGroupHandle _grpHandle, float _volume);
	void setGeneralVolume(float _volume);

};

