#include "engine.hpp"
#include <SFML/System/Thread.hpp>

int main()
{
    Engine engine;
    sf::Thread thread(&Engine::networkThreadFunc, &engine);

    engine.gameThreadFunc();

    return EXIT_SUCCESS;
}
