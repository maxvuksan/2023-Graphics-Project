#include "../Amber/Framework.h"

class World : public Object{
    
    public:
        void Start() override;
        void Update() override;

        /*
            the focus is what the world orients around (only load chunks around the focus transform, etc...)
        */
        void SetFocus(Transform* focus);

    private:
        Transform* focus;

        std::vector<std::vector<Object*>> chunks;
        
        int loading_threshold = 300;
        int collider_threshold = 150;

        int tilesize_x = 8;
        int tilesize_y = 8;

        int tilemap_width = 15;
        int tilemap_height = 15;
        int half_tilemap_width;
        int half_tilemap_height;

        // in chunks...
        int width = 20;
        int height = 6;

        float delay = 500;
        float delay_tracked = 0;
        int t_index = 0;
        


};