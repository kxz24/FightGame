#include "GameState.h"
#include <graphics.h>
#include <string>

GameState::GameState()
    : mode_(GameMode::NONE), timeLeft_(99), player1HP_(100), player2HP_(100),
    player1(200, 250, "Player1", ControlType::PLAYER1), player2(800, 250, "Player2", ControlType::PLAYER2),
    totalGameTime_(99), currentAnimation_num(0), gameOver_(false), winner_(0) {
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

    player1.resetState();
    player2.resetState();

    player1.setFacingRight(true);
    player2.setFacingRight(false);

    player1.setControlType(ControlType::PLAYER1);
    if (mode_ == GameMode::PVP)
        player2.setControlType(ControlType::PLAYER2);
    else
        player2.setControlType(ControlType::AI);

    player1.resetInputState();
    player2.resetInputState();

    timer_.restart();
    currentAnimation_num = 0;

    gameOver_ = false;
    winner_ = 0;
}

void GameState::update(double deltaTime) {
    if (mode_ != GameMode::NONE && timeLeft_ > 0) {
        timeLeft_ = totalGameTime_ - timer_.elapsed();
        if (timeLeft_ < 0) timeLeft_ = 0;
    }

    // 结算条件判断
    if (!gameOver_) {
        if (player1.getIsDead() || player1.getHP() <= 0) {
            gameOver_ = true;
            winner_ = 2;
        }
        else if (player2.getIsDead() || player2.getHP() <= 0) {
            gameOver_ = true;
            winner_ = 1;
        }
        else if (timeLeft_ <= 0) {
            gameOver_ = true;
            if (player1.getHP() > player2.getHP()) winner_ = 1;
            else if (player2.getHP() > player1.getHP()) winner_ = 2;
            else winner_ = 0; // 平局
        }
    }

    // 攻击判定和AI逻辑
    if (player1.getIsAttacking() && player1.getCurrentAction() == CharacterAction::ATTACK && player1.getAttackFrameIndex() == 2) {
        int atkRange = 275 + 40;
        float x1 = player1.getX();
        float x2 = player2.getX();
        bool inRange = false;
        if (player1.isFacingRight() && x2 > x1 && x2 - x1 <= atkRange) inRange = true;
        if (!player1.isFacingRight() && x1 > x2 && x1 - x2 <= atkRange) inRange = true;
        if (inRange && !player2.getIsDefending() && !player2.getIsHurting()) {
            player2.startHurt(10);
        }
    }
    if (player2.getIsAttacking() && player2.getCurrentAction() == CharacterAction::ATTACK && player2.getAttackFrameIndex() == 2) {
        int atkRange = 275 + 40;
        float x1 = player2.getX();
        float x2 = player1.getX();
        bool inRange = false;
        if (player2.isFacingRight() && x2 > x1 && x2 - x1 <= atkRange) inRange = true;
        if (!player2.isFacingRight() && x1 > x2 && x1 - x2 <= atkRange) inRange = true;
        if (inRange && !player1.getIsDefending() && !player1.getIsHurting()) {
            player1.startHurt(10);
        }
    }

    if (mode_ == GameMode::PVP) {
    }
    else if (mode_ == GameMode::PVE || mode_ == GameMode::PRACTICE) {
        player2.aiControl(player1);
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

bool GameState::isGameOver() const {
    return gameOver_;
}
int GameState::getWinner() const {
    return winner_;
}