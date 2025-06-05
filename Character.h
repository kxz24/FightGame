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

enum class ControlType {
    PLAYER1,
    PLAYER2,
    AI
};

class Character {
public:
    Character(float x = 0, float y = 0, const std::string& name = "Hero", ControlType controlType = ControlType::PLAYER1);

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
    bool getIsDefending() const { return isDefending; }
    bool getIsJumping() const { return isJumping; }
    void startJump();
    void startAttack();
    bool getIsAttacking() const { return isAttacking; }

    // 控制类型相关
    void setControlType(ControlType type);
    ControlType getControlType() const;

    void resetInputState();

    // 受击相关
    void startHurt(int damage = 10);
    bool getIsHurting() const { return isHurting; }

    // 受击动画帧
    int getHurtFrameIndex() const { return hurtFrameIndex; }
    CharacterAction getCurrentAction() const { return currentAction_; }
    int getAttackFrameIndex() const { return attackFrameIndex; }
    bool isFacingRight() const { return facingRight_; }

private:
    float x_, y_;
    float vx_, vy_;
    int hp_;
    int frameCount_;
    std::string name_;
    CharacterAction currentAction_;
    bool facingRight_;
    int animFrameCounter = 0;

    bool leftPressed = false, rightPressed = false, jumpPressed = false, attackPressed = false;

    void putimage_alpha(int x, int y, IMAGE* img);
    void loadImages();

    //行走
    IMAGE walkImagesLeft[8];
    IMAGE walkImagesRight[8];
    //站立
    IMAGE idleImagesLeft[7];
    IMAGE idleImagesRight[7];
    //防御
    bool isDefending = false;
    int defendFrameIndex = 0;
    int defendAnimCounter = 0;
    IMAGE defendImagesLeft[6];
    IMAGE defendImagesRight[6];
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
    // 受击
    bool isHurting = false;
    int hurtFrameIndex = 0;
    int hurtAnimCounter = 0;
    const int hurtTotalFrames = 4;
    IMAGE hurtImagesLeft[4];
    IMAGE hurtImagesRight[4];

    // 控制类型
    ControlType controlType_;
};