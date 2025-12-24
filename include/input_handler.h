#pragma once

#include <SDL3/SDL.h>

class InputHandler {
public:
    InputHandler();
    
    void update();
    
    bool isKeyPressed(SDL_Scancode key) const;
    bool wasKeyJustPressed(SDL_Scancode key) const;
    
    bool isAccelerating() const;
    bool isBraking() const;
    bool isTurningLeft() const;
    bool isTurningRight() const;
    bool isPausePressed() const;
    bool isSelectPressed() const;
    
private:
    const bool* currentKeyStates_;
    bool previousKeyStates_[SDL_NUM_SCANCODES];
};
