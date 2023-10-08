#include "../Amber/Framework.h"

class Game : public Core{

    public:
        Game() : Core(900, 900, 600, 600, "My Game"){}

        void Start() override;
        void Update() override;
}; 