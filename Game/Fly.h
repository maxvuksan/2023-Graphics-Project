#include "../Amber/Framework.h"
#include "Pathfinding/PathfindingGraph.h"

class Fly : public Object {

    public:

        void Start() override{

            AnimationRenderer* ar = AddComponent<AnimationRenderer>();
            ar->SetAnimationSet("fly");
            ar->GetSprite().setOrigin(sf::Vector2f(-2,-2));

            search_again_delay = rand() % 2000 + 1000;

            NewEndpoint();
        }

        void Update() override{

            
            if(current_node < path.size()){
                sf::Vector2f direction = Calc::Normalize(sf::Vector2f(path.at(current_node).x * 8, path.at(current_node).y * 8) - GetTransform()->position);
                GetTransform()->position += direction * Time::Dt() * 0.07f;

                if(Calc::Distance(sf::Vector2f(path[current_node].x * 8, path[current_node].y * 8), GetTransform()->position) < 3){
                    current_node++;
                }

            }
            else if(search_again_delay_tracked > search_again_delay){
                search_again_delay_tracked = 0;
                NewEndpoint();
            }

            search_again_delay_tracked += Time::Dt();

            //std::cout << GetTransform()->position.x << " " << GetTransform()->position.y << "\n";
        }

        void NewEndpoint(){
            end_point = sf::Vector2f(rand() % 120 - 60, rand() % 120 - 60);
            path = PathfindingGraph::RequestPathWorld(GameClient::player_pos + end_point, GetTransform()->position, 90);
            current_node = 0;
        }

    private:

        float search_again_delay = 1000;
        float search_again_delay_tracked = 0;

        int current_node = 0;
        std::vector<sf::Vector2i> path; 
        sf::Vector2f end_point;
};