#include "Menu.h"
#include "GameState.h"
#include <graphics.h>
#include <conio.h>
#include <cmath>

Menu::Menu() : state(MENU_MAIN), selectedOption(0), frame(0), selectedTimeIndex(1), selectedLifeIndex(0), bgmVolume(100), cheerVolume(100), effectVolume(100) {}

void Menu::Draw() {
    switch (state) {
    case MENU_MAIN: drawMainMenu(frame); break;
    case MENU_START: drawStartMenu(); break;
    case MENU_SETTINGS: drawSettingsMenu(); break;
    case MENU_SETTINGS_BATTLE: drawSettingsBattleMenu(); break;
    case MENU_SETTINGS_AUDIO: drawSettingsAudioMenu(); break;
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
        else if (state == MENU_SETTINGS_AUDIO) {
            handleSettingsAudio();
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

    settextstyle(45, 0, _T("Arial Black"));
    COLORREF highColor = RGB(255, 242, 0);

    settextcolor(selectedOption == 0 ? highColor : WHITE);
    TCHAR buf[64];
    _stprintf_s(buf, _T("时间  %s  %s  %s"),
        selectedTimeIndex == 0 ? _T(">>30秒<<") : _T(" 30秒 "),
        selectedTimeIndex == 1 ? _T(">>60秒<<") : _T(" 60秒 "),
        selectedTimeIndex == 2 ? _T(">>90秒<<") : _T(" 90秒 ")
    );
    outtextxy(330, 180, buf);

    settextcolor(selectedOption == 1 ? highColor : WHITE);
    _stprintf_s(buf, _T("生命 %s  %s  %s"),
        selectedLifeIndex == 0 ? _T(">>100<<") : _T(" 100 "),
        selectedLifeIndex == 1 ? _T(">>200<<") : _T(" 200 "),
        selectedLifeIndex == 2 ? _T(">>300<<") : _T(" 300 ")
    );
    outtextxy(330, 230, buf);

    settextcolor(selectedOption == 2 ? highColor : WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(450, 280, selectedOption == 2 ? _T(">> 返回 <<") : _T("     返回"));
}

void Menu::drawSettingsAudioMenu() {
    cleardevice();
    IMAGE background;
    loadimage(&background, _T("rec/bg/background3.png"), 1200, 600, 1);
    putimage(0, 0, &background);

    settextcolor(WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(440, 120, _T("音效参数设置"));

    settextstyle(27, 12, _T("Arial Black"));
    COLORREF highColor = RGB(255, 242, 0);

    TCHAR buf[64];
    int baseY = 170, stepY = 50;

    settextcolor(selectedOption == 0 ? highColor : WHITE);
    settextstyle(45, 0, _T("Arial Black"));
    _stprintf_s(buf, _T("BGM音量：%d"), bgmVolume);
    outtextxy(440, baseY, buf);
   
    outtextxy(400, baseY, _T("<"));
    outtextxy(700, baseY, _T(">"));

    settextcolor(selectedOption == 1 ? highColor : WHITE);
    _stprintf_s(buf, _T("欢呼音量：%d"), cheerVolume);
    outtextxy(440, baseY + stepY, buf);
    outtextxy(400, baseY + stepY, _T("<"));
    outtextxy(700, baseY + stepY, _T(">"));

    settextcolor(selectedOption == 2 ? highColor : WHITE);
    _stprintf_s(buf, _T("攻击音量：%d"), effectVolume);
    outtextxy(440, baseY + 2 * stepY, buf);
    outtextxy(400, baseY + 2 * stepY, _T("<"));
    outtextxy(700, baseY + 2 * stepY, _T(">"));

    // 返回
    settextcolor(selectedOption == 3 ? highColor : WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(450, baseY + 3 * stepY, selectedOption == 3 ? _T(">> 返回 <<") : _T("     返回"));
}

void Menu::handleMouse() {
    if (state == MENU_MAIN) {
        int opt_x1 = 450, opt_x2 = 750;
        int opt_y[3][2] = { {300,340}, {350,390}, {400,440} };

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
                        handleMainMenuSelect();
                    }
                }
            }
        }
    }
    else if (state == MENU_START) {
        int opt_x1 = 450, opt_x2 = 750;
        int opt_y[4][2] = { {250,290}, {300,340}, {350,390},{400,440} };

        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;

            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 4; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
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
        int opt_x1 = 430, opt_x2 = 730;
        int opt_y[3][2] = { {170,210}, {220,260}, {270,310} };

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
                        handleSettingsSelect();
                    }
                }
            }
        }
    }
    else if (state == MENU_SETTINGS_BATTLE) {
        int opt_x1 = 400, opt_x2 = 900;
        int opt_y[3][2] = { { 180,220 }, { 230,270 }, { 280,320 } };
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
    else if (state == MENU_SETTINGS_AUDIO) {
        int baseY = 180, stepY = 50;
        int leftBtnX1 = 400, leftBtnX2 = 440;
        int rightBtnX1 = 700, rightBtnX2 = 740;
        int valueX1 = 440, valueX2 = 690;
        int optY[4][2] = { {baseY,baseY + 40}, {baseY + stepY,baseY + stepY + 40}, {baseY + 2 * stepY,baseY + 2 * stepY + 40}, {baseY + 3 * stepY,baseY + 3 * stepY + 40} };
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;
            // 鼠标移动，高亮选项
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 4; ++i) {
                    if (mx >= valueX1 && mx <= valueX2 && my >= optY[i][0] && my <= optY[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            // 鼠标左键点击
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // BGM音量
                if (my >= optY[0][0] && my <= optY[0][1]) {
                    if (mx >= leftBtnX1 && mx <= leftBtnX2 && bgmVolume > 0) {
                        bgmVolume -= 5;
                        if (bgmVolume < 0) bgmVolume = 0;
                    }
                    else if (mx >= rightBtnX1 && mx <= rightBtnX2 && bgmVolume < 100) {
                        bgmVolume += 5;
                        if (bgmVolume > 100) bgmVolume = 100;
                    }
                }
                // Cheer音量
                else if (my >= optY[1][0] && my <= optY[1][1]) {
                    if (mx >= leftBtnX1 && mx <= leftBtnX2 && cheerVolume > 0) {
                        cheerVolume -= 5;
                        if (cheerVolume < 0) cheerVolume = 0;
                    }
                    else if (mx >= rightBtnX1 && mx <= rightBtnX2 && cheerVolume < 100) {
                        cheerVolume += 5;
                        if (cheerVolume > 100) cheerVolume = 100;
                    }
                }
                // Effect音量
                else if (my >= optY[2][0] && my <= optY[2][1]) {
                    if (mx >= leftBtnX1 && mx <= leftBtnX2 && effectVolume > 0) {
                        effectVolume -= 5;
                        if (effectVolume < 0) effectVolume = 0;
                    }
                    else if (mx >= rightBtnX1 && mx <= rightBtnX2 && effectVolume < 100) {
                        effectVolume += 5;
                        if (effectVolume > 100) effectVolume = 100;
                    }
                }
                // 返回
                else if (my >= optY[3][0] && my <= optY[3][1]) {
                    state = MENU_SETTINGS;
                    selectedOption = 0;
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
        state = MENU_SETTINGS_AUDIO;
        selectedOption = 0;
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

void Menu::handleSettingsAudio() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'w' && selectedOption > 0) selectedOption--;
        if (ch == 's' && selectedOption < 3) selectedOption++;

        if (selectedOption == 0 && (ch == 'a' || ch == 'A') && bgmVolume > 0) bgmVolume -= 5;
        if (selectedOption == 0 && (ch == 'd' || ch == 'D') && bgmVolume < 100) bgmVolume += 5;

        if (selectedOption == 1 && (ch == 'a' || ch == 'A') && cheerVolume > 0) cheerVolume -= 5;
        if (selectedOption == 1 && (ch == 'd' || ch == 'D') && cheerVolume < 100) cheerVolume += 5;

        if (selectedOption == 2 && (ch == 'a' || ch == 'A') && effectVolume > 0) effectVolume -= 5;
        if (selectedOption == 2 && (ch == 'd' || ch == 'D') && effectVolume < 100) effectVolume += 5;

        if (ch == '\r' && selectedOption == 3) {
            state = MENU_SETTINGS;
            selectedOption = 0;
        }
        if (ch == 27) {
            state = MENU_SETTINGS;
            selectedOption = 0;
        }
    }
}