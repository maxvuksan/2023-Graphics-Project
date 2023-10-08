#include "../Amber/Framework.h"
#include "Player.h"
#include "World.h"
#include <math.h>

class TestScene : public Scene {

    public: 

        Transform* cam;

        void Start() override{

            Player* player = AddObject<Player>();
            player->GetTransform()->position = sf::Vector2f(50, -500);

            World* world = AddObject<World>();

    /*
            Object* o = AddObject<Object>();
            o->AddComponent<Camera>();
            SetActiveCamera(o->GetComponent<Camera>());
            cam = o->GetTransform();
    */
            world->SetFocus(player->GetTransform());

            /*
            Object* o = AddObject<Object>();    
            auto tm = o->AddComponent<Tilemap>();

            tm->Load("demoTexture", 8, 8, 100, 100);
            tm->SetAll(10);
            tm->SetRow(-1, 3);
            tm->SetArea(-1, 0, 15, 0, 50);
            tm->SetArea(1, 10, 19, 10, 14);
            tm->SetRow(-1,5);
            tm->SetTile(-1,4,6);
            tm->SetArea(-1, 30, 90, 20, 53);
            tm->SetTile(1, 35, 42);
            tm->SetTile(0, 36, 42);
            tm->SetTile(1, 45, 30);
            tm->SetTile(0, 40, 40);
            tm->SetArea(-1, 60, 85, 30, 100);
            tm->SetArea(0, 80, 95, 30, 70);
            tm->SetArea(0, 50, 58, 24, 35);
            tm->SetRow(-1,31);
            tm->SetRow(-1,32);
            tm->SetRow(-1,33);
            tm->SetColumn(-1, 42);
            tm->SetColumn(-1, 43);
            tm->SetColumn(-1, 45);
            tm->SetColumn(-1, 46);
            tm->SetColumn(-1, 47);
            tm->SetTile(-1, 7,2);
            tm->SetTile(-1, 6,1);

            o->AddComponent<TilemapCollider>();
            */
        };

        void Update() override{

            float speed = 0.1f;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                speed = 0.5f;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                cam->position.x -= Time::Dt() * speed;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                cam->position.x += Time::Dt() * speed;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                cam->position.y -= Time::Dt() * speed;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                cam->position.y += Time::Dt() * speed;
            }
        }

};