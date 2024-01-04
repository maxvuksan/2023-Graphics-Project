#include "PixelParticleSource.h"
#include "../Camera.h"
#include "../Object.h"
#include "../../Utility/Calc.h"

void PixelParticleSource::Start(){

    particle_pool.resize(30);

    for(int i = 0; i < 30; i++){
        SimulateParticle(i);
    }
    vertex_array.resize(30);
    vertex_array.setPrimitiveType(sf::Points);
}

void PixelParticleSource::SetOffset(sf::Vector2f offset){
    this->offset = offset;
}

void PixelParticleSource::SimulateParticle(int pool_index){
    particle_pool[pool_index].local_position = sf::Vector2f(rand() % (spawn_variance * 2) - spawn_variance, rand() % (spawn_variance * 2) - spawn_variance);
    particle_pool[pool_index].lifetime = 0.5 + (rand() % 10) * 0.05;
    particle_pool[pool_index].speed = rand() % 2 * 0.005f + 0.005f;
}

void PixelParticleSource::Draw(sf::RenderTarget& surface){
    
    for(int i = 0; i < vertex_array.getVertexCount(); i++){

        vertex_array[i].color = Calc::Lerp(start_colour, end_colour, 1.0f - particle_pool[i].lifetime); 
        vertex_array[i].position = Camera::WorldToScreenPosition(particle_pool[i].local_position + object->GetTransform()->position + offset);
        particle_pool[i].local_position.y -= Time::Dt() * particle_pool[i].speed;

        particle_pool[i].lifetime -= 0.007 * Time::Dt();

        if(particle_pool[i].lifetime < 0){
            SimulateParticle(i);
        }
    }

    surface.draw(vertex_array);

}