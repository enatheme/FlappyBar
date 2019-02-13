#include "main.h"

#include <chrono>
#include <iostream>

int main()
{
    ImGui::CreateContext();
    sf::RenderWindow window(sf::VideoMode(1280, 800), "ImGui test");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
 
    std::srand(std::time(nullptr));
    auto timer_1 = std::chrono::high_resolution_clock::now();
    auto timer_2 = std::chrono::high_resolution_clock::now();
    sf::Clock deltaClock;

    Screen screen = Screen();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
 
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
 
        ImGui::SFML::Update(window, deltaClock.restart());
 
        screen.update();

        // each 0.1 sec, scroll to left
        auto stop_1 = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(stop_1 - timer_1).count() > 100)
        {
            timer_1 = stop_1;
            screen.scroll(Screen::SIDE::Left);
        }
        // each 5 secs, new object
        auto stop_2 = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(stop_2 - timer_2).count() > 5)
        {
            timer_2 = stop_2;
            uint8_t fx = std::rand() % 20;
            uint8_t tx = Screen::X - std::rand() % 20;
            //uint8_t fy = Screen::Y - std::rand() % 5;
            //uint8_t ty = Screen::Y - std::rand() % 5;
            uint8_t fy = Screen::Y - 2;
            uint8_t ty = Screen::Y - 1;
            screen.draw(fx, tx, fy, ty);
        }
        //screen.draw(std::experimental::randint(0, 10), std::experimental::randint(20, 30), std::experimental::randint(Screen::Y - 5, Screen::Y - 2), std::experimental::randint(Screen::Y - 1, Screen::Y));

        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}
