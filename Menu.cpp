#include "Menu.h"
#include "GameState.h"
#include <graphics.h>
#include <conio.h>
#include <cmath>

Menu::Menu() : state(MENU_MAIN), selectedOption(0), frame(0), selectedTimeIndex(1), selectedLifeIndex(0) {}

void Menu::Draw() {
    switch (state) {
    case MENU_MAIN: drawMainMenu(frame); break;
    case MENU_START: drawStartMenu(); break;
    case MENU_SETTINGS: drawSettingsMenu(); break;
    case MENU_SETTINGS_BATTLE: drawSettingsBattleMenu(); break;
    }
    frame++;
}

void Menu::Update() {
    handleMouse();

    if (_kbhit()) {
        char ch = _getch();
        if (state == MENU_MAIN) {
            if (ch == 'w' && selectedOption > 0)
                selectedOption--;
            if (ch == 's' && selectedOption < 2)
                selectedOption++;
            if (ch == '\r')
                handleMainMenuSelect();
        }
        else if (state == MENU_START) {
            if (ch == 'w' && selectedOption > 0)
                selectedOption--;
            if (ch == 's' && selectedOption < 3)
                selectedOption++;
            if (ch == '\r')
                handleStartGameSelect();
        }
        else if (state == MENU_SETTINGS) {
            if (ch == 'w' && selectedOption > 0)
                selectedOption--;
            if (ch == 's' && selectedOption < 2)
                selectedOption++;
            if (ch == '\r')
                handleSettingsSelect();
        }
        else if (state == MENU_SETTINGS_BATTLE) {
            handleSettingsBattle();
        }
    }
}

MenuState Menu::GetState() const {
    return state;
}

void Menu::drawMainMenu(int frame) {
    cleardevice();

    IMAGE background;
    loadimage(&background, _T("rec/bg/Background1.png"), 1200, 600, 1);
    putimage(0, 0, &background);

    COLORREF color1 = RGB(0, 255, 255);
    COLORREF color2 = RGB(255, 0, 255);
    COLORREF color3 = RGB(155, 255, 0);

    int x = 150, y = 100;
    int dx1 = int(3 * sin(frame * 0.13));
    int dy1 = int(2 * cos(frame * 0.11));
    int dx2 = int(4 * sin(frame * 0.08 + 1.5));
    int dy2 = int(3 * cos(frame * 0.14 + 2.1));
    int dx3 = int(2 * sin(frame * 0.18 + 2.8));
    int dy3 = int(4 * cos(frame * 0.09 + 0.8));

    setbkmode(TRANSPARENT);
    settextstyle(80, 54, _T("Arial Black"));
    settextcolor(color1);
    outtextxy(x + dx1, y + dy1, _T("Armored Warriors"));

    settextstyle(76, 52, _T("Arial Black"));
    settextcolor(color2);
    outtextxy(x + dx2, y + dy2, _T("Armored Warriors"));

    settextstyle(72, 50, _T("Arial Black"));
    settextcolor(color3);
    outtextxy(x + dx3, y + dy3, _T("Armored Warriors"));

    settextcolor(BLACK);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(450, 300, selectedOption == 0 ? _T(">> 开始游戏 <<") : _T("     开始游戏"));
    outtextxy(450, 350, selectedOption == 1 ? _T(">> 游戏设置 <<") : _T("     游戏设置"));
    outtextxy(450, 400, selectedOption == 2 ? _T(">> 退出游戏 <<") : _T("     退出游戏"));
}

void Menu::drawStartMenu() {
    cleardevice();
    IMAGE background;
    loadimage(&background, _T("rec/bg/background2.png"), 1200, 600, 1);
    putimage(0, 0, &background);
    settextcolor(WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(450, 200, _T("请选择游戏模式"));
    outtextxy(450, 250, selectedOption == 0 ? _T(">> 双人对战 <<") : _T("     双人对战"));
    outtextxy(450, 300, selectedOption == 1 ? _T(">> 人机对战 <<") : _T("     人机对战"));
    outtextxy(450, 350, selectedOption == 2 ? _T(">> 练习模式 <<") : _T("     练习模式"));
    outtextxy(450, 400, selectedOption == 3 ? _T(">> 返回主菜单 <<") : _T("     返回主菜单"));
}

void Menu::drawSettingsMenu() {
    cleardevice();
    IMAGE background;
    loadimage(&background, _T("rec/bg/background3.png"), 1200, 600, 1);
    putimage(0, 0, &background);
    settextcolor(WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(430, 120, _T("请选择游戏设置"));
    outtextxy(430, 170, selectedOption == 0 ? _T(">> 音效设置 <<") : _T("     音效设置"));
    outtextxy(430, 220, selectedOption == 1 ? _T(">> 对战设置 <<") : _T("     对战设置"));
    outtextxy(430, 270, selectedOption == 2 ? _T(">> 返回主菜单 <<") : _T("     返回主菜单"));
}

void Menu::drawSettingsBattleMenu() {
    cleardevice();
    IMAGE background;
    loadimage(&background, _T("rec/bg/background3.png"), 1200, 600, 1);
    putimage(0, 0, &background);

    settextcolor(WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(440, 120, _T("对战参数设置"));

    settextstyle(27, 12, _T("Arial Black"));
    COLORREF highColor = RGB(255, 242, 0);

    settextcolor(selectedOption == 0 ? highColor : WHITE);
    TCHAR buf[64];
    _stprintf_s(buf, _T("对战时间：  %s  %s  %s"),
        selectedTimeIndex == 0 ? _T(">>30秒<<") : _T(" 30秒 "),
        selectedTimeIndex == 1 ? _T(">>60秒<<") : _T(" 60秒 "),
        selectedTimeIndex == 2 ? _T(">>90秒<<") : _T(" 90秒 ")
    );
    outtextxy(360, 170, buf);

    settextcolor(selectedOption == 1 ? highColor : WHITE);
    _stprintf_s(buf, _T("人物生命：  %s  %s  %s"),
        selectedLifeIndex == 0 ? _T(">>100<<") : _T(" 100 "),
        selectedLifeIndex == 1 ? _T(">>200<<") : _T(" 200 "),
        selectedLifeIndex == 2 ? _T(">>300<<") : _T(" 300 ")
    );
    outtextxy(360, 220, buf);

    settextcolor(selectedOption == 2 ? highColor : WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(450, 270, selectedOption == 2 ? _T(">> 返回 <<") : _T("     返回"));
}
void Menu::handleMouse() {
    if (state == MENU_MAIN) {
        // 用于主菜单选项的y坐标范围
        int opt_x1 = 450, opt_x2 = 750;
        int opt_y[3][2] = { {300,340}, {350,390}, {400,440} };

        // 处理鼠标事件
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;

            // 鼠标移动，高亮选项
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            // 鼠标左键点击，直接执行选项
            if (msg.uMsg == WM_LBUTTONDOWN) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                        handleMainMenuSelect();
                    }
                }
            }
        }
    }
    else if (state == MENU_START) {
        // 用于游戏选项的y坐标范围
        int opt_x1 = 450, opt_x2 = 750;
        int opt_y[4][2] = { {250,290}, {300,340}, {350,390},{400,440} };

        // 处理鼠标事件
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;

            // 鼠标移动，高亮选项
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 4; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            // 鼠标左键点击，直接执行选项
            if (msg.uMsg == WM_LBUTTONDOWN) {
                for (int i = 0; i < 4; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                        handleStartGameSelect();
                    }
                }
            }
        }
    }
    else if (state == MENU_SETTINGS) {
        // 用于设置选项的y坐标范围
        int opt_x1 = 430, opt_x2 = 730;
        int opt_y[3][2] = { {170,210}, {220,260}, {270,310} };

        // 处理鼠标事件
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;

            // 鼠标移动，高亮选项
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            // 鼠标左键点击，直接执行选项
            if (msg.uMsg == WM_LBUTTONDOWN) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                        handleSettingsSelect();
                    }
                }
            }
        }
    }
    else if (state == MENU_SETTINGS_BATTLE) {
        int opt_x1 = 400, opt_x2 = 900;
        int opt_y[3][2] = { { 170,210 }, { 220,260 }, { 270,310 } };
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            if (msg.uMsg == WM_LBUTTONDOWN) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                        if (selectedOption == 2) {
                            state = MENU_SETTINGS;
                            selectedOption = 1;
                        }
                        else if (selectedOption == 0) {
                            selectedTimeIndex = (selectedTimeIndex + 1) % 3;
                            battleTime = timeOptions[selectedTimeIndex];
                        }
                        else if (selectedOption == 1) {
                            selectedLifeIndex = (selectedLifeIndex + 1) % 3;
                            battleLife = lifeOptions[selectedLifeIndex];
                        }
                    }
                }
            }
        }
    }
}

void Menu::handleMainMenuSelect() {
    if (selectedOption == 0) {
        state = MENU_START;
        selectedOption = 0;
    }
    else if (selectedOption == 1) {
        state = MENU_SETTINGS;
        selectedOption = 0;
    }
    else if (selectedOption == 2) {
        closegraph();
        exit(0);
    }
}
void Menu::handleStartGameSelect() {
    if (selectedOption == 0)
        lastSelectedGameMode = GameMode::PVP;
    else if (selectedOption == 1)
        lastSelectedGameMode = GameMode::PVE;
    else if (selectedOption == 2)
        lastSelectedGameMode = GameMode::PRACTICE;
    else if (selectedOption == 3) {
        state = MENU_MAIN;
        selectedOption = 0;
    }
}
GameMode Menu::GetAndClearLastGameModeSelected() {
    GameMode tmp = lastSelectedGameMode;
    lastSelectedGameMode = GameMode::NONE;
    return tmp;
}

void Menu::handleSettingsSelect() {
    if (selectedOption == 0) {
        // 音效设置未实现
    }
    else if (selectedOption == 1) {
        state = MENU_SETTINGS_BATTLE;
        selectedOption = 0;
    }
    else if (selectedOption == 2) {
        state = MENU_MAIN;
        selectedOption = 0;
    }
}

void Menu::handleSettingsBattle() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'w' && selectedOption > 0) selectedOption--;
        if (ch == 's' && selectedOption < 2) selectedOption++;

        if (selectedOption == 0) {
            if (ch == 'a' && selectedTimeIndex > 0) selectedTimeIndex--;
            if (ch == 'd' && selectedTimeIndex < 2) selectedTimeIndex++;
        }
        if (selectedOption == 1) {
            if (ch == 'a' && selectedLifeIndex > 0) selectedLifeIndex--;
            if (ch == 'd' && selectedLifeIndex < 2) selectedLifeIndex++;
        }

        if (ch == '\r') {
            if (selectedOption == 2) {
                state = MENU_SETTINGS;
                selectedOption = 1;
            }
            else {
                battleTime = timeOptions[selectedTimeIndex];
                battleLife = lifeOptions[selectedLifeIndex];
            }
        }
        if (ch == 27) {
            state = MENU_SETTINGS;
            selectedOption = 1;
        }
    }
}