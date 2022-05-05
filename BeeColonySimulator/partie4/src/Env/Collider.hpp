/*
 * POOSV EPFL 2022
 * @authors: 
 */

#pragma once

#include <Utility/Vec2d.hpp>
#include <iostream>
#include <fstream>

/* TO BE COMPLETED */

class Collider {
private:
    Vec2d position;
    double radius;
public:
    Collider(Vec2d const& coordinates, double const& rad);
    Collider(Collider const& other);
    Collider& operator=(Collider const& source);
    Collider& operator+=(Vec2d const& dx);

    Vec2d const& getPosition() const;
    double getRadius() const;
    Vec2d directionTo(Vec2d const& to) const;
    Vec2d directionTo(Collider const& to) const;
    double distanceTo(Vec2d const& to) const;
    double distanceTo(Collider const& to) const;
    void move(Vec2d const& dx);

    bool isColliderInside(Collider const& other) const;
    virtual bool isColliding(Collider const& other) const;
    bool isPointInside(Vec2d const& point) const;

    bool operator>(Collider const& other) const;
    bool operator|(Collider const& other) const;
    bool operator>(Vec2d const& point) const;
};

std::ostream& operator<<(std::ostream&, Collider const& other);

void clamping(Vec2d& position);
