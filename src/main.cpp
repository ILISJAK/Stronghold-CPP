#include "StrongholdAPI.h"

int main() {
    try {
        StrongholdAPI api;
        api.start();

        // Keep the program running until terminated
        std::cout << "Press Enter to exit." << std::endl;
        std::cin.ignore();

        api.stop();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
