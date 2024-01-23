#pragma once
#include "UIRect.h"
#include <functional>
class UIButton : public UIRect{

    public:

        UIButton();
        
        void CatchEvent(sf::Event event) override;
        void Update() override;

        void Draw(sf::RenderTarget& surface) override;

        void SetString(std::string string);

        // @param on_click_function lambda callback to be called when the button is clicked
        void SetOnClickCallback(const std::function<void()>& callback_function);

        // all buttons share a text object for configuration ease
        static sf::Text& GetText(){return text;}


    protected:
        void UpdateTextPosition();
        bool mouse_over;

    private:

        std::function<void()> on_click_function;

        std::string text_string;
        sf::Vector2f text_position;
        sf::Vector2f text_origin;
        sf::Vector2f previous_position;

        sf::RectangleShape outline_shape;
        
        static sf::Text text;

};