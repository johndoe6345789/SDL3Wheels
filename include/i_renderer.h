#pragma once

#include <string>
#include <cstdint>

class Kart;
class Track;

// Color structure (platform-independent)
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Rectangle structure (platform-independent)
struct Rect {
    float x;
    float y;
    float width;
    float height;
};

// Abstract renderer interface
class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    // Core rendering functions
    virtual void clear() = 0;
    virtual void present() = 0;
    
    // High-level rendering functions
    virtual void renderMenu(int selectedItem) = 0;
    virtual void renderTrack(const Track* track) = 0;
    virtual void renderKart(const Kart* kart) = 0;
    virtual void renderHUD(int currentLap, int totalLaps, float raceTime) = 0;
    virtual void renderPauseMenu() = 0;
    virtual void renderFinishScreen(float finalTime) = 0;
    
protected:
    // Low-level drawing primitives
    virtual void drawText(const std::string& text, int x, int y, int size) = 0;
    virtual void drawRect(const Rect& rect, Color color) = 0;
    virtual void fillRect(const Rect& rect, Color color) = 0;
    virtual void drawLine(float x1, float y1, float x2, float y2, Color color) = 0;
    virtual void setBlendMode(bool enabled) = 0;
};
