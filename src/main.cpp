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
    auto id = screen.add_object({true, 5, 5}, 'P', 0, Screen::SIDE::Right);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
 
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                break;
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Up:
                            screen.scroll(1, Screen::SIDE::Top);
                        break;
                        case sf::Keyboard::Down:
                            screen.scroll(1, Screen::SIDE::Bot);
                        break;
                        default:
                        break;
                    }
                break;
                default:
                break;
            }
        }
 
        ImGui::SFML::Update(window, deltaClock.restart());
 
        screen.update();

        // each 0.1 sec, scroll to left
        auto stop_1 = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(stop_1 - timer_1).count() > 1000)
        {
            timer_1 = stop_1;
            screen.scroll(1, Screen::SIDE::Left);
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
            //screen.draw(fx, tx, fy, ty);
        }
        //screen.draw(std::experimental::randint(0, 10), std::experimental::randint(20, 30), std::experimental::randint(Screen::Y - 5, Screen::Y - 2), std::experimental::randint(Screen::Y - 1, Screen::Y));

        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}
