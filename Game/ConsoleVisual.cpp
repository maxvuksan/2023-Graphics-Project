#include "ConsoleVisual.h"
#include "Networking/GameClient.h"

ConsoleVisual* ConsoleVisual::instance = nullptr;

// shorthand for converting keypress to char
#define KEYPRESS_CASE(_case, val) case _case : ret = val; break;

ConsoleVisual::ConsoleVisual(){
    
    if(instance != nullptr){
        std::cout << "ERROR : Creating multiple instances of ConsoleVisual is not allowed\n";
    }

    instance = this;
}
ConsoleVisual::~ConsoleVisual(){
    instance = nullptr;
}

void ConsoleVisual::LinkClient(GameClient* client){
    this->client = client;
}


void ConsoleVisual::Start(){ 

    
    this->SetRenderAtWindowSize(true);

    is_on = false;

    sf::Font* font = AssetManager::GetFont("Amber_Default");

    show_fps = false;
    fps_text.setFont(*font);
    fps_text.setCharacterSize(8);
    fps_text.setPosition(sf::Vector2f(Core::GetWindowWidth() - 100, 8));
    
    int spacing = 12;

    int y_pos = Core::GetWindowHeight() - spacing * 4;

    interact_line.setOrigin(sf::Vector2f(0, 4));
    interact_line.setFont(*font);
    interact_line.setCharacterSize(8);
    interact_line.setPosition(spacing * 4, y_pos);
    interact_line.setString("");
    interact_line.setFillColor(sf::Color::White);  
    interact_background_colour = sf::Color(25, 28, 34, 130);

    interact_box.setFillColor(interact_background_colour);
    interact_box.setPosition(interact_line.getPosition() + sf::Vector2f(-4, -6));
    interact_box.setSize(sf::Vector2f(Core::GetWindowWidth() - interact_box.getPosition().x * 2 + 2, 8 + 5));

    y_pos -= spacing * 2;


    lines.resize(line_count);
    for(int i = 0; i < lines.size(); i++){
        lines[i].setPosition(spacing * 4, y_pos);
        lines[i].setFont(*font);
        lines[i].setCharacterSize(8);
        lines[i].setString("");
        lines[i].setFillColor(sf::Color::White);
        y_pos -= spacing;
    }

}


void ConsoleVisual::CatchEvent(sf::Event event){
    if (event.type == sf::Event::KeyPressed)
    {
        // function
        switch(event.key.scancode){

            case sf::Keyboard::Scan::Slash:{
                is_on = true;

                break;
            }
        }

        if(!is_on){
            return;
        }

        // detecting keystrokes
        std::string ret = "";
        
        switch (event.key.scancode)
        {
            case sf::Keyboard::Scan::Escape:{
                interact_line.setString("");
                is_on = false;

                break;
            }

            // submit input
            case sf::Keyboard::Scan::Enter: {
                
                ConsoleLine result = CommandParser::Execute(interact_line.getString());
                if(result.message != ""){
                    bool send_packet = false;
                    if(result.colour == sf::Color::White){
                        send_packet = true;
                    }

                    this->Print(result, send_packet);
                    
                }
                interact_line.setString("");
                is_on = false;


                break;
            }

            case sf::Keyboard::Scan::Backspace: {
                // removes on character from the end, using substring manipulation
                if(interact_line.getString().getSize() > 0){
                    interact_line.setString(interact_line.getString().substring(
                        0, interact_line.getString().getSize() - 1));
                }
                break;
            }
        }

        if(interact_line.getString().getSize() >= 50){
            return;
        }

        switch(event.key.scancode){


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


void ConsoleVisual::Draw(sf::RenderTarget& texture){

    if(is_on){

        texture.draw(interact_box);

    }

    for(int i = 0; i < lines.size(); i++){
        texture.draw(lines[i]);
    }
    texture.draw(interact_line);

    if(show_fps){

        // prevent flickering when fps changes
        if(fps_refresh_delay_tracked > fps_refresh_delay){
            fps_refresh_delay_tracked = 0;
            fps_text.setString("FPS: " + std::to_string(1000 / Time::Dt()));

        }
        fps_refresh_delay_tracked += Time::Dt();
        texture.draw(fps_text);
    }
}

void ConsoleVisual::Print(const ConsoleLine& message, bool send_packet){
    
    // shifts each line up a place
    for(int i = line_count - 2; i >= 0; i--){
        lines[i + 1].setString(lines[i].getString());
        lines[i + 1].setColor(lines[i].getColor());
    }
    lines[0].setString(message.message);
    lines[0].setColor(message.colour);

    if(send_packet){
        //client->SendChatMessage(message.message);
    }

}
void ConsoleVisual::ClearLines(){
    for(int i = 0; i < line_count; i++){
        lines[i].setString("");
    }
}

void ConsoleVisual::SetShowFps(bool show_fps){
    this->show_fps = show_fps;
}