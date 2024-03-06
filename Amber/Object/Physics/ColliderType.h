#pragma once
/*
    how will other colliders interact with this
*/
enum ColliderType {
    SOLID, // all sides cannot be passed through, fully solid
    PLATFORM, // only the top side can be collided with, colliders can pass through the sides and bottoms
    TRIGGER, // no collision
};