#include "Menu.h"
#include"GameState.h"
#include <graphics.h> // EasyXͷ�ļ�
#include <conio.h>
#include <cmath> 

Menu::Menu() : state(MENU_MAIN), selectedOption(0), frame(0) {}

void Menu::Draw() {
    switch (state) {
    case MENU_MAIN: drawMainMenu(frame); break;
    case MENU_START: drawStartMenu(); break;
    case MENU_SETTINGS: drawSettingsMenu(); break;
        // �����˵�
    }
    frame++;
}

void Menu::Update() {
    // --- ��괦�� ---
    handleMouse();

    // --- ���̴��� ----
    if (_kbhit()) {
        char ch = _getch();
        if (state == MENU_MAIN) {
            //�������˵���ѡ��
            if (ch == 'w' && selectedOption > 0)
                selectedOption--;
            if (ch == 's' && selectedOption < 2) 
                selectedOption++;
            if (ch == '\r')
                handleMainMenuSelect();
        }
        else if (state == MENU_START) {
            // ��������ģʽ��ѡ��
            if (ch == 'w' && selectedOption > 0)
                selectedOption--;
            if (ch == 's' && selectedOption < 3)
                selectedOption++;
            if (ch == '\r')
                handleStartGameSelect();
        }
        else if (state == MENU_SETTINGS) {
            // �������ò˵���ѡ��
            if (ch == 'w' && selectedOption > 0) 
                selectedOption--;
            if (ch == 's' && selectedOption < 2) 
                selectedOption++;
            if (ch == '\r') 
                handleSettingsSelect();
        }
    }
}

MenuState Menu::GetState() const {
    return state;
}

void Menu::drawMainMenu(int frame) {
    cleardevice();

    // ���ر���ͼƬ
    IMAGE background;
    loadimage(&background, _T("rec/bg/Background1.png"), 1200, 600, 1); 
    putimage(0, 0, &background);
    
    // ������ɫ
    COLORREF color1 = RGB(0, 255, 255); 
    COLORREF color2 = RGB(255, 0, 255);   
    COLORREF color3 = RGB(155, 255, 0); 

    //��̬����ƫ����
    int x = 150, y = 100;
    int dx1 = int(3 * sin(frame * 0.13));
    int dy1 = int(2 * cos(frame * 0.11));
    int dx2 = int(4 * sin(frame * 0.08 + 1.5));
    int dy2 = int(3 * cos(frame * 0.14 + 2.1));
    int dx3 = int(2 * sin(frame * 0.18 + 2.8));
    int dy3 = int(4 * cos(frame * 0.09 + 0.8));

    // �ײ����
    setbkmode(TRANSPARENT);
    settextstyle(80, 54, _T("Arial Black"));
    settextcolor(color1);
    outtextxy(x + dx1, y + dy1, _T("Armored Warriors"));

    // �м�����
    settextstyle(76, 52, _T("Arial Black"));
    settextcolor(color2);
    outtextxy(x + dx2, y + dy2, _T("Armored Warriors"));

    // �������
    settextstyle(72, 50, _T("Arial Black"));
    settextcolor(color3);
    outtextxy(x + dx3, y + dy3, _T("Armored Warriors"));

    // ѡ��
    settextcolor(BLACK);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(450, 300, selectedOption == 0 ? _T(">> ��ʼ��Ϸ <<") : _T("     ��ʼ��Ϸ"));
    outtextxy(450, 350, selectedOption == 1 ? _T(">> ��Ϸ���� <<") : _T("     ��Ϸ����"));
    outtextxy(450, 400, selectedOption == 2 ? _T(">> �˳���Ϸ <<") : _T("     �˳���Ϸ"));
}

void Menu::drawStartMenu() {
    cleardevice();
    // ���ر���ͼƬ
    IMAGE background;
    loadimage(&background, _T("rec/bg/background2.png"), 1200, 600, 1);
    putimage(0, 0, &background);
    settextcolor(WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(450, 200, _T("��ѡ����Ϸģʽ"));
    outtextxy(450, 250, selectedOption == 0 ? _T(">> ˫�˶�ս <<") : _T("     ˫�˶�ս"));
    outtextxy(450, 300, selectedOption == 1 ? _T(">> �˻���ս <<") : _T("     �˻���ս"));
    outtextxy(450, 350, selectedOption == 2 ? _T(">> ��ϰģʽ <<") : _T("     ��ϰģʽ"));
    outtextxy(450, 400, selectedOption == 3 ? _T(">> �������˵� <<") : _T("     �������˵�"));
}

void Menu::drawSettingsMenu() {
    cleardevice();
    IMAGE background;
    loadimage(&background, _T("rec/bg/background3.png"), 1200, 600, 1);
    putimage(0, 0, &background);
    settextcolor(WHITE);
    settextstyle(45, 20, _T("Arial Black"));
    outtextxy(430, 120, _T("��ѡ����Ϸ����"));
    outtextxy(430, 170, selectedOption == 0 ? _T(">> ��Ч���� <<") : _T("     ��Ч����"));
    outtextxy(430, 220, selectedOption == 1 ? _T(">> ��ս���� <<") : _T("     ��ս����"));
    outtextxy(430, 270, selectedOption == 2 ? _T(">> �������˵� <<") : _T("     �������˵�"));
}

// �����͵����Ӧ
void Menu::handleMouse() {
    if (state == MENU_MAIN) {
        // �������˵�ѡ���y���귶Χ
        int opt_x1 = 450, opt_x2 = 750;
        int opt_y[3][2] = { {300,340}, {350,390}, {400,440} };

        // ��������¼�
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;

            // ����ƶ�������ѡ��
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            // �����������ֱ��ִ��ѡ��
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
        // ������Ϸѡ���y���귶Χ
        int opt_x1 = 450, opt_x2 = 750;
        int opt_y[4][2] = { {250,290}, {300,340}, {350,390},{400,440} };

        // ��������¼�
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;

            // ����ƶ�������ѡ��
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 4; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            // �����������ֱ��ִ��ѡ��
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
        // ��������ѡ���y���귶Χ
        int opt_x1 = 430, opt_x2 = 730;
        int opt_y[3][2] = { {170,210}, {220,260}, {270,310}};

        // ��������¼�
        while (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int mx = msg.x, my = msg.y;

            // ����ƶ�������ѡ��
            if (msg.uMsg == WM_MOUSEMOVE) {
                for (int i = 0; i < 3; ++i) {
                    if (mx >= opt_x1 && mx <= opt_x2 && my >= opt_y[i][0] && my <= opt_y[i][1]) {
                        selectedOption = i;
                    }
                }
            }
            // �����������ֱ��ִ��ѡ��
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
}

// �˵�ѡ���߼�
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
        // �˳���Ϸ
        closegraph();
        exit(0);
    }
}
// �˵�ѡ���߼�
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

    }
    else if (selectedOption == 1) {

    }
    else if (selectedOption == 2) {
        state = MENU_MAIN;
        selectedOption = 0; // ����ѡ��
    }
}
