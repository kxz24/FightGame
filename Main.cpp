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

    bool aDownLast = false, dDownLast = false;
    bool sDownLast = false;

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
            // °´¼ü¼ì²â
            bool aDown = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000);
            bool dDown = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('d') & 0x8000);
            bool sDown = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('s') & 0x8000);
            if (sDown && !sDownLast) {
                game.player1.startDefend();
            }
            sDownLast = sDown;
            if (aDown != aDownLast)
                game.player1.setKeyState('a', aDown);
            if (dDown != dDownLast)
                game.player1.setKeyState('d', dDown);

            aDownLast = aDown;
            dDownLast = dDown;

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