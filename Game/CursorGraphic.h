#include "../Amber/Framework.h"

// overlay to indicate the cursors interactions with the world, (e.g. breaking a block)
class CursorGraphic : public Object {

    public:
        void Start() override{
            
            sr = AddComponent<SpriteRenderer>();
            sr->SetTexture("cursor_breaking", false);
            sf::Texture* texture = AssetManager::GetTexture("cursor_breaking");

            cellsize = texture->getSize().y;
            width = texture->getSize().x;
            completeness = -1;
            SetComplete(0);
        }

        void SetComplete(float portion){
            if(portion == this->completeness){
                return;
            }
            
            this->completeness = Calc::Clamp(portion, 0, 1);

            int frame_index = floor((completeness * width) / (float)cellsize);  
            sr->SetTextureRect(frame_index * cellsize, 0, cellsize, cellsize);    
        }

    private:
        float completeness;

        int width;
        int cellsize;
        
        SpriteRenderer* sr;
};