#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class Kart;
class Track;
class InputHandler;
class IRenderer;

enum class GameState {
    MENU,
    RACING,
    PAUSED,
    FINISHED
};

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void shutdown();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    void updateMenu();
    void updateRacing(float deltaTime);
    void updatePaused();
    void updateFinished();

    SDL_Window* window_;
    SDL_Renderer* sdlRenderer_;
    
    std::unique_ptr<IRenderer> renderer_;
    std::unique_ptr<Track> track_;
    std::unique_ptr<InputHandler> inputHandler_;
    std::vector<std::unique_ptr<Kart>> karts_;
    
    GameState state_;
    bool running_;
    int selectedMenuItem_;
    float raceTime_;
    int playerLap_;
    int totalLaps_;
};
