#include "renderer.h"
#include "kart.h"
#include "track.h"
#include <sstream>
#include <iomanip>

Renderer::Renderer(SDL_Renderer* renderer)
    : sdlRenderer_(renderer)
{
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(sdlRenderer_, 20, 100, 20, 255); // Dark green background
    SDL_RenderClear(sdlRenderer_);
}

void Renderer::present() {
    SDL_RenderPresent(sdlRenderer_);
}

void Renderer::renderMenu(int selectedItem) {
    // Draw title
    drawText("SDL3 WHEELS", 250, 100, 3);
    drawText("Wacky Wheels Clone", 280, 160, 1);
    
    // Draw menu items
    const char* items[] = {"Start Race", "Options", "Quit"};
    for (int i = 0; i < 3; i++) {
        if (i == selectedItem) {
            SDL_SetRenderDrawColor(sdlRenderer_, 255, 255, 0, 255);
            SDL_FRect highlight = {250, 250 + i * 60, 300, 40};
            fillRect(highlight, {255, 255, 0, 100});
        }
        drawText(items[i], 280, 255 + i * 60, 2);
    }
    
    drawText("Controls: Arrow Keys or WASD", 200, 500, 1);
}

void Renderer::renderTrack(const Track* track) {
    // Draw track background
    SDL_SetRenderDrawColor(sdlRenderer_, 40, 140, 40, 255);
    SDL_RenderClear(sdlRenderer_);
    
    // Draw track segments (barriers)
    for (const auto& segment : track->getTrackSegments()) {
        fillRect(segment, {100, 100, 100, 255});
    }
    
    // Draw checkpoints (semi-transparent)
    SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_BLEND);
    for (const auto& cp : track->getCheckpoints()) {
        SDL_FRect rect = {cp.x, cp.y, cp.width, cp.height};
        fillRect(rect, {255, 255, 0, 80});
    }
    SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_NONE);
}

void Renderer::renderKart(const Kart* kart) {
    Vector2 pos = kart->getPosition();
    float angle = kart->getAngle();
    
    // Draw kart as a rotated rectangle
    SDL_FRect kartRect = {pos.x - 15, pos.y - 10, 30, 20};
    
    if (kart->isPlayer()) {
        fillRect(kartRect, {255, 0, 0, 255}); // Red for player
    } else {
        fillRect(kartRect, {0, 0, 255, 255}); // Blue for AI
    }
    
    // Draw direction indicator
    float radians = angle * M_PI / 180.0f;
    float dirX = pos.x + SDL_cosf(radians) * 20;
    float dirY = pos.y + SDL_sinf(radians) * 20;
    
    SDL_SetRenderDrawColor(sdlRenderer_, 255, 255, 255, 255);
    SDL_RenderDrawLineF(sdlRenderer_, pos.x, pos.y, dirX, dirY);
}

void Renderer::renderHUD(int currentLap, int totalLaps, float raceTime) {
    // Draw lap counter
    std::ostringstream lapText;
    lapText << "Lap: " << currentLap << "/" << totalLaps;
    drawText(lapText.str(), 10, 10, 2);
    
    // Draw race time
    std::ostringstream timeText;
    timeText << "Time: " << std::fixed << std::setprecision(2) << raceTime << "s";
    drawText(timeText.str(), 10, 40, 2);
}

void Renderer::renderPauseMenu() {
    // Draw semi-transparent overlay
    SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_BLEND);
    SDL_FRect overlay = {0, 0, 800, 600};
    fillRect(overlay, {0, 0, 0, 180});
    SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_NONE);
    
    drawText("PAUSED", 320, 250, 3);
    drawText("Press ESC to Resume", 260, 320, 2);
}

void Renderer::renderFinishScreen(float finalTime) {
    SDL_SetRenderDrawColor(sdlRenderer_, 20, 20, 20, 255);
    SDL_RenderClear(sdlRenderer_);
    
    drawText("RACE FINISHED!", 240, 200, 3);
    
    std::ostringstream timeText;
    timeText << "Final Time: " << std::fixed << std::setprecision(2) << finalTime << "s";
    drawText(timeText.str(), 260, 300, 2);
    
    drawText("Press Enter to Continue", 220, 400, 2);
}

void Renderer::drawText(const std::string& text, int x, int y, int size) {
    // Simple character-by-character rendering using rectangles
    // This is a placeholder - in a real game you'd use SDL_ttf or bitmap fonts
    int charWidth = 8 * size;
    int charHeight = 12 * size;
    
    for (size_t i = 0; i < text.length(); i++) {
        SDL_FRect charRect = {
            static_cast<float>(x + i * charWidth),
            static_cast<float>(y),
            static_cast<float>(charWidth - 2),
            static_cast<float>(charHeight)
        };
        
        SDL_SetRenderDrawColor(sdlRenderer_, 255, 255, 255, 255);
        SDL_RenderFillRectF(sdlRenderer_, &charRect);
    }
}

void Renderer::drawRect(const SDL_FRect& rect, SDL_Color color) {
    SDL_SetRenderDrawColor(sdlRenderer_, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRectF(sdlRenderer_, &rect);
}

void Renderer::fillRect(const SDL_FRect& rect, SDL_Color color) {
    SDL_SetRenderDrawColor(sdlRenderer_, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(sdlRenderer_, &rect);
}
