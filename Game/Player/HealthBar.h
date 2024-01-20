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
        
        int GetCycle(){return cycle;}
        int GetMaxCycle(){return max_cycle;}
        void SetCycle(int cycle);
        void SetMaxCycle(int max_cycle);

        

    private:

        static int hunger_decay_delay;
        float hunger_decay_delay_tracked;

        int health;
        int max_health = 200;

        int hunger;
        int max_hunger = 200;

        int cycle = 50;
        int max_cycle = 200;

        sf::RectangleShape health_frame;
        sf::RectangleShape health_remaining;

        sf::RectangleShape hunger_frame;
        sf::RectangleShape hunger_remaining;

        sf::RectangleShape cycle_frame;
        sf::RectangleShape cycle_remaining;
};