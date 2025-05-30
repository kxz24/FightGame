#include <graphics.h>
#include <conio.h>
#include "Menu.h"
#include "GameState.h"

enum class AppState { MENU, GAME };

int main() {
    initgraph(1200, 600);
    BeginBatchDraw();
    Menu menu;
    GameState game;
    AppState appState = AppState::MENU;

    while (true) {
        DWORD startTime = GetTickCount();
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
            game.update();
            game.render();
            // 检查是否返回菜单
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 27) { // Esc
                    appState = AppState::MENU;
                }
            }
        }
        DWORD elapsed = GetTickCount() - startTime;
        if (elapsed < 1000 / 165)
            Sleep(1000 / 165 - elapsed);
        FlushBatchDraw();
    }
    EndBatchDraw();
    closegraph();
    return 0;
}