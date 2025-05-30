#include "GameState.h"
#include <graphics.h>
#include <string>

const int Animation_num = 11; // 有11个动画帧
int currentAnimation_num = 0; // 当前动画帧索引
// 构造函数，初始化为无模式、99秒、玩家血量满
GameState::GameState() : mode_(GameMode::NONE), timeLeft_(99), player1HP_(100), player2HP_(100) {}

// 设置当前的游戏模式
void GameState::setMode(GameMode mode) { mode_ = mode; }

// 获取当前的游戏模式
GameMode GameState::getMode() const { return mode_; }

// 进入游戏场景时初始化时间和血量等
void GameState::enter() {
    timeLeft_ = 99;        // 剩余时间重置为99秒
    player1HP_ = 100;      // 玩家1血量重置
    player2HP_ = 100;      // 玩家2血量重置
}

// 每帧调用，更新时间（假设每帧约0.016秒），可扩展为处理更多逻辑
void GameState::update() {
    if (mode_ != GameMode::NONE && timeLeft_ > 0)
        timeLeft_ -= 0.3; // 时间递减，最少到0
}

// 绘制游戏主场景：背景、血条、人物、时间和模式显示
void GameState::render() {
    cleardevice(); // 清屏

	// 动画帧切换
    static int frameCounter = 0;
    currentAnimation_num = (currentAnimation_num + 1) % Animation_num;
    // 背景
	IMAGE Animation[Animation_num];
	for (int i = 0; i < Animation_num; ++i) {
		std::string filename = "rec/Anim/Animation" + std::to_string(i + 1) + ".png";
		loadimage(&Animation[i], filename.c_str(), 1200, 600, true);
	}
    // 绘制当前动画帧
	putimage(0, 0, &Animation[currentAnimation_num]); 
   
    // 绘制两侧血条
    setfillcolor(RED);
    fillrectangle(50, 50, 50 + player1HP_ * 2, 80);      // 玩家1血条
    fillrectangle(350, 50, 350 + player2HP_ * 2, 80);    // 玩家2或AI血条

    // 绘制剩余时间
    settextcolor(WHITE);
    settextstyle(30, 0, "Arial Black");
    std::string timeStr = "Time: " + std::to_string(int(timeLeft_));
    outtextxy(550, 10, timeStr.c_str());

    // 绘制模式名称
    settextstyle(20, 0, "Arial Black");
    const char* modeStr = "";
    if (mode_ == GameMode::PVP) {
        modeStr = "Mode: PVP";
    }
    else if (mode_ == GameMode::PVE) {
        modeStr = "Mode: PVE";
    }
    else if (mode_ == GameMode::PRACTICE) {
        modeStr = "Mode: PRACTICE";
    }
    settextcolor(BLACK);
    outtextxy(30, 550, modeStr);

    
}