#include "AudioManager.h"
#include "AudioMap.h"
#include "miniaudio.h"

AudioManager::AudioManager()
{
    m_engine = new ma_engine;
    ma_engine_config m_config = ma_engine_config_init();
    ma_result result = ma_engine_init(&m_config, m_engine);
    if (result != MA_SUCCESS)
        throw std::runtime_error("Error with the initialisation of miniaudio, code: " + std::to_string(result));
}

AudioManager::~AudioManager()
{
    for (ma_sound* s : m_sounds)
    {
        ma_sound_uninit(s);
        delete s;
    }

    for (size_t i = 0; i < m_soundsGroups.size(); i++)
    {
        ma_sound_group_uninit(m_soundsGroups[i]);
        delete m_soundsGroups[i];
    }

    ma_engine_uninit(m_engine);
    delete m_engine;
}

int AudioManager::load(const char* _path)
{
    ma_sound* sound = new ma_sound;
    if (ma_sound_init_from_file(m_engine, _path, 0, nullptr, nullptr, sound) != MA_SUCCESS)
    {
        delete sound;
        return -1;
    }

    m_sounds.push_back(sound);
    return m_sounds.size() - 1;
}

int AudioManager::load(const char* _path, SoundGroupHandle _grpHandle)
{
    if (_grpHandle > m_soundsGroups.size())
        return -1;

    if (_grpHandle == m_soundsGroups.size())
    {
        ma_sound_group* newGroup = new ma_sound_group;
        if (ma_sound_group_init(m_engine, 0, nullptr, newGroup) != MA_SUCCESS)
            return -1;
        m_soundsGroups.push_back(newGroup);
    }

    ma_sound* sound = new ma_sound;
    if (ma_sound_init_from_file(m_engine, _path, 0, m_soundsGroups[_grpHandle], NULL, sound) != MA_SUCCESS)
    {
        delete sound;
        return -1;
    }

    m_sounds.push_back(sound);
    return m_sounds.size() - 1;
}

void AudioManager::play(SoundHandle _handle, uint64_t _framedelay)
{
    if (_handle >= m_sounds.size())
        return;

    ma_sound* sound = m_sounds[_handle];
    if (ma_sound_is_playing(sound))
        return;

    ma_sound_seek_to_pcm_frame(sound, 0);
    ma_sound_set_fade_in_pcm_frames(sound, 0., 1., static_cast<ma_uint64>(_framedelay));
    ma_sound_start(sound);
}

void AudioManager::stop(SoundHandle _handle, uint64_t _framedelay)
{
    if (_handle >= m_sounds.size())
        return;

    ma_sound_set_stop_time_in_pcm_frames(m_sounds[_handle], ma_engine_get_time_in_pcm_frames(m_engine) + static_cast<ma_uint64>(_framedelay));
}

void AudioManager::setVolume(SoundHandle _handle, float _volume)
{
    if (_handle >= m_sounds.size())
        return;

    ma_sound_set_volume(m_sounds[_handle], _volume);
}

void AudioManager::setGroupVolume(SoundGroupHandle _grpHandle, float _volume)
{
    if (_grpHandle > m_soundsGroups.size() - 1)
        return;

    ma_sound_group_set_volume(m_soundsGroups[_grpHandle], _volume);
}

void AudioManager::setGeneralVolume(float _volume)
{
    ma_engine_set_volume(m_engine, _volume);
}
