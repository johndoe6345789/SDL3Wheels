#include "game.h"
#include "kart.h"
#include "track.h"
#include "input_handler.h"
#include "renderer.h"
#include <iostream>

Game::Game() 
    : window_(nullptr)
    , sdlRenderer_(nullptr)
    , state_(GameState::MENU)
    , running_(false)
    , selectedMenuItem_(0)
    , raceTime_(0.0f)
    , playerLap_(0)
    , totalLaps_(3)
{
}

Game::~Game() {
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    window_ = SDL_CreateWindow(
        "SDL3 Wheels - Wacky Wheels Clone",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    
    if (!window_) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    sdlRenderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer_) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer_ = std::make_unique<Renderer>(sdlRenderer_);
    track_ = std::make_unique<Track>();
    inputHandler_ = std::make_unique<InputHandler>();
    
    // Create player kart
    karts_.push_back(std::make_unique<Kart>(400, 500, true));
    
    // Create AI karts
    karts_.push_back(std::make_unique<Kart>(350, 520, false));
    karts_.push_back(std::make_unique<Kart>(450, 520, false));
    karts_.push_back(std::make_unique<Kart>(300, 540, false));
    
    running_ = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    
    while (running_) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Cap delta time to prevent physics issues
        if (deltaTime > 0.05f) {
            deltaTime = 0.05f;
        }
        
        handleEvents();
        update(deltaTime);
        render();
        
        SDL_Delay(16); // ~60 FPS
    }
}

void Game::shutdown() {
    karts_.clear();
    inputHandler_.reset();
    track_.reset();
    renderer_.reset();
    
    if (sdlRenderer_) {
        SDL_DestroyRenderer(sdlRenderer_);
    }
    if (window_) {
        SDL_DestroyWindow(window_);
    }
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running_ = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            if (state_ == GameState::RACING) {
                state_ = GameState::PAUSED;
            } else if (state_ == GameState::PAUSED) {
                state_ = GameState::RACING;
            } else if (state_ == GameState::MENU || state_ == GameState::FINISHED) {
                running_ = false;
            }
        }
    }
    
    inputHandler_->update();
}

void Game::update(float deltaTime) {
    switch (state_) {
        case GameState::MENU:
            updateMenu();
            break;
        case GameState::RACING:
            updateRacing(deltaTime);
            break;
        case GameState::PAUSED:
            updatePaused();
            break;
        case GameState::FINISHED:
            updateFinished();
            break;
    }
}

void Game::updateMenu() {
    if (inputHandler_->wasKeyJustPressed(SDL_SCANCODE_UP)) {
        selectedMenuItem_ = (selectedMenuItem_ - 1 + 3) % 3;
    }
    if (inputHandler_->wasKeyJustPressed(SDL_SCANCODE_DOWN)) {
        selectedMenuItem_ = (selectedMenuItem_ + 1) % 3;
    }
    if (inputHandler_->isSelectPressed()) {
        if (selectedMenuItem_ == 0) {
            // Start Race
            state_ = GameState::RACING;
            raceTime_ = 0.0f;
            playerLap_ = 0;
            // Reset karts
            karts_[0] = std::make_unique<Kart>(400, 500, true);
            for (size_t i = 1; i < karts_.size(); i++) {
                karts_[i] = std::make_unique<Kart>(350 + i * 50, 500 + i * 20, false);
            }
        } else if (selectedMenuItem_ == 2) {
            // Quit
            running_ = false;
        }
    }
}

void Game::updateRacing(float deltaTime) {
    raceTime_ += deltaTime;
    
    // Update player kart
    Kart* playerKart = karts_[0].get();
    
    if (inputHandler_->isAccelerating()) {
        playerKart->accelerate(1.0f);
    }
    if (inputHandler_->isBraking()) {
        playerKart->brake();
    }
    if (inputHandler_->isTurningLeft()) {
        playerKart->turn(-1.0f);
    }
    if (inputHandler_->isTurningRight()) {
        playerKart->turn(1.0f);
    }
    
    // Update all karts
    for (auto& kart : karts_) {
        kart->update(deltaTime);
        
        // Simple AI for non-player karts
        if (!kart->isPlayer()) {
            kart->accelerate(0.7f);
            // Simple track following
            if (kart->getPosition().x < 300) {
                kart->turn(0.5f);
            } else if (kart->getPosition().x > 500) {
                kart->turn(-0.5f);
            }
        }
        
        // Check checkpoints
        Vector2 pos = kart->getPosition();
        int oldCheckpoint = kart->getCheckpoint();
        int newCheckpoint = track_->checkCheckpoint(pos.x, pos.y, oldCheckpoint);
        if (newCheckpoint != oldCheckpoint) {
            kart->setCheckpoint(newCheckpoint);
            // If we crossed the start/finish line (checkpoint 0) after completing a lap
            if (newCheckpoint == 0 && oldCheckpoint > 0) {
                kart->incrementLap();
            }
        }
    }
    
    playerLap_ = playerKart->getCurrentLap();
    
    // Check if race is finished
    if (playerLap_ >= totalLaps_) {
        state_ = GameState::FINISHED;
    }
}

void Game::updatePaused() {
    if (inputHandler_->isPausePressed()) {
        state_ = GameState::RACING;
    }
}

void Game::updateFinished() {
    if (inputHandler_->isSelectPressed()) {
        state_ = GameState::MENU;
        selectedMenuItem_ = 0;
    }
}

void Game::render() {
    renderer_->clear();
    
    switch (state_) {
        case GameState::MENU:
            renderer_->renderMenu(selectedMenuItem_);
            break;
            
        case GameState::RACING:
            renderer_->renderTrack(track_.get());
            for (const auto& kart : karts_) {
                renderer_->renderKart(kart.get());
            }
            renderer_->renderHUD(playerLap_, totalLaps_, raceTime_);
            break;
            
        case GameState::PAUSED:
            renderer_->renderTrack(track_.get());
            for (const auto& kart : karts_) {
                renderer_->renderKart(kart.get());
            }
            renderer_->renderPauseMenu();
            break;
            
        case GameState::FINISHED:
            renderer_->renderFinishScreen(raceTime_);
            break;
    }
    
    renderer_->present();
}
