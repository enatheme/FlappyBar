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
#include <vector>

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
    struct Position
    {
        bool _is_valid;
        uint8_t _x;
        uint8_t _y;
    };

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

    // return true if no collision
    bool scroll(uint8_t speed, SIDE side, bool move_object);
    static constexpr uint8_t X = 40;
    static constexpr uint8_t Y = 180;

    uint8_t add_object(Position p, char symbol)
    {
        return do_add_object(p, symbol);
    }
    uint8_t add_object(Position p, char symbol, uint8_t speed, SIDE direction)
    {
        auto id = add_object(p, symbol);
        do_modify_speed_object(id, speed, direction);
        return id;
    }
private:
    class Object
    {
    public:
        Object (uint8_t id, char symbol, Screen::Position position)
            :
                m_id(id), m_symbol(symbol), m_position(position)
        {}

        uint8_t get_id() const;
        char get_symbol() const;
        Screen::Position get_position() const;
        void set_position(Screen::Position);
        // next position must be aware at the screen scrolling
        Screen::Position get_next_position(uint8_t scrolling, SIDE side);
        void set_speed(uint8_t speed, SIDE direction);

    private:
        Screen::Position do_invalid_position();

    private:
        uint8_t m_id = 0; // id 0 is invalid
        char m_symbol = ' ';
        Screen::Position m_position = {true, 0, 0};
        uint8_t m_speed = 0;
        SIDE m_direction = SIDE::Top;
    };
    void do_scroll_left();
    void do_scroll_right();
    void do_scroll_top();
    void do_scroll_bot();

    uint8_t do_add_object(Position p, char symbol);
    void do_modify_speed_object(uint8_t id, uint8_t speed, SIDE direction);
    void do_delete_object(uint8_t id);

    constexpr void flip(char & c)
    {
        c = ((c == '*') ? ' ' : '*');
    }
    constexpr void xor_flip(char & c)
    {
        c = ((c == ' ') ? ' ' : '*');
    }
    char m_screen[Screen::X][Screen::Y];

    private:
        uint8_t m_last_id = 0;
        std::vector<Object> m_objects;
};
