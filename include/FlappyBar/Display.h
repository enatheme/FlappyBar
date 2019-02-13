#pragma once

#include "imgui/lib/imgui.h"
#include "imgui/lib/imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>

struct Sprites
{
    using Sprite = bool[5][4];
    constexpr void generate(std::array<uint8_t, 5> input, Sprite & sprite)
    {
        for (uint8_t line = 0 ; line < 5 ; ++line)
        {
            sprite[line][0] = input[line] & 0x8;
            sprite[line][1] = input[line] & 0x4;
            sprite[line][2] = input[line] & 0x2;
            sprite[line][3] = input[line] & 0x1;
        }
    }
    constexpr Sprites() : values()
    {
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0x9, 0x9, 0xF}, values[0x0]);
        generate(std::array<uint8_t, 5>{0x2, 0x6, 0x2, 0x2, 0x7}, values[0x1]);
        generate(std::array<uint8_t, 5>{0xF, 0x1, 0xF, 0x8, 0xF}, values[0x2]);
        generate(std::array<uint8_t, 5>{0xF, 0x1, 0xF, 0x1, 0xF}, values[0x3]);
        generate(std::array<uint8_t, 5>{0x9, 0x9, 0xF, 0x1, 0x1}, values[0x4]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x1, 0xF}, values[0x5]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x9, 0xF}, values[0x6]);
        generate(std::array<uint8_t, 5>{0xF, 0x1, 0x2, 0x4, 0x4}, values[0x7]);
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0xF, 0x9, 0xF}, values[0x8]);
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0xF, 0x1, 0xF}, values[0x9]);
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0xF, 0x9, 0x9}, values[0xA]);
        generate(std::array<uint8_t, 5>{0xE, 0x9, 0xE, 0x9, 0xE}, values[0xB]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0x8, 0x8, 0xF}, values[0xC]);
        generate(std::array<uint8_t, 5>{0xE, 0x9, 0x9, 0x9, 0xE}, values[0xD]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x8, 0xF}, values[0xE]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x8, 0x8}, values[0xF]);
    }
    std::array<Sprite, 17> values;
    constexpr const Sprite & operator[](uint8_t i) const
    {
        return values[i];
    }
};


class Display
{
public:
    Display(const char * title, bool is_visible) : m_title(title), m_is_visible(is_visible) {}
    virtual ~Display() = default;
    virtual void update() = 0;
    virtual void clear() = 0;

protected:
    const char * m_title;
    bool m_is_visible;
};

class Screen: public Display
{
public:
    enum class SIDE { Top, Bot, Right, Left };
    Screen() : Display("Screen", true)
    {
        std::memset(m_screen, ' ', Screen::X * Screen::Y);
    }
    Screen(const Screen & s);

    void update() override final;

    void clear() override final;

    void draw(uint8_t sprite, uint8_t x, uint8_t y);
    void draw(uint8_t from_x, uint8_t from_y, uint8_t to_x, uint8_t to_y);

    void operator=(const Screen & s);
    void operator=(Screen && s);

    void scroll(SIDE side);
    static constexpr uint8_t X = 40;
    static constexpr uint8_t Y = 180;
private:
    void do_scroll_left();
    void do_scroll_right();
    void do_scroll_top();
    void do_scroll_bot();

    constexpr void flip(char & c)
    {
        c = ((c == '*') ? ' ' : '*');
    }
    constexpr void xor_flip(char & c)
    {
        c = ((c == ' ') ? ' ' : '*');
    }
    char m_screen[Screen::X][Screen::Y];
};
