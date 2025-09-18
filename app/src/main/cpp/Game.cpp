#include "Game.h"
#include "AndroidOut.h"
#include <cmath>
#include <algorithm>

Game::Game() : currentLevel_(0), gameTime_(0.0f), leftPressed_(false), rightPressed_(false), jumpPressed_(false) {
    // Initialize player
    player_.position = {0.0f, 0.0f, 0.0f};
    player_.velocity = {0.0f, 0.0f, 0.0f};
    player_.size = {0.2f, 0.2f, 0.1f};
    player_.onGround = false;
    player_.alive = true;
    player_.jumpForce = 8.0f;
    player_.moveSpeed = 4.0f;
    
    initializeLevels();
    
    // Set player to start position of first level
    if (!levels_.empty()) {
        player_.position = levels_[0].startPosition;
    }
}

Game::~Game() {
}

void Game::initializeLevels() {
    levels_.resize(20);
    for (int i = 0; i < 20; i++) {
        createLevel(i + 1, levels_[i]);
    }
}

void Game::createLevel(int levelNum, Level& level) {
    level.levelNumber = levelNum;
    level.gravity = -15.0f - (levelNum * 0.5f); // Gravity increases with level
    level.platforms.clear();
    level.obstacles.clear();
    
    // Start and end positions
    level.startPosition = {-8.0f, -1.5f, 0.0f};
    level.endPosition = {8.0f, 2.0f, 0.0f};
    
    // Create platforms based on level difficulty
    switch (levelNum) {
        case 1: // Very easy - straight path
            level.platforms.push_back({{-8.0f, -2.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-4.0f, -1.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{0.0f, 0.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{4.0f, 1.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{8.0f, 2.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            break;
            
        case 2: // Easy with small gaps
            level.platforms.push_back({{-8.0f, -2.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-5.0f, -1.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-2.0f, 0.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{1.0f, 1.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{4.0f, 1.5f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{7.0f, 2.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            break;
            
        case 3: // First obstacles
            level.platforms.push_back({{-8.0f, -2.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-5.0f, -1.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-1.0f, 0.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{3.0f, 1.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{7.0f, 2.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            // Add spikes
            level.obstacles.push_back({{-4.0f, -0.8f, 0.0f}, {0.3f, 0.3f, 0.3f}, true, false, {0,0,0}, 0});
            level.obstacles.push_back({{0.0f, 0.2f, 0.0f}, {0.3f, 0.3f, 0.3f}, true, false, {0,0,0}, 0});
            break;
            
        case 4: // Moving platforms introduction
            level.platforms.push_back({{-8.0f, -2.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-5.0f, -1.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, true, {0,1,0}, 1.0f, {-5.0f, -1.0f, 0.0f}, 2.0f});
            level.platforms.push_back({{-1.0f, 1.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{2.0f, 0.5f, 0.0f}, {1.5f, 0.2f, 0.5f}, true, {1,0,0}, 1.5f, {2.0f, 0.5f, 0.0f}, 2.5f});
            level.platforms.push_back({{7.0f, 2.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.obstacles.push_back({{0.0f, 1.2f, 0.0f}, {0.3f, 0.3f, 0.3f}, true, false, {0,0,0}, 0});
            break;
            
        case 5: // More complex jumps
            level.platforms.push_back({{-8.0f, -2.0f, 0.0f}, {1.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-6.0f, -0.5f, 0.0f}, {1.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-3.5f, 0.5f, 0.0f}, {1.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{-1.0f, -0.5f, 0.0f}, {1.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{1.5f, 1.0f, 0.0f}, {1.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{4.0f, 0.0f, 0.0f}, {1.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.platforms.push_back({{7.0f, 2.0f, 0.0f}, {1.5f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
            level.obstacles.push_back({{-2.5f, 0.7f, 0.0f}, {0.3f, 0.3f, 0.3f}, true, false, {0,0,0}, 0});
            level.obstacles.push_back({{2.5f, 1.2f, 0.0f}, {0.3f, 0.3f, 0.3f}, true, false, {0,0,0}, 0});
            break;
            
        default: // Levels 6-20: Progressively harder
            {
                float difficulty = (levelNum - 5) / 15.0f; // 0 to 1
                int numPlatforms = 5 + (levelNum / 3);
                int numObstacles = levelNum / 2;
                
                // Generate platforms with increasing gaps and complexity
                float x = -8.0f;
                float y = -2.0f;
                
                for (int i = 0; i < numPlatforms; i++) {
                    float platformWidth = 1.5f - (difficulty * 0.5f); // Smaller platforms
                    float gapSize = 2.0f + (difficulty * 2.0f); // Larger gaps
                    
                    bool isMoving = (levelNum > 8) && (i % 3 == 1);
                    Vector3 moveDir = {0, 0, 0};
                    float moveSpeed = 0;
                    float moveRange = 0;
                    
                    if (isMoving) {
                        moveDir = (i % 2 == 0) ? Vector3{1, 0, 0} : Vector3{0, 1, 0};
                        moveSpeed = 1.0f + (difficulty * 1.5f);
                        moveRange = 1.5f + (difficulty * 1.0f);
                    }
                    
                    level.platforms.push_back({{x, y, 0.0f}, {platformWidth, 0.2f, 0.5f}, 
                                             isMoving, moveDir, moveSpeed, {x, y, 0.0f}, moveRange});
                    
                    x += gapSize + platformWidth;
                    y += (sin(i * 0.5f) * 1.5f * difficulty); // Varying heights
                    
                    if (x > 6.0f) break;
                }
                
                // Add final platform
                level.platforms.push_back({{8.0f, 2.0f, 0.0f}, {2.0f, 0.2f, 0.5f}, false, {0,0,0}, 0, {0,0,0}, 0});
                
                // Add obstacles
                for (int i = 0; i < numObstacles && i < 8; i++) {
                    float obstacleX = -6.0f + (i * 2.0f);
                    float obstacleY = -1.0f + (sin(i) * 2.0f);
                    
                    bool isMovingObstacle = (levelNum > 12) && (i % 2 == 0);
                    Vector3 moveDir = {0, 0, 0};
                    float moveSpeed = 0;
                    
                    if (isMovingObstacle) {
                        moveDir = {1, 0, 0};
                        moveSpeed = 2.0f + (difficulty * 2.0f);
                    }
                    
                    level.obstacles.push_back({{obstacleX, obstacleY, 0.0f}, {0.3f, 0.3f, 0.3f}, 
                                             true, isMovingObstacle, moveDir, moveSpeed});
                }
            }
            break;
    }
}

void Game::update(float deltaTime) {
    if (!player_.alive) return;
    
    gameTime_ += deltaTime;
    
    updatePlayer(deltaTime);
    updateMovingPlatforms(deltaTime);
    updateMovingObstacles(deltaTime);
    checkCollisions();
    
    // Check if player reached the end
    Vector3 endPos = levels_[currentLevel_].endPosition;
    float distToEnd = sqrt(pow(player_.position.x - endPos.x, 2) + pow(player_.position.y - endPos.y, 2));
    if (distToEnd < 1.0f) {
        nextLevel();
    }
    
    // Check if player fell off the world
    if (player_.position.y < -5.0f) {
        player_.alive = false;
    }
}

void Game::updatePlayer(float deltaTime) {
    // Apply gravity
    player_.velocity.y += levels_[currentLevel_].gravity * deltaTime;
    
    // Handle input
    if (leftPressed_) {
        player_.velocity.x = -player_.moveSpeed;
    } else if (rightPressed_) {
        player_.velocity.x = player_.moveSpeed;
    } else {
        player_.velocity.x *= 0.8f; // Friction
    }
    
    if (jumpPressed_ && player_.onGround) {
        player_.velocity.y = player_.jumpForce;
        player_.onGround = false;
        jumpPressed_ = false;
    }
    
    // Update position
    player_.position.x += player_.velocity.x * deltaTime;
    player_.position.y += player_.velocity.y * deltaTime;
    
    // Limit horizontal movement
    player_.position.x = std::max(-10.0f, std::min(10.0f, player_.position.x));
}

void Game::updateMovingPlatforms(float deltaTime) {
    for (auto& platform : levels_[currentLevel_].platforms) {
        if (!platform.isMoving) continue;
        
        platform.position.x += platform.moveDirection.x * platform.moveSpeed * deltaTime;
        platform.position.y += platform.moveDirection.y * platform.moveSpeed * deltaTime;
        
        // Check bounds and reverse direction
        float distFromOriginal = sqrt(pow(platform.position.x - platform.originalPos.x, 2) + 
                                    pow(platform.position.y - platform.originalPos.y, 2));
        
        if (distFromOriginal > platform.moveRange) {
            platform.moveDirection.x *= -1;
            platform.moveDirection.y *= -1;
        }
    }
}

void Game::updateMovingObstacles(float deltaTime) {
    for (auto& obstacle : levels_[currentLevel_].obstacles) {
        if (!obstacle.isMoving) continue;
        
        obstacle.position.x += obstacle.moveDirection.x * obstacle.moveSpeed * deltaTime;
        
        // Simple bounds checking for obstacles
        if (obstacle.position.x > 9.0f || obstacle.position.x < -9.0f) {
            obstacle.moveDirection.x *= -1;
        }
    }
}

void Game::checkCollisions() {
    player_.onGround = false;
    
    // Check platform collisions
    for (const auto& platform : levels_[currentLevel_].platforms) {
        if (checkPlatformCollision(player_.position, player_.size, platform)) {
            // Landing on top of platform
            if (player_.velocity.y <= 0 && 
                player_.position.y > platform.position.y + platform.size.y/2) {
                player_.position.y = platform.position.y + platform.size.y/2 + player_.size.y/2;
                player_.velocity.y = 0;
                player_.onGround = true;
            }
        }
    }
    
    // Check obstacle collisions
    for (const auto& obstacle : levels_[currentLevel_].obstacles) {
        if (checkObstacleCollision(player_.position, player_.size, obstacle)) {
            player_.alive = false;
            break;
        }
    }
}

bool Game::checkPlatformCollision(const Vector3& pos, const Vector3& size, const Platform& platform) {
    return (pos.x - size.x/2 < platform.position.x + platform.size.x/2 &&
            pos.x + size.x/2 > platform.position.x - platform.size.x/2 &&
            pos.y - size.y/2 < platform.position.y + platform.size.y/2 &&
            pos.y + size.y/2 > platform.position.y - platform.size.y/2);
}

bool Game::checkObstacleCollision(const Vector3& pos, const Vector3& size, const Obstacle& obstacle) {
    return (pos.x - size.x/2 < obstacle.position.x + obstacle.size.x/2 &&
            pos.x + size.x/2 > obstacle.position.x - obstacle.size.x/2 &&
            pos.y - size.y/2 < obstacle.position.y + obstacle.size.y/2 &&
            pos.y + size.y/2 > obstacle.position.y - obstacle.size.y/2);
}

void Game::handleInput(float touchX, float touchY, bool isPressed) {
    if (!isPressed) {
        leftPressed_ = false;
        rightPressed_ = false;
        jumpPressed_ = false;
        return;
    }
    
    // Simple touch controls: left side = left, right side = right, top = jump
    if (touchX < 0.0f) {
        leftPressed_ = true;
    } else if (touchX > 0.0f) {
        rightPressed_ = true;
    }
    
    if (touchY > 0.0f) {
        jumpPressed_ = true;
    }
}

void Game::reset() {
    currentLevel_ = 0;
    player_.position = levels_[0].startPosition;
    player_.velocity = {0, 0, 0};
    player_.alive = true;
    player_.onGround = false;
    gameTime_ = 0.0f;
}

void Game::nextLevel() {
    currentLevel_++;
    if (currentLevel_ < levels_.size()) {
        player_.position = levels_[currentLevel_].startPosition;
        player_.velocity = {0, 0, 0};
        player_.onGround = false;
    }
}

void Game::restartLevel() {
    player_.position = levels_[currentLevel_].startPosition;
    player_.velocity = {0, 0, 0};
    player_.alive = true;
    player_.onGround = false;
}