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

    bool aDownLast = false;
    bool dDownLast = false;
    bool sDownLast = false;
    bool wDownLast = false;
    bool eDownLast = false;

    bool hDownLast = false; // Íæ¼Ò2×ó
    bool kDownLast = false; // Íæ¼Ò2ÓÒ
    bool jDownLast = false; // Íæ¼Ò2·ÀÓù
    bool uDownLast = false; // Íæ¼Ò2ÌøÔ¾
    bool iDownLast = false; // Íæ¼Ò2¹¥»÷

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
            // Íæ¼Ò1°´¼ü¼ì²â
            bool aDown = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000);
            bool dDown = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('d') & 0x8000);
            bool sDown = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('s') & 0x8000);
            bool wDown = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('w') & 0x8000);
            bool eDown = (GetAsyncKeyState('E') & 0x8000) || (GetAsyncKeyState('e') & 0x8000);

            // Íæ¼Ò2°´¼ü¼ì²â
            bool hDown = (GetAsyncKeyState('H') & 0x8000) || (GetAsyncKeyState('h') & 0x8000);
            bool kDown = (GetAsyncKeyState('K') & 0x8000) || (GetAsyncKeyState('k') & 0x8000);
            bool jDown = (GetAsyncKeyState('J') & 0x8000) || (GetAsyncKeyState('j') & 0x8000);
            bool uDown = (GetAsyncKeyState('U') & 0x8000) || (GetAsyncKeyState('u') & 0x8000);
            bool iDown = (GetAsyncKeyState('I') & 0x8000) || (GetAsyncKeyState('i') & 0x8000);

            // Íæ¼Ò1
            if (wDown && !wDownLast)
                game.player1.setKeyState('w', true);
            wDownLast = wDown;

            if (sDown && !sDownLast)
                game.player1.startDefend();
            sDownLast = sDown;

            if (aDown != aDownLast)
                game.player1.setKeyState('a', aDown);
            if (dDown != dDownLast)
                game.player1.setKeyState('d', dDown);
            if (eDown && !eDownLast)
                game.player1.setKeyState('e', true);

            aDownLast = aDown;
            dDownLast = dDown;
            eDownLast = eDown;

            // Íæ¼Ò2£¨ºÍÉÏÃæÒ»Ñù£©
            if (uDown && !uDownLast)
                game.player2.setKeyState('u', true); // ÌøÔ¾
            uDownLast = uDown;

            if (jDown && !jDownLast)
                game.player2.startDefend(); // ·ÀÓù
            jDownLast = jDown;

            if (hDown != hDownLast)
                game.player2.setKeyState('h', hDown); // ×ó
            if (kDown != kDownLast)
                game.player2.setKeyState('k', kDown); // ÓÒ
            if (iDown && !iDownLast)
                game.player2.setKeyState('i', true); // ¹¥»÷

            hDownLast = hDown;
            kDownLast = kDown;
            iDownLast = iDown;

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