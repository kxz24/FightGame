#pragma once
#include "Character.h"
#include "Timer.h"

enum class GameMode {
    NONE,
    PVP,
    PVE,
    PRACTICE
};

class GameState {
public:
    GameState();
    void setMode(GameMode mode);
    GameMode getMode() const;
    void enter();
    void update(double deltaTime);
    void render();
    double getTimeLeft() const;

    // 公开成员便于主循环访问
    Character player1;
    Character player2;

private:
    GameMode mode_;
    float timeLeft_;
    int player1HP_, player2HP_;

    Timer timer_;
    double totalGameTime_ = 99;

    static const int Animation_num = 11;
    IMAGE Animation[Animation_num];
    int currentAnimation_num = 0;
    int bgAnimFrameCounter = 0;
    void loadResources();
};