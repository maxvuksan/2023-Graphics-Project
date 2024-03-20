#include "BackgroundShadowManager.h"
#include "../../Settings.h"
#include "LightingManager.h"
#include "../World.h"

void BackgroundShadowManager::Draw(sf::RenderTarget& surface){

    if(Settings::AMBIENT_OCCLUSION){
        
        // AMBIENT_OCCLUSION _________________________________________________________________________________________________________________________

        LightingManager::ambient_occlusion_texture.clear(sf::Color::White);

        AssetManager::GetShader("Amber_BlurVertical")->setUniform("u_strength", Settings::AMBIENT_OCCLUSION_BLUR_FACTOR);
        AssetManager::GetShader("Amber_BlurHorizontal")->setUniform("u_strength", Settings::AMBIENT_OCCLUSION_BLUR_FACTOR);

        // draw black tile siloutte
        for(int x = LightingManager::world->GetActiveChunksMin().x; x < LightingManager::world->GetActiveChunksMax().x; x++){
            for(int y = LightingManager::world->GetActiveChunksMin().y; y < LightingManager::world->GetActiveChunksMax().y; y++){
                LightingManager::world->GetChunks()->at(x).at(y)->GetTilemap(SetLocation::MAIN)->DrawSiloutte(LightingManager::ambient_occlusion_texture);
            }
        }

        
        // blur siloutte
        LightingManager::ambient_occlusion_texture.display();
        LightingManager::back_lighting_texture.draw(sf::Sprite(LightingManager::ambient_occlusion_texture.getTexture()), AssetManager::GetShader("Amber_BlurVertical"));
        LightingManager::back_lighting_texture.display();
        LightingManager::ambient_occlusion_texture.draw(sf::Sprite(LightingManager::back_lighting_texture.getTexture()), AssetManager::GetShader("Amber_BlurHorizontal"));
        
        LightingManager::back_lighting_texture.clear(sf::Color::Transparent);
        
        // mask occlusion to background walls only
        for(int x = LightingManager::world->GetActiveChunksMin().x; x < LightingManager::world->GetActiveChunksMax().x; x++){
            for(int y = LightingManager::world->GetActiveChunksMin().y; y < LightingManager::world->GetActiveChunksMax().y; y++){
                LightingManager::world->GetChunks()->at(x).at(y)->GetTilemap(SetLocation::BACKGROUND)->DrawWithExternalTexture(LightingManager::back_lighting_texture, AssetManager::GetTexture("Amber_White"));
            }
        }   

        LightingManager::back_lighting_texture.display();
        LightingManager::ambient_occlusion_texture.draw(sf::Sprite(LightingManager::back_lighting_texture.getTexture()), sf::BlendMultiply);
        LightingManager::ambient_occlusion_texture.display();

        // draw with banding shader to create 1px outline
        AssetManager::GetShader("Amber_Banding")->setUniform("u_band_count", 1);
        LightingManager::back_lighting_texture.draw(sf::Sprite(LightingManager::ambient_occlusion_texture.getTexture()), AssetManager::GetShader("Amber_Banding"));
        LightingManager::back_lighting_texture.display();

        LightingManager::ambient_occlusion_texture.clear(sf::Color::White);
        sf::Sprite shadow_sprite(LightingManager::back_lighting_texture.getTexture());
        LightingManager::ambient_occlusion_texture.draw(shadow_sprite);
        LightingManager::ambient_occlusion_texture.display();

        surface.draw(sf::Sprite(LightingManager::back_lighting_texture.getTexture()), sf::BlendMultiply);

        // FOLIAGE SHADOWS ___________________________________________________________________________________________________________________________

        LightingManager::back_lighting_texture.clear(sf::Color::White);
        for(int x = LightingManager::world->GetActiveChunksMin().x; x < LightingManager::world->GetActiveChunksMax().x; x++){
            for(int y = LightingManager::world->GetActiveChunksMin().y; y < LightingManager::world->GetActiveChunksMax().y; y++){

                sf::RenderStates render_states;
                render_states.transform.translate(Scene::GetActiveCamera()->WorldToScreenPosition(sf::Vector2f(LightingManager::background_shadow_offset,LightingManager::background_shadow_offset)));
                render_states.texture = AssetManager::GetTexture("foliage");
                render_states.shader = AssetManager::GetShader("Amber_ColourToBlack");

                LightingManager::back_lighting_texture.draw(LightingManager::world->GetChunks()->at(x).at(y)->GetFoliageVertexArray(), render_states);
            }
        }   
        LightingManager::back_lighting_texture.display();

        shadow_sprite.setTexture(LightingManager::back_lighting_texture.getTexture());

        surface.draw(shadow_sprite, sf::BlendMultiply);
    } 
}