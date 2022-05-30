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
    bool fogIncrease (getAppEnv().fogHumidityThreshold(getPosition()));
    double incFactor (getAppConfig().fog_increase_density_factor);
    double decFactor (getAppConfig().fog_decrease_density_factor);
    _density += incFactor*fogIncrease - !fogIncrease*decFactor;
    int maxDensity(getAppConfig().fog_max_density);
    bool co1 (_density > maxDensity), co2 (_density < 0);

    _density = _density*(!co1 and !co2) + maxDensity*co1;
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
    TopLeftBottomRight corners (findCloudCorners());
    auto rect (buildRectangle(corners._topLeft, corners._bottomRight, sf::Color::Green, 2));
    target.draw(rect);
}

bool Fog::isDead() const {
    return _density < getAppConfig().fog_min_density;
}

bool Fog::isPointInsideActiveZone(const Vec2d &p) const {
    TopLeftBottomRight corners (findCloudCorners());

    if (p.x() >= corners._topLeft.x() and p.x() <= corners._bottomRight.x()) {
        if (p.y() >= corners._topLeft.y() and p.y() <= corners._bottomRight.y()) {
            return true;
        }
    }

    return false;
}

TopLeftBottomRight Fog::findCloudCorners() const {
    double x (getAppConfig().fog_active_site_factor_x);
    double y (getAppConfig().fog_active_site_factor_y);
    Vec2d factor (x, y);
    Vec2d topLeft (getPosition() - factor*getRadius());
    Vec2d bottomRight (getPosition() + factor*getRadius());
    TopLeftBottomRight temp({topLeft, bottomRight});
    return temp;
}
