#pragma once
#include <string>

class Settings {

    public:
    
        static std::string SAVE_PATH;

        static float LIGHT_BLUR_FACTOR;
        static int LIGHT_REFRESH_RATE; // values could range from say 20 - 200 

        static bool AMBIENT_OCCLUSION;
        static float AMBIENT_OCCLUSION_BLUR_FACTOR;
        static int AMBIENT_OCCLUSION_ALPHA;

};