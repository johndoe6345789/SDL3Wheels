#include "input_handler.h"
#include <cstring>

InputHandler::InputHandler() 
    : currentKeyStates_(nullptr)
{
    std::memset(previousKeyStates_, 0, sizeof(previousKeyStates_));
    currentKeyStates_ = SDL_GetKeyboardState(nullptr);
}

void InputHandler::update() {
    // Store previous state
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        previousKeyStates_[i] = currentKeyStates_[i];
    }
}

bool InputHandler::isKeyPressed(SDL_Scancode key) const {
    return currentKeyStates_[key];
}

bool InputHandler::wasKeyJustPressed(SDL_Scancode key) const {
    return currentKeyStates_[key] && !previousKeyStates_[key];
}

bool InputHandler::isAccelerating() const {
    return isKeyPressed(SDL_SCANCODE_UP) || isKeyPressed(SDL_SCANCODE_W);
}

bool InputHandler::isBraking() const {
    return isKeyPressed(SDL_SCANCODE_DOWN) || isKeyPressed(SDL_SCANCODE_S);
}

bool InputHandler::isTurningLeft() const {
    return isKeyPressed(SDL_SCANCODE_LEFT) || isKeyPressed(SDL_SCANCODE_A);
}

bool InputHandler::isTurningRight() const {
    return isKeyPressed(SDL_SCANCODE_RIGHT) || isKeyPressed(SDL_SCANCODE_D);
}

bool InputHandler::isPausePressed() const {
    return wasKeyJustPressed(SDL_SCANCODE_P) || wasKeyJustPressed(SDL_SCANCODE_ESCAPE);
}

bool InputHandler::isSelectPressed() const {
    return wasKeyJustPressed(SDL_SCANCODE_RETURN) || wasKeyJustPressed(SDL_SCANCODE_SPACE);
}
