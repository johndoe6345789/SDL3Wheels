#include "track.h"

Track::Track() {
    initializeTrack();
}

void Track::initializeTrack() {
    // Create a simple oval track with track segments
    // Outer boundaries
    trackSegments_.push_back({50, 50, 700, 20});     // Top
    trackSegments_.push_back({50, 530, 700, 20});    // Bottom
    trackSegments_.push_back({50, 50, 20, 500});     // Left
    trackSegments_.push_back({730, 50, 20, 500});    // Right
    
    // Inner boundaries (create the track)
    trackSegments_.push_back({150, 150, 500, 20});   // Top inner
    trackSegments_.push_back({150, 430, 500, 20});   // Bottom inner
    trackSegments_.push_back({150, 150, 20, 300});   // Left inner
    trackSegments_.push_back({630, 150, 20, 300});   // Right inner
    
    // Checkpoints for lap tracking
    checkpoints_.push_back({390, 50, 20, 100});      // Start/Finish
    checkpoints_.push_back({730, 290, 70, 20});      // Right side
    checkpoints_.push_back({390, 450, 20, 100});     // Bottom
    checkpoints_.push_back({50, 290, 70, 20});       // Left side
}

bool Track::isOnTrack(float x, float y) const {
    // Check if point is within track bounds
    if (x < 70 || x > 730 || y < 70 || y > 530) {
        return false;
    }
    
    // Check if point is not on inner track boundaries
    if (x > 170 && x < 630 && y > 170 && y < 430) {
        return false;
    }
    
    return true;
}

int Track::checkCheckpoint(float x, float y, int currentCheckpoint) const {
    for (size_t i = 0; i < checkpoints_.size(); i++) {
        const Checkpoint& cp = checkpoints_[i];
        if (x >= cp.x && x <= cp.x + cp.width &&
            y >= cp.y && y <= cp.y + cp.height) {
            // Player must go through checkpoints in order
            int expectedNext = (currentCheckpoint + 1) % checkpoints_.size();
            if (i == static_cast<size_t>(expectedNext)) {
                return i;
            }
        }
    }
    return currentCheckpoint;
}
