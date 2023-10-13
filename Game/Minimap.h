#include "../Amber/Framework.h"

class Minimap : public Object{

    public:
        
        void Start();
        void CatchEvent(sf::Event) override;

        UIPixelGrid* GetPixelGrid(){return pixel_grid;}

        bool GetActive(){ return pixel_grid->IsActive(); } 

    private:
        UIPixelGrid* pixel_grid;

};