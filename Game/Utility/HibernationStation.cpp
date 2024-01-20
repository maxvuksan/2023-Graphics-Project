#include "HibernationStation.h"
#include "../World/Lighting/LightSource.h"

void HibernationStation::Start(){
    
    auto ls = AddComponent<LightSource>();
    ls->offset = sf::Vector2f(
            ItemDictionary::UTILITY_BLOCK_DATA[utility_Hibernator].footprint.x * ItemDictionary::tile_size / 2.0f,
            ItemDictionary::UTILITY_BLOCK_DATA[utility_Hibernator].footprint.y * ItemDictionary::tile_size / 2.0f);

    ls->colour = sf::Color::White;
    ls->decay = 0.03;
}