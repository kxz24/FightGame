#pragma once
#include <graphics.h>
#include <string>

enum class CharacterAction {
    IDLE,
    WALK,
    ATTACK,
    DEFEND,
    JUMP,
    HURT,
    DEAD
};

class Character {
public:
    Character(float x = 0, float y = 0, const std::string& name = "Hero");

    void setPosition(float x, float y);
    float getX() const;
    float getY() const;

    void setAction(CharacterAction action);

    void setSpeed(float vx, float vy);
    void addSpeed(float dx, float dy);
    float getVX() const;
    float getVY() const;

    void setKeyState(int key, bool pressed);

    void aiControl();
    void render();
    void setFacingRight(bool right);
    std::string getName() const;
    void update(float deltaTime);

    void setHP(int hp);
    int getHP() const;

    void startDefend();
    bool getIsDefending() const; // 提供给判定是否防御中

    bool getIsJumping() const { return isJumping; }
    void startJump();

    // 攻击相关
    void startAttack();
    bool getIsAttacking() const { return isAttacking; }

private:
    float x_, y_;
    float vx_, vy_;
    int hp_;
    int frameCount_;
    std::string name_;
    CharacterAction currentAction_;
    bool facingRight_;
    int animFrameCounter = 0;

    bool leftPressed, rightPressed, jumpPressed, attackPressed;

    void putimage_alpha(int x, int y, IMAGE* img);
    void loadImages();
    //行走
    IMAGE walkImagesLeft[8];
    IMAGE walkImagesRight[8];
    //站立
    IMAGE idleImagesLeft[7];
    IMAGE idleImagesRight[7];
    //防御
    bool isDefending = false;       // 是否处于防御动画期间
    int defendFrameIndex = 0;       // 当前防御帧（0~5）
    int defendAnimCounter = 0;      // 控制每两帧切换一次
    IMAGE defendImagesLeft[6];          // 防御动画帧
    IMAGE defendImagesRight[6];         // 防御动画帧
    //跳跃
    bool isJumping = false;
    int jumpFrameIndex = 0;
    const int jumpTotalFrames = 15;
    float jumpStartY = 0;
    float jumpVY = 0;
    const float gravity = 3.0f;
    IMAGE jumpImagesLeft[5];
    IMAGE jumpImagesRight[5];

    // 攻击
    bool isAttacking = false;
    int attackFrameIndex = 0;
    int attackAnimCounter = 0;
    const int attackTotalFrames = 5;
    IMAGE attackImagesLeft[5];
    IMAGE attackImagesRight[5];
};