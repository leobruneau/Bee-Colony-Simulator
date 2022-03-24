/*
 * POOSV EPFL 2022
 * @authors: 
 */

#pragma once

#include <Utility/Vec2d.hpp>
#include <iostream>

/* TO BE COMPLETED */

class Collider {
private:
    Vec2d position;
    double radius;
public:
    Collider(Vec2d const& coordinates, double const& rad);
    Collider(Collider const& other);
    Collider& operator=(const Collider& source);

    Vec2d const& getPosition() const;
    double getRadius() const;
    Vec2d positionTo(Vec2d to);
};

void clamping(Vec2d& position);
