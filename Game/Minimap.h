#include "../Amber/Framework.h"

class Minimap : public Object{

    public:
        
        void Start();
        void CatchEvent(sf::Event) override;
        void OnSetActive() override;

        void Update() override;

        UIPixelGrid* GetPixelGrid(){return pixel_grid;}


    private:
        UIPixelGrid* pixel_grid;

        sf::Vector2i mouse_pan_inital; // position of mouse when panning started
        sf::Vector2f pan_position_inital; // position of the object when panning started
        bool set_pan;

};