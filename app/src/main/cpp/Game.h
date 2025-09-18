#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include "Model.h"
#include "TextureAsset.h"

struct Platform {
    Vector3 position;
    Vector3 size;
    bool isMoving;
    Vector3 moveDirection;
    float moveSpeed;
    Vector3 originalPos;
    float moveRange;
};

struct Obstacle {
    Vector3 position;
    Vector3 size;
    bool isSpike;
    bool isMoving;
    Vector3 moveDirection;
    float moveSpeed;
};

struct Player {
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    bool onGround;
    bool alive;
    float jumpForce;
    float moveSpeed;
};

struct Level {
    std::vector<Platform> platforms;
    std::vector<Obstacle> obstacles;
    Vector3 startPosition;
    Vector3 endPosition;
    float gravity;
    int levelNumber;
};

class Game {
public:
    Game();
    ~Game();
    
    void update(float deltaTime);
    void handleInput(float touchX, float touchY, bool isPressed);
    void reset();
    void nextLevel();
    void restartLevel();
    
    const Player& getPlayer() const { return player_; }
    const Level& getCurrentLevel() const { return levels_[currentLevel_]; }
    int getCurrentLevelNumber() const { return currentLevel_ + 1; }
    bool isGameComplete() const { return currentLevel_ >= levels_.size(); }
    bool isPlayerDead() const { return !player_.alive; }
    
private:
    void initializeLevels();
    void createLevel(int levelNum, Level& level);
    void updatePlayer(float deltaTime);
    void checkCollisions();
    void updateMovingPlatforms(float deltaTime);
    void updateMovingObstacles(float deltaTime);
    bool checkPlatformCollision(const Vector3& pos, const Vector3& size, const Platform& platform);
    bool checkObstacleCollision(const Vector3& pos, const Vector3& size, const Obstacle& obstacle);
    
    Player player_;
    std::vector<Level> levels_;
    int currentLevel_;
    float gameTime_;
    bool leftPressed_;
    bool rightPressed_;
    bool jumpPressed_;
};

#endif // GAME_H