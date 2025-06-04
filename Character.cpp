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

// ������Ϸ��ѭ���ⲿ�İ�����Ӧ
void Character::setKeyState(int key, bool pressed) {
    if (key == 'a' || key == 'A') leftPressed = pressed;
    if (key == 'd' || key == 'D') rightPressed = pressed;
    if ((key == 'w' || key == 'W') && pressed) {
        if (!isJumping && !isDefending) {
            startJump();
        }
    }

    // ���������Ծ���������ֹ���������л�
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
// AI�����߼�����ǰ������Ϊ��ֹ״̬
void Character::aiControl() {
    setAction(CharacterAction::IDLE);
}

//���ƽ�ɫ
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

// ���ý�ɫ����
void Character::setFacingRight(bool right) {
    facingRight_ = right;
}

// ��ȡ��ɫ����
std::string Character::getName() const { return name_; }

// ���½�ɫ״̬
void Character::update(float deltaTime) {
    animFrameCounter++;
    if (animFrameCounter >= 2) {
        frameCount_++;
        animFrameCounter = 0;
    }
    // �������֡��
    if (currentAction_ == CharacterAction::DEFEND && isDefending) {
        defendAnimCounter++;
        if (defendAnimCounter >= 2) { // ÿ2֡�л�һ��
            defendFrameIndex++;
            defendAnimCounter = 0;
        }
        if (defendFrameIndex >= 6) {
            // �����������꣬�˳�����״̬
            isDefending = false;
            setAction(CharacterAction::IDLE);
        }
    }
    // ������Ծ�߼�
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
        // ��ػ�֡������
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
    // ���½�ɫλ��
    //����ʵ�ڲ���д��ô���ƽ�ɫ��λ�õ��߼��ˣ����Ǻ�������򵥴ֱ��İ취��������Ч������
    if (x_ >= 100 && x_ <= 850)
        x_ += vx_ * deltaTime * 60;
    else if (x_ < 100)
        x_ = 101;
    else if (x_ > 850)
        x_ = 849;
}

void Character::setHP(int hp) { hp_ = hp; }
int Character::getHP() const { return hp_; }

// Ԥ���ؽ�ɫ������ͼ�񣬷�ֹ��������ʱ����
//ûԤ�������ʱֱ�Ӽ��س���Ῠ����PPT�������ٶȺ���
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

//��bվup��Voidmatrix�Ľ̳���ѧϰ��putimage_alpha������ʵ��
// �ú���ʹ����ͼ���͸������Ϣ���Ա�֤ͼ���͸�����ֲ��ᱻ����Ϊ��ɫ
void Character::putimage_alpha(int x, int y, IMAGE* img) {
    int h = img->getheight();
    int w = img->getwidth();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

//����
void Character::startDefend() {
    if (!isDefending) {
        setAction(CharacterAction::DEFEND);
        isDefending = true;
        defendFrameIndex = 0;
        defendAnimCounter = 0;
        vx_ = 0; // ����ʱ���ƶ�
    }
}
//��Ծ


void Character::startJump() {
    if (isJumping) return;
    isJumping = true;
    setAction(CharacterAction::JUMP);
    jumpFrameIndex = 0;
    jumpStartY = y_;
    jumpVY = -7 * gravity; 
}
