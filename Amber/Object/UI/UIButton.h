#pragma once
#include "UIRect.h"
#include <functional>
class UIButton : public UIRect{

    public:

        UIButton();
        
        void CatchEventEventFocusBounded(sf::Event event) override;  

        void Update() override;

        void Draw(sf::RenderTarget& surface) override;

        void SetString(std::string string);
        void SetTexture(const char* label);
        void SetTextureRect(int left, int top, int width, int height);


        // @param on_click_function lambda callback to be called when the button is clicked
        void SetOnClickCallback(const std::function<void()>& callback_function);

        // all buttons share a text object for configuration ease
        static sf::Text& GetText(){return text;}


    protected:
        void UpdateTextPosition();
        bool mouse_over;

    private:

        std::function<void()> on_click_function;

        sf::Sprite sprite;

        std::string text_string;
        sf::Vector2f text_position;
        sf::Vector2f text_origin;
        sf::Vector2f previous_position;

        sf::RectangleShape outline_shape;
        
        static sf::Text text;

};