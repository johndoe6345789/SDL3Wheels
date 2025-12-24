#include "game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    Game game;
    
    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }
    
    game.run();
    game.shutdown();
    
    return 0;
}
