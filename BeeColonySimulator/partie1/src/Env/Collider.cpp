/*
 * POOSV EPFL 2022
 * @authors:
 * Marco Antognini
 */

/* TO BE COMPLETED */

#include "Collider.hpp"
#include <Utility/Constants.hpp>
#include <Utility/Vec2d.hpp>
#include <Application.hpp>

Collider::Collider(Vec2d const& coordinates, double const& rad)
    : position(coordinates), radius(rad) {
    clamping(position);
}

void clamping(Vec2d& position) {
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x(); // largeur
    auto height = worldSize.y(); // hauteur

    double mod_x = fmod(position.x(), width), mod_y = fmod(position.y(), height);

    if(mod_x < 0 and mod_y < 0) {
        Vec2d clamped(mod_x + width, mod_y + height);
        position = clamped;
    } else if(mod_x < 0 and mod_y >= 0) {
        Vec2d clamped(mod_x + width, mod_y);
        position = clamped;
    } else if(mod_x >= 0 and mod_y < 0) {
        Vec2d clamped(mod_x, mod_y + height);
        position = clamped;
    } else if(mod_x >= 0 and mod_y >= 0) {
        Vec2d clamped(mod_x, mod_y);
        position = clamped;
    }
}
