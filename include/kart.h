#pragma once

#include <SDL2/SDL.h>

struct Vector2 {
    float x;
    float y;
    
    Vector2() : x(0), y(0) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    
    float length() const {
        return SDL_sqrtf(x * x + y * y);
    }
    
    Vector2 normalized() const {
        float len = length();
        if (len > 0) {
            return Vector2(x / len, y / len);
        }
        return Vector2(0, 0);
    }
};

class Kart {
public:
    Kart(float x, float y, bool isPlayer = false);
    
    void update(float deltaTime);
    void accelerate(float amount);
    void turn(float angle);
    void brake();
    
    Vector2 getPosition() const { return position_; }
    float getAngle() const { return angle_; }
    float getSpeed() const { return speed_; }
    bool isPlayer() const { return isPlayer_; }
    int getCurrentLap() const { return currentLap_; }
    int getCheckpoint() const { return checkpoint_; }
    
    void setCheckpoint(int cp) { checkpoint_ = cp; }
    void incrementLap() { currentLap_++; }
    
private:
    Vector2 position_;
    Vector2 velocity_;
    float angle_;
    float speed_;
    float acceleration_;
    float maxSpeed_;
    float turnSpeed_;
    float friction_;
    
    bool isPlayer_;
    int currentLap_;
    int checkpoint_;
};
