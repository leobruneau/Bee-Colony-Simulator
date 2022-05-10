//
// Created by Léo Bruneau on 04/05/22.
//

#include "Bee.hpp"

Bee::Bee(Hive* homeHive, Vec2d &position, double radius, double energy, double speed)
    : Collider(position, radius),
    homeHive_(homeHive),
    velocity_(Vec2d::fromRandomAngle()*speed),
    energy_(energy) {
    // Constructor's body
}

void Bee::drawOn(sf::RenderTarget &target) const {
    auto beeSprite(buildSprite(getPosition(), getRadius(), getAppTexture(getConfig()["texture"].toString())));

    if (velocity_.angle() >= PI/2 or velocity_.angle() <= -PI/2) beeSprite.scale(1, -1);
    beeSprite.rotate(velocity_.angle()/DEG_TO_RAD);

    target.draw(beeSprite);
}

void Bee::update(sf::Time dt) {
    move(dt);

    if (energy_ > 0) energy_ -= 0.1*dt.asSeconds();
    else if (energy_ < 0) energy_ = 0;
}

void Bee::move(sf::Time dt) {
    double p (getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
    double alpha_max (getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
    double alpha(.0);

    if (bernoulli(p)) {
        alpha = uniform(-alpha_max, alpha_max);
        velocity_.rotate(alpha);
    }

    auto dx = velocity_*dt.asSeconds();
    auto newPosition (getPosition() + dx);
    clamping(newPosition);

    if (getAppEnv().isFlyable(newPosition)) Collider::move(velocity_*dt.asSeconds());
    else {
        double beta(.0);
        if (bernoulli(0.5)) beta = PI/4;
        else beta = -PI/4;
        velocity_.rotate(beta);
    }
}

bool Bee::isDead() const {
    if (energy_ == 0) return true;
    else return false;
}


