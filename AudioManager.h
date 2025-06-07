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

private:
    static bool isBGMPlaying;
    static bool isCheerPlaying;
    static std::string currentBGM;
    static std::string currentCheer;
};