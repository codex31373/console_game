#include <iostream>
#include "Game.hpp"

int main() {
    try {
        Game game;
        if (!game.initialize()) {
            std::cerr << "Failed to initialize game!" << std::endl;
            return 1;
        }

        game.run();
        game.shutdown();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred!" << std::endl;
        return 1;
    }

    return 0;
}
