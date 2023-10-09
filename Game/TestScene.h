#include "../Amber/Framework.h"
#include "Player.h"
#include "World.h"
#include <math.h>

class TestScene : public Scene {

    public: 

        Transform* cam;

        void Start() override{

            World* world = AddObject<World>();


            Player* player = AddObject<Player>();
            player->GetTransform()->position = sf::Vector2f(50, -500);
            player->LinkWorld(world);

    /*
            Object* o = AddObject<Object>();
            o->AddComponent<Camera>();
            SetActiveCamera(o->GetComponent<Camera>());
            cam = o->GetTransform();
    */
            world->SetFocus(player->GetTransform());

        };

        void Update() override{

            return;
            
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