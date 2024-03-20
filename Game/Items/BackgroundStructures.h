#pragma once
/*
    large background pieces which cannot be interacted with, bound to chunks
*/

struct BackgroundStructreData{
    std::string name;
    sf::Vector2i pixel_position_on_sprite_sheet;
    sf::Vector2i pixel_dimensions;
};

enum BackgroundStructure {

    structure_BoilerPlate,
    structure_ChamberTop,
    structure_Chamber,
    structure_ChamberBottom,

    structure_SupportHorizontal,
    structure_SupportVertical,
    structure_SupportBoilerPlate,

    structure_NUMBER_OF_STRUCTURES,

};