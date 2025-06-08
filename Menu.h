#pragma once
#include <graphics.h>
#include "GameState.h"

enum MenuState {
    MENU_MAIN,
    MENU_START,
    MENU_SETTINGS,
    MENU_SETTINGS_BATTLE,
    MENU_SETTINGS_AUDIO,
    MENU_EXIT
};

class Menu {
public:
    Menu();
    void Draw();
    void Update();
    MenuState GetState() const;
    GameMode GetAndClearLastGameModeSelected();

    int battleLife = 100;
    int battleTime = 60;

    int bgmVolume = 100;      // 0~100
    int cheerVolume = 100;    // 0~100
    int effectVolume = 100;   // 0~100

private:
    MenuState state;
    int frame = 0;
    int selectedOption;

    int timeOptions[3] = { 30, 60, 90 };
    int lifeOptions[3] = { 100, 200, 300 };
    int selectedTimeIndex = 1;
    int selectedLifeIndex = 0;

    void drawMainMenu(int frame);
    void drawStartMenu();
    void drawSettingsMenu();
    void drawSettingsBattleMenu();
    void drawSettingsAudioMenu(); 

    void handleMouse();
    void handleMainMenuSelect();
    void handleStartGameSelect();
    void handleSettingsSelect();
    void handleSettingsBattle();
    void handleSettingsAudio(); 

    GameMode lastSelectedGameMode = GameMode::NONE;
};