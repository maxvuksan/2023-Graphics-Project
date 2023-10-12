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
            player->GetTransform()->position = sf::Vector2f(50, -1000);
            player->LinkWorld(world);


            world->SetFocus(player->GetTransform());

        };

        void CatchEvent(sf::Event event) override{
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.scancode == sf::Keyboard::Scan::Q){
                    Core::DEBUG_MODE = !Core::DEBUG_MODE;
                }
            }
        }

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