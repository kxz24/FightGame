#include "GameState.h"
#include <graphics.h>
#include <string>

GameState::GameState()
    : mode_(GameMode::NONE), timeLeft_(99), player1HP_(100), player2HP_(100),
    player1(200, 250, "Player1"), player2(800, 250, "Player2"),
    totalGameTime_(99), currentAnimation_num(0) {
    loadResources();
}

void GameState::loadResources() {
    for (int i = 0; i < Animation_num; ++i) {
        std::string filename = "rec/Anim/Animation" + std::to_string(i + 1) + ".png";
        loadimage(&Animation[i], filename.c_str(), 1200, 600, true);
    }
}

void GameState::setMode(GameMode mode) { mode_ = mode; }
GameMode GameState::getMode() const { return mode_; }

void GameState::enter() {
    timeLeft_ = totalGameTime_;
    player1HP_ = 100;
    player2HP_ = 100;
    player1.setHP(100);
    player2.setHP(100);
    player1.setPosition(200, 250);
    player2.setPosition(800, 250);
    player1.setSpeed(0, 0);
    player2.setSpeed(0, 0);
    player1.setFacingRight(true);
    player2.setFacingRight(false);
    timer_.restart();
    currentAnimation_num = 0;
}

void GameState::update(double deltaTime) {
    if (mode_ != GameMode::NONE && timeLeft_ > 0) {
        timeLeft_ = totalGameTime_ - timer_.elapsed();
        if (timeLeft_ < 0) timeLeft_ = 0;
    }

    if (mode_ == GameMode::PVP) {
        // 控制已转移到主循环
    }
    else if (mode_ == GameMode::PVE || mode_ == GameMode::PRACTICE) {
        player2.aiControl();
    }

    player1.update((float)deltaTime);
    player2.update((float)deltaTime);

    bgAnimFrameCounter++;
    if (bgAnimFrameCounter >= 4) {
        currentAnimation_num = (currentAnimation_num + 1) % Animation_num;
        bgAnimFrameCounter = 0;
    }
}

void GameState::render() {
    cleardevice();
    putimage(0, 0, &Animation[currentAnimation_num]);

    player1.render();
    player2.render();

    setfillcolor(RED);
    fillrectangle(50, 50, 50 + player1.getHP() * 3, 80);
    fillrectangle(800, 50, 800 + player2.getHP() * 3, 80);

    settextcolor(WHITE);
    settextstyle(30, 0, "Arial Black");
    char timeStr[32];
    snprintf(timeStr, sizeof(timeStr), "Time: %.2f", timeLeft_);
    outtextxy(550, 10, timeStr);

    settextstyle(20, 0, "Arial Black");
    const char* modeStr = "";
    if (mode_ == GameMode::PVP) modeStr = "Mode: PVP";
    else if (mode_ == GameMode::PVE) modeStr = "Mode: PVE";
    else if (mode_ == GameMode::PRACTICE) modeStr = "Mode: PRACTICE";
    settextcolor(BLACK);
    outtextxy(30, 550, modeStr);
}

double GameState::getTimeLeft() const {
    return timeLeft_;
}