#include "BackgroundFogLayer.h"
#include "LightingManager.h"
#include "../World.h"


void BackgroundFogLayer::Draw(sf::RenderTarget& surface){

    LightingManager::ambient_occlusion_texture.clear(sf::Color::Transparent);

    // draw black tile siloutte
    for(int x = LightingManager::world->GetActiveChunksMin().x; x < LightingManager::world->GetActiveChunksMax().x; x++){
        for(int y = LightingManager::world->GetActiveChunksMin().y; y < LightingManager::world->GetActiveChunksMax().y; y++){
            
            LightingManager::world->GetChunks()->at(x).at(y)->DrawBackgroundStructures(LightingManager::ambient_occlusion_texture);
        }
    }

    LightingManager::ambient_occlusion_texture.display();
    
    sf::Sprite tex_sprite(LightingManager::ambient_occlusion_texture.getTexture());
    surface.draw(tex_sprite);

}
