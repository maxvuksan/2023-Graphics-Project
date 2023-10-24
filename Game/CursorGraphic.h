#pragma once
#include "../Amber/Framework.h"

// overlay to indicate the cursors interactions with the world, (e.g. breaking a block)
class CursorGraphic : public Object {

    public:
        void Start() override{
            
            image = AddComponent<UIImage>();

            image->SetTexture("cursor_breaking");

            cellsize = image->height;
            
            rect.setSize(sf::Vector2f(cellsize, cellsize));
            rect.setOutlineColor(sf::Color::White);
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineThickness(1);

            completeness = -1;
            SetComplete(0);
        }

        void SetComplete(float portion){
            if(portion == this->completeness){
                return;
            }
            
            this->completeness = Calc::Clamp(portion, 0, 1);

            int frame_index = floor((completeness * image->width) / (float)cellsize);  
            image->SetTextureRect(frame_index * cellsize, 0, cellsize, cellsize);    
        }

        void Draw(sf::RenderTarget& surface) override{
            rect.setPosition(Camera::WorldToScreenPosition(GetTransform()->position));
            surface.draw(rect);
        }

    private:
        float completeness;

        UIImage* image;
        sf::RectangleShape rect;
        int cellsize;
};