#include "AudioManager.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

bool AudioManager::isBGMPlaying = false;
bool AudioManager::isCheerPlaying = false;
std::string AudioManager::currentBGM = "";
std::string AudioManager::currentCheer = "";

int AudioManager::bgmVolume = 100;
int AudioManager::cheerVolume = 100;
int AudioManager::effectVolume = 100;

void AudioManager::SetBGMVolume(int volume) {
    bgmVolume = volume;
    mciSendStringA(("setaudio bgm volume to " + std::to_string(volume * 10)).c_str(), 0, 0, 0);
}
void AudioManager::SetCheerVolume(int volume) {
    cheerVolume = volume;
    mciSendStringA(("setaudio cheer volume to " + std::to_string(volume * 10)).c_str(), 0, 0, 0);
}
void AudioManager::SetEffectVolume(int volume) {
    effectVolume = volume;
    mciSendStringA(("setaudio effect volume to " + std::to_string(volume * 10)).c_str(), 0, 0, 0);
}

void AudioManager::PlayBGM(const std::string& filename, bool loop) {
    if (isBGMPlaying && currentBGM == filename) return;
    StopBGM();
    std::string cmd = "open \"" + filename + "\" alias bgm";
    mciSendStringA(cmd.c_str(), 0, 0, 0);
    std::string playCmd = "play bgm";
    if (loop) playCmd += " repeat";
    mciSendStringA(playCmd.c_str(), 0, 0, 0);
    SetBGMVolume(bgmVolume); 
    isBGMPlaying = true;
    currentBGM = filename;
}
void AudioManager::StopBGM() {
    mciSendStringA("stop bgm", 0, 0, 0);
    mciSendStringA("close bgm", 0, 0, 0);
    isBGMPlaying = false;
    currentBGM = "";
}

void AudioManager::PlayCheer(const std::string& filename, bool loop) {
    if (isCheerPlaying && currentCheer == filename) return;
    StopCheer();
    std::string cmd = "open \"" + filename + "\" alias cheer";
    mciSendStringA(cmd.c_str(), 0, 0, 0);
    std::string playCmd = "play cheer";
    if (loop) playCmd += " repeat";
    mciSendStringA(playCmd.c_str(), 0, 0, 0);
    SetCheerVolume(cheerVolume);
    isCheerPlaying = true;
    currentCheer = filename;
}
void AudioManager::StopCheer() {
    mciSendStringA("stop cheer", 0, 0, 0);
    mciSendStringA("close cheer", 0, 0, 0);
    isCheerPlaying = false;
    currentCheer = "";
}

void AudioManager::PlayEffect(const std::string& filename) {
    static std::string lastEffect = "";
    if (lastEffect != filename) {
        mciSendStringA("close effect", 0, 0, 0);
        std::string cmd = "open \"" + filename + "\" alias effect";
        mciSendStringA(cmd.c_str(), 0, 0, 0);
        lastEffect = filename;
    }
    SetEffectVolume(effectVolume); 
    mciSendStringA("play effect from 0", 0, 0, 0);
}

void AudioManager::StopAll() {
    StopBGM();
    StopCheer();
    mciSendStringA("close effect", 0, 0, 0);
}