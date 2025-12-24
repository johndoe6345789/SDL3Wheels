#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Kart;
class Track;

class Renderer {
public:
    Renderer(SDL_Renderer* renderer);
    
    void clear();
    void present();
    
    void renderMenu(int selectedItem);
    void renderTrack(const Track* track);
    void renderKart(const Kart* kart);
    void renderHUD(int currentLap, int totalLaps, float raceTime);
    void renderPauseMenu();
    void renderFinishScreen(float finalTime);
    
private:
    void drawText(const std::string& text, int x, int y, int size);
    void drawRect(const SDL_FRect& rect, SDL_Color color);
    void fillRect(const SDL_FRect& rect, SDL_Color color);
    
    SDL_Renderer* sdlRenderer_;
};
