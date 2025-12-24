#pragma once

#include <SDL3/SDL.h>
#include <vector>

struct Checkpoint {
    float x;
    float y;
    float width;
    float height;
};

class Track {
public:
    Track();
    
    const std::vector<SDL_FRect>& getTrackSegments() const { return trackSegments_; }
    const std::vector<Checkpoint>& getCheckpoints() const { return checkpoints_; }
    
    bool isOnTrack(float x, float y) const;
    int checkCheckpoint(float x, float y, int currentCheckpoint) const;
    
private:
    void initializeTrack();
    
    std::vector<SDL_FRect> trackSegments_;
    std::vector<Checkpoint> checkpoints_;
};
