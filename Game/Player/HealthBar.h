#include "../../Amber/Framework.h"

class HealthBar : public Object {

    public:

        HealthBar();

        void Draw(sf::RenderTarget& surface);

        int GetHealth(){return health;}
        int GetMaxHealth(){return max_health;}
        void SetHealth(int health);
        void SetMaxHealth(int max_health);

        int GetHunger(){return hunger;}
        int GetMaxHunger(){return max_hunger;}
        void SetHunger(int hunger);
        void SetMaxHunger(int max_hunger);


        

    private:

        int health = 100;
        int max_health = 200;

        int hunger = 50;
        int max_hunger = 200;

        sf::RectangleShape health_frame;
        sf::RectangleShape health_remaining;

        sf::RectangleShape hunger_frame;
        sf::RectangleShape hunger_remaining;
};