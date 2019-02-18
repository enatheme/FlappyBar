#include "Display.h"

void Screen::update()
{
    ImGui::Begin("Debug");
    static int number;
    static int column;
    static int line;
    ImGui::InputInt("number (0 to 16)", &number);
    ImGui::InputInt("column", &column);
    ImGui::InputInt("line", &line);
    if (ImGui::Button("Draw it"))
    {   
        draw(number, line, column);
    }   

    if (ImGui::Button("Move top"))
    {   
		do_scroll_top();
    }   
	ImGui::SameLine();
    if (ImGui::Button("Move bot"))
    {   
		do_scroll_bot();
    }   
    if (ImGui::Button("Move right"))
    {   
		do_scroll_right();
    }   
	ImGui::SameLine();
    if (ImGui::Button("Move left"))
    {   
		do_scroll_left();
    }   

    static int from_x;
    static int to_x;
    static int from_y;
    static int to_y;
    ImGui::InputInt("from x", &from_x);
    ImGui::InputInt("to x", &to_x);
    ImGui::InputInt("from y", &from_y);
    ImGui::InputInt("to y", &to_y);
    if (ImGui::Button("Draw me"))
    {   
        draw(from_x, to_x, from_y, to_y);
    }
    ImGui::End();

    ImGui::Begin(m_title);

    for (uint8_t i = 0 ; i < Screen::X ; ++i)
    {
        ImGui::TextUnformatted(m_screen[i], m_screen[i] + Screen::Y);
    }
    ImGui::End();
}

void Screen::clear()
{
    std::memset(m_screen, '*', Screen::X * Screen::Y);
}

void Screen::draw(uint8_t sprite, uint8_t x, uint8_t y)
{
    for (uint8_t iter_x = 0, end_x = std::min(uint8_t(x + 5), uint8_t(Screen::X - 1)) ; x + iter_x < end_x ; ++iter_x)
    {
        for (uint8_t iter_y = 0, end_y = std::min(uint8_t(y + 4), uint8_t(Screen::Y - 1)) ; y + iter_y < end_y ; ++iter_y)
        {
            if (Sprites()[sprite][iter_x][iter_y])
            {
                flip(m_screen[x + iter_x][y + iter_y]);
            }
            else
            {
                xor_flip(m_screen[x + iter_x][y + iter_y]);
            }
        }
    }
}

Screen::Screen(const Screen & s) : Display("Screen", true)
{
    std::memcpy(m_screen, s.m_screen, sizeof(char) * Screen::X * Screen::Y);
}

void Screen::operator=(const Screen & s)
{
    std::memcpy(m_screen, s.m_screen, sizeof(char) * Screen::X * Screen::Y);
}

void Screen::operator=(Screen && s)
{
    std::memcpy(m_screen, s.m_screen, sizeof(char) * Screen::X * Screen::Y);
}

void Screen::draw(uint8_t from_x, uint8_t to_x, uint8_t from_y, uint8_t to_y)
{
    for (uint8_t i = from_x ; i < std::min(Screen::X, to_x) ; ++i)
    {
        for (uint8_t j = from_y ; j < std::min(Screen::Y, to_y) ; ++j)
        {
            m_screen[i][j] = 'x';
        }
    }
}

void Screen::scroll(SIDE side)
{
    switch (side)
    {
        case SIDE::Top:
            do_scroll_top();
        break;
        case SIDE::Bot:
            do_scroll_bot();
        break;
        case SIDE::Right:
            do_scroll_right();
        break;
        case SIDE::Left:
            do_scroll_left();
        break;
    }
};

void Screen::do_scroll_top()
{
    for (uint8_t i = 1 ; i < Screen::X ; ++i)
    {
        for (uint8_t j = 0 ; j < Screen::Y ; ++j)
        {
            m_screen[i - 1][j] = m_screen[i][j];
        }
    }
    for (uint8_t j = 0 ; j < Screen::Y ; ++j)
    {
        m_screen[Screen::X][j] = ' ';
    }
}

void Screen::do_scroll_bot()
{
    for (uint8_t i = Screen::X ; i > 0 ; --i)
    {
        for (uint8_t j = 0 ; j < Screen::Y ; ++j)
        {
            m_screen[i][j] = m_screen[i - 1][j];
        }
    }
    for (uint8_t j = 0 ; j < Screen::Y ; ++j)
    {
        m_screen[0][j] = ' ';
    }
}

void Screen::do_scroll_left()
{
    for (uint8_t j = 1 ; j < Screen::Y ; ++j)
    {
        for (uint8_t i = 0 ; i < Screen::X ; ++i)
        {
            m_screen[i][j - 1] = m_screen[i][j];
        }
    }
    for (uint8_t i = 0 ; i < Screen::X ; ++i)
    {
        m_screen[i][Screen::Y] = ' ';
    }
}

void Screen::do_scroll_right()
{
    for (uint8_t j = Screen::Y ; j > 0 ; --j)
    {
        for (uint8_t i = 0 ; i < Screen::X ; ++i)
        {
            m_screen[i][j] = m_screen[i][j - 1];
        }
    }
    for (uint8_t i = 0 ; i < Screen::X ; ++i)
    {
        m_screen[0][Screen::Y] = ' ';
    }
}

uint8_t Screen::do_add_object(Position p, char symbol)
{
    m_last_id++;
    m_object.emplace_back({m_last_id, p, symbol});
    return m_last_id;
}

void Screen::do_modity_speed_object(uint8_t id, uint8_t speed, SIDE direction)
{
    m_vector.at[id].set_speed(speed, direction);
}

uint8_t Screen::Object::get_id() const
{
    return m_id;
}

char Screen::Object::get_symbol() const
{
    return m_symbol;
}

Screen::Position Screen::Object::get_position() const
{
    return m_position;
}

Screen::Position Screen::Object::get_next_position()
{
    if (!m_position._is_valid)
    {
        return m_position;
    }
    switch (m_direction)
    {
        case SIDE::Top:
            if ((m_position._y - m_speed) < 0)
            {
                return do_invalid_position();
            }
            m_position._y -= m_speed;
        break;
        case SIDE::Bot:
            if ((m_position._y + m_speed) > Screen::Y)
            {
                return do_invalid_position();
            }
            m_position._y += m_speed;
        break;
        case SIDE::Left:
            if ((m_position._x - m_speed) < 0)
            {
                return do_invalid_position();
            }
            m_position._x -= m_speed;
        break;
        case SIDE::Right:
            if ((m_position._x + m_speed) > Screen::X)
            {
                return do_invalid_position();
            }
            m_position._x += m_speed;
        break;
    }
    return m_position;
}

Screen::Position Screen::Object::do_invalid_position()
{
    m_position._is_valid = false;
    return m_position;
}
