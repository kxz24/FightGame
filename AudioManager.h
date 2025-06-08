#pragma once
#include <string>

class AudioManager {
public:
    static void PlayBGM(const std::string& filename, bool loop = true);
    static void StopBGM();

    static void PlayCheer(const std::string& filename, bool loop = true);
    static void StopCheer();

    static void PlayEffect(const std::string& filename);

    static void StopAll();

    static void SetBGMVolume(int volume);
    static void SetCheerVolume(int volume);
    static void SetEffectVolume(int volume);

private:
    static bool isBGMPlaying;
    static bool isCheerPlaying;
    static std::string currentBGM;
    static std::string currentCheer;

    static int bgmVolume;
    static int cheerVolume;
    static int effectVolume;
};