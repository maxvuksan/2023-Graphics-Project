#pragma once
#include "../../Amber/Framework.h"

// overlay to indicate the cursors interactions with the world, (e.g. breaking a block)
class CursorGraphic : public Object {

    public:
        void Start() override{

            cellsize = 8;

            cursor_sprite.setTexture(*AssetManager::GetTexture("cursor"));
            breaking_sprite.setTexture(*AssetManager::GetTexture("cursor_breaking"));

            cursor_sprite.setTextureRect(sf::IntRect(cellsize,0,cellsize,cellsize));
                    
            completeness = -1;
            SetComplete(0);
            auto_target_blocks = false;
        }

        void SetAutoTargetBlocks(bool auto_target_blocks){

            this->auto_target_blocks = auto_target_blocks;
            if(auto_target_blocks){
                cursor_sprite.setTextureRect(sf::IntRect(0,0,cellsize,cellsize));
                cursor_sprite.setOrigin(sf::Vector2f(0,0));
            }
            else{
                cursor_sprite.setTextureRect(sf::IntRect(cellsize,0,cellsize,cellsize));
                cursor_sprite.setOrigin(sf::Vector2f(cellsize / 2.0f, cellsize / 2.0f));
            }
        }

        void SetBreakingPosition(sf::Vector2f snapped_position){
            breaking_position = snapped_position;
            if(auto_target_blocks){
                GetTransform()->position = snapped_position;
            }
        }

        void SetComplete(float portion){
            if(portion == this->completeness){
                return;
            }
            
            this->completeness = Calc::Clamp(portion, 0, 1);

            int frame_index = floor((completeness * breaking_sprite.getTexture()->getSize().x) / (float)cellsize);  
            breaking_sprite.setTextureRect(sf::IntRect(frame_index * cellsize, 0, cellsize, cellsize));    
        }

        void Draw(sf::RenderTarget& surface) override{
            cursor_sprite.setPosition(Camera::WorldToScreenPosition(GetTransform()->position));
            breaking_sprite.setPosition(Camera::WorldToScreenPosition(breaking_position));
            
            surface.draw(breaking_sprite);
            surface.draw(cursor_sprite);
        }

    private:
        float completeness;
        bool auto_target_blocks;


        sf::Vector2f breaking_position;
        sf::Sprite breaking_sprite;
        sf::Sprite cursor_sprite;

        int cellsize;
};