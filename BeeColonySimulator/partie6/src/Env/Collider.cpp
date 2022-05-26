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
#include <vector>
#include <utility>
#include <cmath>

Collider::Collider(Vec2d const& coordinates, double const& rad)
    : position(coordinates), radius(rad) {
    clamping(position);
}

Collider::Collider(Collider const& other)
    : position(other.position), radius(other.radius) {
    // body of copy-constructor
}

Vec2d const& Collider::getPosition() const {
    return position;
}

double Collider::getRadius() const {
    return radius;
}

//Collider& Collider::operator=(Collider const& source) {
//    position = source.position;
//    radius = source.radius;
//    return *this;
//}

/*
 * Cleaner implementation of = operator overloading since we are just copying
 * the values from one object (the source) to another (the target)
*/

Collider& Collider::operator=(Collider const& source) = default;

Vec2d Collider::directionTo(Vec2d const& to) const {
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x();
    auto height = worldSize.y();
    double delta_x(to.x()-position.x()), delta_y(to.y()-position.y());

    Vec2d nearestTo(to);

    if(delta_x < - width/2) {
        Vec2d change(width, 0);
        nearestTo += change;
    } else if(delta_x > width/2) {
        Vec2d change(-width, 0);
        nearestTo += change;
    }

    if(delta_y < -height/2) {
        Vec2d change(0, height);
        nearestTo += change;
    } else if(delta_y > height/2) {
        Vec2d change(0, -height);
        nearestTo += change;
    }

    return {nearestTo.x()-position.x(), nearestTo.y()-position.y()};
}

Vec2d Collider::directionTo(Collider const& to) const {
    return directionTo(to.position);
}

double Collider::distanceTo(const Vec2d &to) const {
    return (directionTo(to)).length();
}

double Collider::distanceTo(const Collider &to) const {
    return (directionTo(to.position)).length();
}

void Collider::move(Vec2d const& dx) {
    position += dx;
    clamping(position);
}

bool Collider::isColliderInside(Collider const& other) const {
    if ((other.radius <= radius) and (distanceTo(other) <= (radius - other.radius))) {
        return true;
    } else {
        return false;
    }
}

bool Collider::isColliding(Collider const& other) const {
    double radiusSum(radius + other.radius);
    if((distanceTo(other)) <= radiusSum) {
        return true;
    } else {
        return false;
    }
}

bool Collider::isPointInside(const Vec2d &point) const {
    if(distanceTo(point) <= radius) {
        return true;
    } else {
        return false;
    }
}

bool Collider::operator>(Collider const& other) const {
    return isColliderInside(other);
}

bool Collider::operator|(Collider const& other) const {
    return isColliding(other);
}

bool Collider::operator>(Vec2d const& point) const {
    return isPointInside(point);
}

Collider& Collider::operator+=(Vec2d const& dx) {
    position += dx;
    return *this;
}

double Collider::getFactor() const {
    return 1;
}

void clamping(Vec2d& position) {
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x(); // width
    auto height = worldSize.y(); // height

    do {
        // this loop we ensure that the clamping works even if the coordinates of
        //  the position are over bounds by more than one time the width and/or height

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

    } while (!(position.x() <= width and position.x() >= 0) or !(position.y() <= height and position.y() >= 0));
}

std::ostream& operator<<(std::ostream& out, Collider const& other) {
    out << "Collider: position = " << "(" << other.getPosition().x() << ", " << other.getPosition().y() << ") radius = " << other.getRadius();
    return out;
}





















// end