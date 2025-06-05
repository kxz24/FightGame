#include <graphics.h>
#include <conio.h>
#include <chrono>
#include "Menu.h"
#include "GameState.h"
#include <windows.h>

enum class AppState { MENU, GAME };

int main() {
    initgraph(1200, 600);

    BeginBatchDraw();
    Menu menu;
    GameState game;
    AppState appState = AppState::MENU;

    const int FPS = 60;
    const int FRAME_TIME = 1000 / FPS;

    auto prevTime = std::chrono::steady_clock::now();

    while (true) {
        auto frameStart = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration<double>(frameStart - prevTime).count();
        prevTime = frameStart;

        if (appState == AppState::MENU) {
            menu.Draw();
            menu.Update();
            GameMode selected = menu.GetAndClearLastGameModeSelected();
            if (selected != GameMode::NONE) {
                game.setMode(selected);
                game.enter();
                appState = AppState::GAME;
            }
        }
        else if (appState == AppState::GAME) {
            // 玩家1按键检测
            bool aDown = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000);
            bool dDown = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('d') & 0x8000);
            bool sDown = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('s') & 0x8000);
            bool wDown = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('w') & 0x8000);
            bool eDown = (GetAsyncKeyState('E') & 0x8000) || (GetAsyncKeyState('e') & 0x8000);

            // 玩家2按键检测
            bool hDown = (GetAsyncKeyState('H') & 0x8000) || (GetAsyncKeyState('h') & 0x8000);
            bool kDown = (GetAsyncKeyState('K') & 0x8000) || (GetAsyncKeyState('k') & 0x8000);
            bool jDown = (GetAsyncKeyState('J') & 0x8000) || (GetAsyncKeyState('j') & 0x8000);
            bool uDown = (GetAsyncKeyState('U') & 0x8000) || (GetAsyncKeyState('u') & 0x8000);
            bool iDown = (GetAsyncKeyState('I') & 0x8000) || (GetAsyncKeyState('i') & 0x8000);

            // 玩家1输入每帧都set，保证松开时能恢复
            game.player1.setKeyState('a', aDown);
            game.player1.setKeyState('d', dDown);
            if (wDown) game.player1.setKeyState('w', true);
            if (eDown) game.player1.setKeyState('e', true);
            if (sDown) game.player1.startDefend();

            // 玩家2输入只在PVP模式下响应
            if (game.getMode() == GameMode::PVP) {
                game.player2.setKeyState('h', hDown);
                game.player2.setKeyState('k', kDown);
                if (uDown) game.player2.setKeyState('u', true);
                if (iDown) game.player2.setKeyState('i', true);
                if (jDown) game.player2.startDefend();
            }

            game.update(deltaTime);
            game.render();

            if (_kbhit()) {
                int ch = _getch();
                if (ch == 27) { // Esc
                    appState = AppState::MENU;
                }
            }
        }
        auto frameEnd = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
        int delay = FRAME_TIME - elapsed;
        if (delay > 0) Sleep(delay);
        FlushBatchDraw();
    }
    EndBatchDraw();
    closegraph();
    return 0;
}