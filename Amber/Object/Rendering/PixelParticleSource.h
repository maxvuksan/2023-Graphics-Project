
#pragma once
#include "../Component.h"

class PixelParticleSource : public Component{


    public:

        struct Particle{

            sf::Vector2f local_position;
            float lifetime;
            float speed;
        };


        void Start() override;
        void Draw(sf::RenderTarget& surface) override;

        void SetOffset(sf::Vector2f offset);

    private:

        void SimulateParticle(int pool_index);

        sf::Vector2f offset;

        sf::Color start_colour = sf::Color(255, 176, 92, 255);
        sf::Color end_colour = sf::Color(100,0,100,30);

        int spawn_variance = 2;

        std::vector<Particle> particle_pool;
        sf::VertexArray vertex_array;



};