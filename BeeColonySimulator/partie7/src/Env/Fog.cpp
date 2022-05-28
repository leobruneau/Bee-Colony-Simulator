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
    bool fogUpdate (getAppEnv().fogHumidityThreshold(getPosition()));
    _density += (fogUpdate - !fogUpdate)*getAppConfig().fog_increase_density_factor;
    int minDensity(getAppConfig().fog_min_density);
    int maxDensity(getAppConfig().fog_max_density);
    bool co1 (_density > maxDensity), co2 (_density < minDensity);

    _density = _density*(!co1 and !co2) + minDensity*co2 + maxDensity*co1;

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
    double x (getAppConfig().fog_active_site_factor_x);
    double y (getAppConfig().fog_active_site_factor_y);
    Vec2d factor (x, y);
    Vec2d topLeft (getPosition() - factor*getRadius());
    Vec2d bottomRight (getPosition() + factor*getRadius());
    auto rect (buildRectangle(topLeft, bottomRight, sf::Color::Green, 2));
    target.draw(rect);
}
