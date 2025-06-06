#include "Character.h"
#include <graphics.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "MSIMG32.LIB")

Character::Character(float x, float y, const std::string& name, ControlType controlType)
    : x_(x), y_(y), vx_(0), vy_(0), name_(name), hp_(100), frameCount_(0),
    currentAction_(CharacterAction::IDLE), facingRight_(true),
    leftPressed(false), rightPressed(false), jumpPressed(false), attackPressed(false),
    controlType_(controlType)
{
    loadImages();
}

void Character::setControlType(ControlType type) { controlType_ = type; }
ControlType Character::getControlType() const { return controlType_; }
void Character::resetInputState() {
    leftPressed = rightPressed = jumpPressed = attackPressed = false;
}

void Character::setPosition(float x, float y) { x_ = x; y_ = y; }
float Character::getX() const { return x_; }
float Character::getY() const { return y_; }

void Character::setSpeed(float vx, float vy) { vx_ = vx; vy_ = vy; }
void Character::addSpeed(float dx, float dy) { vx_ += dx; vy_ += dy; }
float Character::getVX() const { return vx_; }
float Character::getVY() const { return vy_; }

void Character::setAction(CharacterAction action) {
    if (currentAction_ != action) {
        currentAction_ = action;
        frameCount_ = 0;
    }
}

void Character::setKeyState(int key, bool pressed) {
    if (isHurting || isDead) return; // 死亡或受击期间不能任何操作
    if (controlType_ == ControlType::PLAYER1) {
        if (key == 'a' || key == 'A') leftPressed = pressed;
        if (key == 'd' || key == 'D') rightPressed = pressed;
        if ((key == 'w' || key == 'W') && pressed) {
            if (!isJumping && !isDefending && !isAttacking) startJump();
        }
        if ((key == 'e' || key == 'E') && pressed) {
            if (!isAttacking && !isJumping && !isDefending) startAttack();
        }
    }
    else if (controlType_ == ControlType::PLAYER2) {
        if (key == 'h' || key == 'H') leftPressed = pressed;
        if (key == 'k' || key == 'K') rightPressed = pressed;
        if ((key == 'u' || key == 'U') && pressed) {
            if (!isJumping && !isDefending && !isAttacking) startJump();
        }
        if ((key == 'i' || key == 'I') && pressed) {
            if (!isAttacking && !isJumping && !isDefending) startAttack();
        }
    }
    if (isJumping || isDefending || isAttacking) return;
    if (leftPressed && !rightPressed) {
        vx_ = -8;
        setFacingRight(false);
        setAction(CharacterAction::WALK);
    }
    else if (!leftPressed && rightPressed) {
        vx_ = 8;
        setFacingRight(true);
        setAction(CharacterAction::WALK);
    }
    else {
        vx_ = 0;
        setAction(CharacterAction::IDLE);
    }
}

void Character::aiControl() {
    setAction(CharacterAction::IDLE);
    resetInputState();
}

void Character::render() {
    IMAGE* img = nullptr;
    int offsetX = 0;
    int normalWidth = 225;
    int attackWidth = 275;

    if (currentAction_ == CharacterAction::HURT && isHurting) {
        int frameIdx = hurtFrameIndex;
        if (facingRight_)
            img = &hurtImagesRight[frameIdx];
        else
            img = &hurtImagesLeft[frameIdx];
    }
    else if (currentAction_ == CharacterAction::DEFEND && isDefending) {
        int frameIdx = defendFrameIndex;
        if (facingRight_) {
            if (frameIdx >= 0 && frameIdx < 6)
                img = &defendImagesRight[frameIdx];
            else
                img = &idleImagesRight[0];
        }
        else {
            if (frameIdx >= 0 && frameIdx < 6)
                img = &defendImagesLeft[frameIdx];
            else
                img = &idleImagesLeft[0];
        }
    }
    else if (currentAction_ == CharacterAction::ATTACK && isAttacking) {
        int frameIdx = attackFrameIndex;
        if (facingRight_)
            img = &attackImagesRight[frameIdx];
        else
            img = &attackImagesLeft[frameIdx];
        offsetX = -(attackWidth - normalWidth) / 2;
    }
    else if (currentAction_ == CharacterAction::WALK) {
        int frameIdx = frameCount_ % 8;
        if (facingRight_)
            img = &walkImagesRight[frameIdx];
        else
            img = &walkImagesLeft[frameIdx];
    }
    else if (currentAction_ == CharacterAction::IDLE) {
        int frameIdx = frameCount_ % 7;
        if (facingRight_)
            img = &idleImagesRight[frameIdx];
        else
            img = &idleImagesLeft[frameIdx];
    }
    else if (currentAction_ == CharacterAction::JUMP && isJumping) {
        int imgIdx = 0;
        if (jumpFrameIndex < 3)        imgIdx = 0;
        else if (jumpFrameIndex < 6)   imgIdx = 1;
        else if (jumpFrameIndex < 9)   imgIdx = 2;
        else if (jumpFrameIndex < 12)  imgIdx = 3;
        else                           imgIdx = 4;
        if (facingRight_)
            img = &jumpImagesRight[imgIdx];
        else
            img = &jumpImagesLeft[imgIdx];
    }
    else if (currentAction_ == CharacterAction::DEAD && isDead) {
        int frameIdx = deadFrameIndex;
        if (frameIdx >= deadTotalFrames) frameIdx = deadTotalFrames - 1;
        if (facingRight_)
            img = &deadImagesRight[frameIdx];
        else
            img = &deadImagesLeft[frameIdx];
    }

    if (img)
        putimage_alpha(static_cast<int>(x_) + offsetX, static_cast<int>(y_), img);

    settextcolor(WHITE);
    settextstyle(18, 0, "Arial");
    outtextxy(static_cast<int>(x_) + 80, static_cast<int>(y_) - 30, name_.c_str());
}

void Character::setFacingRight(bool right) { facingRight_ = right; }
std::string Character::getName() const { return name_; }

void Character::update(float deltaTime) {
    animFrameCounter++;
    if (animFrameCounter >= 2) {
        frameCount_++;
        animFrameCounter = 0;
    }

    // 受击动画
    if (currentAction_ == CharacterAction::HURT && isHurting) {
        hurtAnimCounter++;
        if (hurtAnimCounter >= 3) {
            hurtFrameIndex++;
            hurtAnimCounter = 0;
        }
        if (hurtFrameIndex >= hurtTotalFrames) {
            isHurting = false;
            setAction(CharacterAction::IDLE);
        }
        return;
    }

    // 攻击动画
    if (currentAction_ == CharacterAction::ATTACK && isAttacking) {
        attackAnimCounter++;
        if (attackAnimCounter >= 2) {
            attackFrameIndex++;
            attackAnimCounter = 0;
        }
        if (attackFrameIndex >= attackTotalFrames) {
            isAttacking = false;
            if (leftPressed && !rightPressed) {
                vx_ = -8;
                setFacingRight(false);
                setAction(CharacterAction::WALK);
            }
            else if (!leftPressed && rightPressed) {
                vx_ = 8;
                setFacingRight(true);
                setAction(CharacterAction::WALK);
            }
            else {
                vx_ = 0;
                setAction(CharacterAction::IDLE);
            }
        }
        return;
    }

    // 防御动画
    if (currentAction_ == CharacterAction::DEFEND && isDefending) {
        defendAnimCounter++;
        if (defendAnimCounter >= 2) {
            defendFrameIndex++;
            defendAnimCounter = 0;
        }
        if (defendFrameIndex >= 6) {
            isDefending = false;
            setAction(CharacterAction::IDLE);
        }
        return;
    }

    // 跳跃动画
    if (currentAction_ == CharacterAction::JUMP && isJumping) {
        jumpFrameIndex++;
        y_ += jumpVY;
        jumpVY += gravity;
        if (x_ >= 100 && x_ <= 850)
            x_ += vx_ * deltaTime * 60;
        else if (x_ < 100)
            x_ = 101;
        else if (x_ > 850)
            x_ = 849;
        if (jumpFrameIndex >= jumpTotalFrames || y_ >= jumpStartY) {
            isJumping = false;
            if (leftPressed && !rightPressed) {
                vx_ = -8;
                setFacingRight(false);
                setAction(CharacterAction::WALK);
            }
            else if (!leftPressed && rightPressed) {
                vx_ = 8;
                setFacingRight(true);
                setAction(CharacterAction::WALK);
            }
            else {
                vx_ = 0;
                setAction(CharacterAction::IDLE);
            }
        }
        return;
    }
    if (currentAction_ == CharacterAction::DEAD && isDead) {
        deadAnimCounter++;
        if (deadAnimCounter >= 3) {
            deadFrameIndex++;
            deadAnimCounter = 0;
        }
        if (deadFrameIndex >= deadTotalFrames) {
            deadFrameIndex = deadTotalFrames - 1; // 停在最后一帧
        }
        return;
    }
    // 普通移动
    if (x_ >= 100 && x_ <= 850)
        x_ += vx_ * deltaTime * 60;
    else if (x_ < 100)
        x_ = 101;
    else if (x_ > 850)
        x_ = 849;
}

void Character::setHP(int hp) {
    hp_ = hp;
    if (hp_ <= 0 && !isDead) {
        startDead();
    }
}
int Character::getHP() const { return hp_; }


void Character::loadImages() {
    for (int i = 0; i < 8; ++i) {
        std::string filename = "rec/walk_left/walk" + std::to_string(i + 1) + ".png";
        loadimage(&walkImagesLeft[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 8; ++i) {
        std::string filename = "rec/walk_right/walk" + std::to_string(i + 1) + ".png";
        loadimage(&walkImagesRight[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 7; ++i) {
        std::string filename = "rec/idle_left/idle" + std::to_string(i + 1) + ".png";
        loadimage(&idleImagesLeft[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 7; ++i) {
        std::string filename = "rec/idle_right/idle" + std::to_string(i + 1) + ".png";
        loadimage(&idleImagesRight[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 6; ++i) {
        std::string filename = "rec/defend_left/defend" + std::to_string(i + 1) + ".png";
        loadimage(&defendImagesLeft[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 6; ++i) {
        std::string filename = "rec/defend_right/defend" + std::to_string(i + 1) + ".png";
        loadimage(&defendImagesRight[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 5; ++i) {
        std::string filename = "rec/jump_left/jump" + std::to_string(i + 1) + ".png";
        loadimage(&jumpImagesLeft[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 5; ++i) {
        std::string filename = "rec/jump_right/jump" + std::to_string(i + 1) + ".png";
        loadimage(&jumpImagesRight[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 5; ++i) {
        std::string filename = "rec/attack_left/attack" + std::to_string(i + 1) + ".png";
        loadimage(&attackImagesLeft[i], filename.c_str(), 275, 200, true);
    }
    for (int i = 0; i < 5; ++i) {
        std::string filename = "rec/attack_right/attack" + std::to_string(i + 1) + ".png";
        loadimage(&attackImagesRight[i], filename.c_str(), 275, 200, true);
    }
    for (int i = 0; i < 4; ++i) {
        std::string filename = "rec/hurt_left/hurt" + std::to_string(i + 1) + ".png";
        loadimage(&hurtImagesLeft[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 4; ++i) {
        std::string filename = "rec/hurt_right/hurt" + std::to_string(i + 1) + ".png";
        loadimage(&hurtImagesRight[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 12; ++i) {
        std::string filename = "rec/death_left/death" + std::to_string(i + 1) + ".png";
        loadimage(&deadImagesLeft[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 12; ++i) {
        std::string filename = "rec/death_right/death" + std::to_string(i + 1) + ".png";
        loadimage(&deadImagesRight[i], filename.c_str(), 225, 200, true);
    }
}

void Character::putimage_alpha(int x, int y, IMAGE* img) {
    int h = img->getheight();
    int w = img->getwidth();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void Character::startDefend() {
    // 跳跃中不能进入防御
    if (isJumping) return;
    if (isDefending || isAttacking || isHurting) return;
    setAction(CharacterAction::DEFEND);
    isDefending = true;
    defendFrameIndex = 0;
    defendAnimCounter = 0;
    vx_ = 0;
}

void Character::startAttack() {
    if (isAttacking || isHurting) return;
    isAttacking = true;
    setAction(CharacterAction::ATTACK);
    attackFrameIndex = 0;
    attackAnimCounter = 0;
    vx_ = 0;
}

void Character::startJump() {
    if (isJumping || isAttacking || isHurting) return;
    isJumping = true;
    setAction(CharacterAction::JUMP);
    jumpFrameIndex = 0;
    jumpStartY = y_;
    jumpVY = -7 * gravity;
}

void Character::startHurt(int damage) {
    if (isHurting) return;
    isHurting = true;
    hurtFrameIndex = 0;
    hurtAnimCounter = 0;
    setAction(CharacterAction::HURT);
    setSpeed(0, 0);
    setHP(getHP() - damage);
}

void Character::startDead() {
    if (isDead) return;
    isDead = true;
    deadFrameIndex = 0;
    deadAnimCounter = 0;
    setAction(CharacterAction::DEAD);
    setSpeed(0, 0);
}