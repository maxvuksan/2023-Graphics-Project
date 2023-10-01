#include "../Amber/Framework.h"

class TestScene : public Scene {

    public: 

        Transform* cam;

        void Start() override{


            Object* o2 = AddObject<Object>();
            o2->AddComponent<Camera>();
            SetActiveCamera(o2->GetComponent<Camera>());
            o2->GetComponent<Camera>()->offset = sf::Vector2f(0,0);

            o2->AddComponent<PointLight>();
            cam = o2->GetTransform();

            Object* o = AddObject<Object>();
            

            auto tm = o->AddComponent<Tilemap>();
   
            tm->Load("demoTexture", 8, 8, 100, 100);
            tm->SetAll(10);
            tm->SetRow(-1, 3);
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
        };

        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;
        bool shift = false;

        void CatchEvent(sf::Event event) override{
            if (event.type == sf::Event::KeyPressed)
            {

                if (event.key.scancode == sf::Keyboard::Scan::D)
                {
                    right = true;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::A)
                {
                    left = true;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::W)
                {
                    up = true;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::S)
                {
                    down = true;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::LShift)
                {
                    shift = true;
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.scancode == sf::Keyboard::Scan::D)
                {
                    right = false;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::A)
                {
                    left = false;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::W)
                {
                    up = false;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::S)
                {
                    down = false;
                }
                else if (event.key.scancode == sf::Keyboard::Scan::LShift)
                {
                    shift = false;
                }
            }

        }

        void Update() override{

            float speed = 0.1f;

            if(shift){
                speed = 0.5f;
            }

            if(right){
                cam->position.x += Time::Dt() * speed;
            }
            else if(left){
                cam->position.x -= Time::Dt() * speed;
            }
            if(up){
                cam->position.y -= Time::Dt() * speed;
            }
            else if(down){
                cam->position.y += Time::Dt() * speed;
            }
        }



};