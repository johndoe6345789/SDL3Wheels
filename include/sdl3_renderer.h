#pragma once

#include "i_renderer.h"
#include <SDL3/SDL.h>

class SDL3Renderer : public IRenderer {
public:
    explicit SDL3Renderer(SDL_Renderer* renderer);
    ~SDL3Renderer() override = default;
    
    // Core rendering functions
    void clear() override;
    void present() override;
    
    // High-level rendering functions
    void renderMenu(int selectedItem) override;
    void renderTrack(const Track* track) override;
    void renderKart(const Kart* kart) override;
    void renderHUD(int currentLap, int totalLaps, float raceTime) override;
    void renderPauseMenu() override;
    void renderFinishScreen(float finalTime) override;
    
protected:
    // Low-level drawing primitives
    void drawText(const std::string& text, int x, int y, int size) override;
    void drawRect(const Rect& rect, Color color) override;
    void fillRect(const Rect& rect, Color color) override;
    void drawLine(float x1, float y1, float x2, float y2, Color color) override;
    void setBlendMode(bool enabled) override;
    
private:
    SDL_Renderer* sdlRenderer_;
    
    // Helper to convert platform-independent types to SDL types
    SDL_FRect toSDLRect(const Rect& rect) const;
    SDL_Color toSDLColor(const Color& color) const;
};
