#include "Character.h"
#include <graphics.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "MSIMG32.LIB")

Character::Character(float x, float y, const std::string& name)
    : x_(x), y_(y), vx_(0), vy_(0), name_(name), hp_(100), frameCount_(0),
    currentAction_(CharacterAction::IDLE), facingRight_(true),
    leftPressed(false), rightPressed(false), jumpPressed(false), attackPressed(false)
{
    loadImages();
}

void Character::setPosition(float x, float y) {
    x_ = x;
    y_ = y;
}
float Character::getX() const { return x_; }
float Character::getY() const { return y_; }

void Character::setSpeed(float vx, float vy) {
    vx_ = vx;
    vy_ = vy;
}
void Character::addSpeed(float dx, float dy) {
    vx_ += dx;
    vy_ += dy;
}
float Character::getVX() const { return vx_; }
float Character::getVY() const { return vy_; }

void Character::setAction(CharacterAction action) {
    if (currentAction_ != action) {
        currentAction_ = action;
        frameCount_ = 0;
    }
}

// 处理游戏主循环外部的按键响应
void Character::setKeyState(int key, bool pressed) {
    if (key == 'a' || key == 'A') leftPressed = pressed;
    if (key == 'd' || key == 'D') rightPressed = pressed;
    if ((key == 'w' || key == 'W') && pressed) {
        if (!isJumping && !isDefending) {
            startJump();
        }
    }

    // 如果正在跳跃或防御，禁止其他动作切换
    if (isJumping || isDefending) return;

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
// AI控制逻辑，当前仅设置为静止状态
void Character::aiControl() {
    setAction(CharacterAction::IDLE);
}

//绘制角色
void Character::render() {
    IMAGE* img = nullptr;
    if (currentAction_ == CharacterAction::DEFEND && isDefending) {
        int frameIdx = defendFrameIndex;
        if (frameIdx >= 0 && frameIdx < 6)
            img = &defendImagesLeft[frameIdx];
        else
            img = &idleImagesLeft[0];
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
    putimage_alpha(static_cast<int>(x_), static_cast<int>(y_), img);
    settextcolor(WHITE);
    settextstyle(18, 0, "Arial");
    outtextxy(static_cast<int>(x_)+80, static_cast<int>(y_) - 30, name_.c_str());
}

// 设置角色朝向
void Character::setFacingRight(bool right) {
    facingRight_ = right;
}

// 获取角色名称
std::string Character::getName() const { return name_; }

// 更新角色状态
void Character::update(float deltaTime) {
    animFrameCounter++;
    if (animFrameCounter >= 2) {
        frameCount_++;
        animFrameCounter = 0;
    }
    // 计算防御帧数
    if (currentAction_ == CharacterAction::DEFEND && isDefending) {
        defendAnimCounter++;
        if (defendAnimCounter >= 2) { // 每2帧切换一次
            defendFrameIndex++;
            defendAnimCounter = 0;
        }
        if (defendFrameIndex >= 6) {
            // 防御动画播完，退出防御状态
            isDefending = false;
            setAction(CharacterAction::IDLE);
        }
    }
    // 处理跳跃逻辑
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
        // 落地或帧数到达
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
    // 更新角色位置
    //这里实在不想写怎么限制角色的位置的逻辑了，于是乎用了最简单粗暴的办法，看起来效果不错
    if (x_ >= 100 && x_ <= 850)
        x_ += vx_ * deltaTime * 60;
    else if (x_ < 100)
        x_ = 101;
    else if (x_ > 850)
        x_ = 849;
}

void Character::setHP(int hp) { hp_ = hp; }
int Character::getHP() const { return hp_; }

// 预加载角色的所有图像，防止程序运行时卡顿
//没预处理绘制时直接加载程序会卡的像PPT，加载速度很慢
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
        std::string filename = "rec/defend/defend" + std::to_string(i + 1) + ".png";
        loadimage(&defendImagesLeft[i], filename.c_str(), 225, 200, true);
    }
    for (int i = 0; i < 6; ++i) {
        std::string filename = "rec/defend/defend" + std::to_string(i + 1) + ".png";
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
}

//从b站up主Voidmatrix的教程中学习了putimage_alpha函数的实现
// 该函数使用了图像的透明度信息，以保证图像的透明部分不会被覆盖为黑色
void Character::putimage_alpha(int x, int y, IMAGE* img) {
    int h = img->getheight();
    int w = img->getwidth();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

//防御
void Character::startDefend() {
    if (!isDefending) {
        setAction(CharacterAction::DEFEND);
        isDefending = true;
        defendFrameIndex = 0;
        defendAnimCounter = 0;
        vx_ = 0; // 防御时不移动
    }
}
//跳跃


void Character::startJump() {
    if (isJumping) return;
    isJumping = true;
    setAction(CharacterAction::JUMP);
    jumpFrameIndex = 0;
    jumpStartY = y_;
    jumpVY = -7 * gravity; 
}
