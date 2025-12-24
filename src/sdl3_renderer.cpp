#include "sdl3_renderer.h"
#include "kart.h"
#include "track.h"
#include <sstream>
#include <iomanip>

SDL3Renderer::SDL3Renderer(SDL_Renderer* renderer)
    : sdlRenderer_(renderer)
{
}

void SDL3Renderer::clear() {
    SDL_SetRenderDrawColor(sdlRenderer_, 20, 100, 20, 255); // Dark green background
    SDL_RenderClear(sdlRenderer_);
}

void SDL3Renderer::present() {
    SDL_RenderPresent(sdlRenderer_);
}

void SDL3Renderer::renderMenu(int selectedItem) {
    // Draw title
    drawText("SDL3 WHEELS", 250, 100, 3);
    drawText("Wacky Wheels Clone", 280, 160, 1);
    
    // Draw menu items
    const char* items[] = {"Start Race", "Options", "Quit"};
    for (int i = 0; i < 3; i++) {
        if (i == selectedItem) {
            Rect highlight = {250, 250 + i * 60.0f, 300, 40};
            fillRect(highlight, {255, 255, 0, 100});
        }
        drawText(items[i], 280, 255 + i * 60, 2);
    }
    
    drawText("Controls: Arrow Keys or WASD", 200, 500, 1);
}

void SDL3Renderer::renderTrack(const Track* track) {
    // Draw track background
    SDL_SetRenderDrawColor(sdlRenderer_, 40, 140, 40, 255);
    SDL_RenderClear(sdlRenderer_);
    
    // Draw track segments (barriers)
    for (const auto& segment : track->getTrackSegments()) {
        Rect rect = {segment.x, segment.y, segment.w, segment.h};
        fillRect(rect, {100, 100, 100, 255});
    }
    
    // Draw checkpoints (semi-transparent)
    setBlendMode(true);
    for (const auto& cp : track->getCheckpoints()) {
        Rect rect = {cp.x, cp.y, cp.width, cp.height};
        fillRect(rect, {255, 255, 0, 80});
    }
    setBlendMode(false);
}

void SDL3Renderer::renderKart(const Kart* kart) {
    Vector2 pos = kart->getPosition();
    float angle = kart->getAngle();
    
    // Draw kart as a rectangle
    Rect kartRect = {pos.x - 15, pos.y - 10, 30, 20};
    
    if (kart->isPlayer()) {
        fillRect(kartRect, {255, 0, 0, 255}); // Red for player
    } else {
        fillRect(kartRect, {0, 0, 255, 255}); // Blue for AI
    }
    
    // Draw direction indicator
    float radians = angle * M_PI / 180.0f;
    float dirX = pos.x + SDL_cosf(radians) * 20;
    float dirY = pos.y + SDL_sinf(radians) * 20;
    
    drawLine(pos.x, pos.y, dirX, dirY, {255, 255, 255, 255});
}

void SDL3Renderer::renderHUD(int currentLap, int totalLaps, float raceTime) {
    // Draw lap counter
    std::ostringstream lapText;
    lapText << "Lap: " << currentLap << "/" << totalLaps;
    drawText(lapText.str(), 10, 10, 2);
    
    // Draw race time
    std::ostringstream timeText;
    timeText << "Time: " << std::fixed << std::setprecision(2) << raceTime << "s";
    drawText(timeText.str(), 10, 40, 2);
}

void SDL3Renderer::renderPauseMenu() {
    // Draw semi-transparent overlay
    setBlendMode(true);
    Rect overlay = {0, 0, 800, 600};
    fillRect(overlay, {0, 0, 0, 180});
    setBlendMode(false);
    
    drawText("PAUSED", 320, 250, 3);
    drawText("Press ESC to Resume", 260, 320, 2);
}

void SDL3Renderer::renderFinishScreen(float finalTime) {
    SDL_SetRenderDrawColor(sdlRenderer_, 20, 20, 20, 255);
    SDL_RenderClear(sdlRenderer_);
    
    drawText("RACE FINISHED!", 240, 200, 3);
    
    std::ostringstream timeText;
    timeText << "Final Time: " << std::fixed << std::setprecision(2) << finalTime << "s";
    drawText(timeText.str(), 260, 300, 2);
    
    drawText("Press Enter to Continue", 220, 400, 2);
}

void SDL3Renderer::drawText(const std::string& text, int x, int y, int size) {
    // Simple character-by-character rendering using rectangles
    // This is a placeholder - in a real game you'd use SDL_ttf or bitmap fonts
    int charWidth = 8 * size;
    int charHeight = 12 * size;
    
    for (size_t i = 0; i < text.length(); i++) {
        Rect charRect = {
            static_cast<float>(x + i * charWidth),
            static_cast<float>(y),
            static_cast<float>(charWidth - 2),
            static_cast<float>(charHeight)
        };
        
        fillRect(charRect, {255, 255, 255, 255});
    }
}

void SDL3Renderer::drawRect(const Rect& rect, Color color) {
    SDL_FRect sdlRect = toSDLRect(rect);
    SDL_SetRenderDrawColor(sdlRenderer_, color.r, color.g, color.b, color.a);
    SDL_RenderRect(sdlRenderer_, &sdlRect);
}

void SDL3Renderer::fillRect(const Rect& rect, Color color) {
    SDL_FRect sdlRect = toSDLRect(rect);
    SDL_SetRenderDrawColor(sdlRenderer_, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(sdlRenderer_, &sdlRect);
}

void SDL3Renderer::drawLine(float x1, float y1, float x2, float y2, Color color) {
    SDL_SetRenderDrawColor(sdlRenderer_, color.r, color.g, color.b, color.a);
    SDL_RenderLine(sdlRenderer_, x1, y1, x2, y2);
}

void SDL3Renderer::setBlendMode(bool enabled) {
    if (enabled) {
        SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_BLEND);
    } else {
        SDL_SetRenderDrawBlendMode(sdlRenderer_, SDL_BLENDMODE_NONE);
    }
}

SDL_FRect SDL3Renderer::toSDLRect(const Rect& rect) const {
    return SDL_FRect{rect.x, rect.y, rect.width, rect.height};
}

SDL_Color SDL3Renderer::toSDLColor(const Color& color) const {
    return SDL_Color{color.r, color.g, color.b, color.a};
}
