#pragma once
#include <graphics.h> // EasyX 头文件
#include"GameState.h"
enum MenuState {
    MENU_MAIN,
    MENU_START,
    MENU_SETTINGS,
	MENU_EXIT
    // 可扩展其他菜单
};
enum MenuAnimState {
    ANIM_NONE,
    ANIM_TO_START,    // 放大
    ANIM_TO_MAIN      // 缩小
};

class Menu {
public:
    Menu();
    void Draw();
    void Update();
    MenuState GetState() const;
    GameMode GetAndClearLastGameModeSelected();
private:
    MenuState state;
    int frame = 0; // 用于动画效果
    int selectedOption;

    // 主菜单绘制
    void drawMainMenu(int frame);
    // 开始菜单绘制
    void drawStartMenu();
    // 设置菜单绘制
    void drawSettingsMenu();

    // 鼠标处理（高亮和点击）
    void handleMouse();
    // 主菜单选项确认（键盘回车或鼠标点击）
    void handleMainMenuSelect();
	// 其他菜单选项确认
	void handleStartGameSelect();
	void handleSettingsSelect();
    GameMode lastSelectedGameMode = GameMode::NONE;
};