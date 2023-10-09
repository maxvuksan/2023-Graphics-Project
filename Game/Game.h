#include "../Amber/Framework.h"

class Game : public Core{

    public:
        Game() : Core(1200, 1200, 300, 300, "My Game"){}

        void Start() override;
        void Update() override;
}; 