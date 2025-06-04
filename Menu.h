#pragma once
#include <graphics.h> // EasyX ͷ�ļ�
#include"GameState.h"
enum MenuState {
    MENU_MAIN,
    MENU_START,
    MENU_SETTINGS,
	MENU_EXIT
    // ����չ�����˵�
};
enum MenuAnimState {
    ANIM_NONE,
    ANIM_TO_START,    // �Ŵ�
    ANIM_TO_MAIN      // ��С
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
    int frame = 0; // ���ڶ���Ч��
    int selectedOption;

    // ���˵�����
    void drawMainMenu(int frame);
    // ��ʼ�˵�����
    void drawStartMenu();
    // ���ò˵�����
    void drawSettingsMenu();

    // ��괦�������͵����
    void handleMouse();
    // ���˵�ѡ��ȷ�ϣ����̻س����������
    void handleMainMenuSelect();
	// �����˵�ѡ��ȷ��
	void handleStartGameSelect();
	void handleSettingsSelect();
    GameMode lastSelectedGameMode = GameMode::NONE;
};