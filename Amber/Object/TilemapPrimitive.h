#pragma once
#include <SFML/Graphics.hpp>

class TilemapPrimitive : public sf::Drawable, public sf::Transformable
{
    /*
        TilemapPrimitive:

        depends on inherited functionality provided by Tilemap
    */

    public:
        
        bool Load(sf::Texture* texture, sf::Vector2u tile_size, unsigned int width, unsigned int height, const int* tiles);

        /*
            @returns A 2D vector of integer values representing the tilemap
        */
        std::vector<std::vector<int>>& GetGrid();
        void SetTile(int tile_index, unsigned int x, unsigned int y);
        /*
            swaps back to the tilemap texture
        */
        void RevertTexture();
        /*
            sets the drawing texture
        */
        void SetTexture(sf::Texture* texture);


        sf::Vector2u size;
        sf::Vector2u tile_size;
        
    private:

        std::vector<std::vector<int>> grid;

        sf::VertexArray m_vertices;
        
        sf::Texture* texture_reference;
        sf::Texture* focus_texture;


        /*
            base function for drawing, not same as the Draw() component function
        */
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            // apply the transform
            states.transform *= getTransform();
            // apply the tileset texture
            states.texture = focus_texture;
            // draw the vertex array
            target.draw(m_vertices, states);
        }

};