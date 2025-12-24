#include "kart.h"
#include <cmath>

Kart::Kart(float x, float y, bool isPlayer)
    : position_(x, y)
    , velocity_(0, 0)
    , angle_(0.0f)
    , speed_(0.0f)
    , acceleration_(0.0f)
    , maxSpeed_(300.0f)
    , turnSpeed_(3.0f)
    , friction_(0.95f)
    , isPlayer_(isPlayer)
    , currentLap_(0)
    , checkpoint_(0)
{
}

void Kart::update(float deltaTime) {
    // Update velocity based on angle and speed
    float radians = angle_ * M_PI / 180.0f;
    velocity_.x = SDL_cosf(radians) * speed_;
    velocity_.y = SDL_sinf(radians) * speed_;
    
    // Update position
    position_ = position_ + velocity_ * deltaTime;
    
    // Apply friction
    speed_ *= friction_;
    
    // Apply acceleration
    speed_ += acceleration_ * deltaTime;
    
    // Clamp speed
    if (speed_ > maxSpeed_) {
        speed_ = maxSpeed_;
    }
    if (speed_ < -maxSpeed_ * 0.5f) {
        speed_ = -maxSpeed_ * 0.5f;
    }
    
    // Reset acceleration
    acceleration_ = 0.0f;
    
    // Keep karts on screen (simple boundary)
    if (position_.x < 50) position_.x = 50;
    if (position_.x > 750) position_.x = 750;
    if (position_.y < 50) position_.y = 50;
    if (position_.y > 550) position_.y = 550;
}

void Kart::accelerate(float amount) {
    acceleration_ = 200.0f * amount;
}

void Kart::turn(float angle) {
    float speedFactor = SDL_fabsf(speed_) / maxSpeed_;
    if (speedFactor < 0.1f) speedFactor = 0.1f;
    angle_ += angle * turnSpeed_ * speedFactor;
}

void Kart::brake() {
    speed_ *= 0.85f;
    if (SDL_fabsf(speed_) < 1.0f) {
        speed_ = 0.0f;
    }
}
