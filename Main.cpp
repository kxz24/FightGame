#include <graphics.h>
#include<iostream>ds
#include "Menu.h"


int main() {
    initgraph(1200, 600);
    Menu menu;
	BeginBatchDraw();
    while (1) {
		DWORD startTime = GetTickCount();
		menu.Draw();
        menu.Update();
		DWORD elapsedTime = GetTickCount() - startTime;
        if (elapsedTime < 1000 / 144)
			Sleep(1000 / 144 - elapsedTime); // 动态休眠以保证144 FPS
		FlushBatchDraw();
    }
	EndBatchDraw();
    closegraph();
	// 退出前清理
    return 0;
}