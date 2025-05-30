#ifndef GAMESTATE_H
#define GAMESTATE_H

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
    void update();
    void render();
private:
    GameMode mode_;
    float timeLeft_;
    int player1HP_, player2HP_;
};

#endif
