#include "Console.h"

// shorthand for converting keypress to char
#define KEYPRESS_CASE(_case, val) case _case : ret = val; break;

void Console::Start(){ 

    active = false;

    sf::Font* font = AssetManager::GetFont("Amber_Default");


    int spacing = 12;

    int y_pos = Core::GetDisplayHeight() - spacing * 2;

    interact_line.setOrigin(sf::Vector2f(0, 4));
    interact_line.setFont(*font);
    interact_line.setCharacterSize(8);
    interact_line.setPosition(spacing * 2, y_pos);
    interact_line.setString("/sumsum adadada herllo");
    interact_line.setFillColor(sf::Color::White);  

    interact_box.setFillColor(sf::Color(25, 28, 34, 100));
    interact_box.setPosition(interact_line.getPosition() + sf::Vector2f(-2, -4));
    interact_box.setSize(sf::Vector2f(Core::GetDisplayWidth() - interact_box.getPosition().x * 2, 8 + 2 + 2));

    y_pos -= spacing * 2;


    lines.resize(line_count);
    for(int i = 0; i < lines.size(); i++){
        lines[i].setPosition(spacing * 2, y_pos);
        lines[i].setFont(*font);
        lines[i].setCharacterSize(8);
        lines[i].setString("TESTING 123 adadada herllo @ apple");
        lines[i].setFillColor(sf::Color::White);
        y_pos -= spacing;
    }

}

void Console::CatchEvent(sf::Event event){
    if (event.type == sf::Event::KeyPressed)
    {
        // function
        switch(event.key.scancode){

            case sf::Keyboard::Scan::Slash:{
                active = true;
                break;
            }
            case sf::Keyboard::Scan::Escape:{
                active = false;
                break;
            }
        }

        if(!active){
            return;
        }

        // detecting keystrokes
        std::string ret = "";
        
        switch (event.key.scancode)
        {
            case sf::Keyboard::Scan::Backspace: {
                // removes on character from the end, using substring manipulation
                if(interact_line.getString().getSize() > 0){
                    interact_line.setString(interact_line.getString().substring(
                        0, interact_line.getString().getSize() - 1));
                }
                break;
            }


            KEYPRESS_CASE(sf::Keyboard::Scan::A, "A")
            KEYPRESS_CASE(sf::Keyboard::Scan::B, "B")
            KEYPRESS_CASE(sf::Keyboard::Scan::C, "C")
            KEYPRESS_CASE(sf::Keyboard::Scan::D, "D")
            KEYPRESS_CASE(sf::Keyboard::Scan::E, "E")
            KEYPRESS_CASE(sf::Keyboard::Scan::F, "F")
            KEYPRESS_CASE(sf::Keyboard::Scan::G, "G")
            KEYPRESS_CASE(sf::Keyboard::Scan::H, "H")
            KEYPRESS_CASE(sf::Keyboard::Scan::I, "I")
            KEYPRESS_CASE(sf::Keyboard::Scan::J, "J")
            KEYPRESS_CASE(sf::Keyboard::Scan::K, "K")
            KEYPRESS_CASE(sf::Keyboard::Scan::L, "L")
            KEYPRESS_CASE(sf::Keyboard::Scan::M, "M")
            KEYPRESS_CASE(sf::Keyboard::Scan::N, "N")
            KEYPRESS_CASE(sf::Keyboard::Scan::O, "O")
            KEYPRESS_CASE(sf::Keyboard::Scan::P, "P")
            KEYPRESS_CASE(sf::Keyboard::Scan::Q, "Q")
            KEYPRESS_CASE(sf::Keyboard::Scan::R, "R")
            KEYPRESS_CASE(sf::Keyboard::Scan::S, "S")
            KEYPRESS_CASE(sf::Keyboard::Scan::T, "T")
            KEYPRESS_CASE(sf::Keyboard::Scan::U, "U")
            KEYPRESS_CASE(sf::Keyboard::Scan::V, "V")
            KEYPRESS_CASE(sf::Keyboard::Scan::W, "W")
            KEYPRESS_CASE(sf::Keyboard::Scan::Y, "Y")
            KEYPRESS_CASE(sf::Keyboard::Scan::X, "X")
            KEYPRESS_CASE(sf::Keyboard::Scan::Z, "Z")

            KEYPRESS_CASE(sf::Keyboard::Scan::Num1, "1")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num2, "2")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num3, "3")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num4, "4")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num5, "5")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num6, "6")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num7, "7")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num8, "8")
            KEYPRESS_CASE(sf::Keyboard::Scan::Num9, "9")

            KEYPRESS_CASE(sf::Keyboard::Scan::Space, " ")
            KEYPRESS_CASE(sf::Keyboard::Scan::Slash, "/")
        }

        interact_line.setString(interact_line.getString() + ret);

    }
}


void Console::Draw(sf::RenderTarget& texture){

    if(active){

        texture.draw(interact_box);

        for(int i = 0; i < lines.size(); i++){
            texture.draw(lines[i]);
        }
        texture.draw(interact_line);
    }
}

