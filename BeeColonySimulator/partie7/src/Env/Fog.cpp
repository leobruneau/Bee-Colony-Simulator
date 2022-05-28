//
// Created by Léo Bruneau on 27/05/22.
//

#include "Fog.hpp"
#include "../Application.hpp"

Fog::Fog(Vec2d position, double radius, double density)
    : Collider(position, radius), _density(density) {
    // body of constructor
}

Fog::~Fog() {

}

void Fog::update(sf::Time dt) {
//    _density = 255*(_density > 255) + 255*(_density < 0);
    passiveMovement(dt);
}

void Fog::drawOn(sf::RenderTarget &target) const {
    auto fogSprite (buildSprite(getPosition(), getRadius(), getAppTexture(getAppConfig().fog_texture)));
    fogSprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)_density));
    target.draw(fogSprite);

    if (isDebugOn()) showDebugInfo(target);
}

void Fog::passiveMovement(sf::Time dt) {
    Wind wind (getAppEnv().getWind());
    double factor (pow(getRadius(), .3));
    auto dx ((wind._direction*wind._speed*dt.asSeconds()) / factor);
    Collider::move(dx);
}

void Fog::showDebugInfo(sf::RenderTarget &target) const {
//    auto radius (buildAnnulus(getPosition(), getRadius()*.3, sf::Color::Green, 2));
    Vec2d factor (.3, .13);
    Vec2d topLeft (getPosition() - factor*getRadius());
    Vec2d bottomRight (getPosition() + factor*getRadius());
    auto rect (buildRectangle(topLeft, bottomRight, sf::Color::Green, 2));
    target.draw(rect);
}
