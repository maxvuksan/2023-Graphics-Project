#include "../Amber/Framework.h"

class Minimap : public Object{

    public:
        
        void Start();
        void CatchEvent(sf::Event) override;
        void OnSetActive() override;

        UIPixelGrid* GetPixelGrid(){return pixel_grid;}


    private:
        UIPixelGrid* pixel_grid;

};