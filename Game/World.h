#include "../Amber/Framework.h"

class World : public Object{
    
    public:
        void Start() override;
        void Update() override;

    private:
        std::vector<std::vector<Object*>> chunks;
        
        int tilemap_width = 15;
        int tilemap_height = 15;

        // in chunks...
        int width = 20;
        int height = 6;

        float delay = 500;
        float delay_tracked = 0;
        int t_index = 0;
        


};