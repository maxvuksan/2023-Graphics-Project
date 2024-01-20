#pragma once

/*
    determines how a collider interacts with other colliders
*/
enum CollisionInteractionMode{
    ALL, // collides with everything
    IGNORE_PLATFORM, // ignores platform colliders (used for platform dropdown mechanic)
};