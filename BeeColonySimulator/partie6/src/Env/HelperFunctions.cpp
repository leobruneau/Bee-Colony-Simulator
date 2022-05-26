//
// Created by Léo Bruneau on 03/05/22.
//

#include "HelperFunctions.hpp"

int help::getIndex(const Vec2d &p, float const& size, int const& nb) {
    return trunc(p.x()/size) + trunc(p.y()/size)*nb;
}

int help::getX(const Vec2d &p, const float &size) {
    return trunc(p.x()/size);
}

int help::getY(const Vec2d &p, const float &size) {
    return trunc(p.y()/size);
}
