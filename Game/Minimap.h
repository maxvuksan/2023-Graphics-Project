#include "../Amber/Framework.h"

class Minimap : public Object{

    public:
        Minimap();
        void Draw_Window(sf::RenderTarget&) override;
        void CatchEvent(sf::Event) override;

        void SetPixel(int x, int y);
        void RemovePixel(int x, int y);
        void Create(int width, int height);

    private:
        sf::Texture texture;
        sf::Image image;
        bool created;
        bool changed;
        bool active;
};