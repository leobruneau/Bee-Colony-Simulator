//
// Created by Léo Bruneau on 27/05/22.
//

#include "Weather.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"

Weather::Weather(double temperature, Vec2d windDirection, double windSpeed)
    : _temperature(temperature), _wind({windDirection, windSpeed}) {

}

Weather::~Weather() {
    for (auto& _f : _fog) {
        delete _f;
        _f = nullptr;
    }
    _fog.clear();
}

void Weather::update(sf::Time dt) {
    for (auto const& _f : _fog) _f->update(dt);
    _wind._speed = getAppConfig().max_wind_speed;
    _fogGenerator.update(dt);
}

void Weather::drawOn(sf::RenderTarget &target) const {
    for (auto const& _f : _fog)
        _f->drawOn(target);
    if (isDebugOn()) showDebugInfo(target);
}

void Weather::showDebugInfo(sf::RenderTarget &target) const {
    Vec2d position (getAppEnv().getSize() - 30, 30);
    auto compass (buildSprite(position, 50, getAppTexture("compass.png")));
    auto arrow (buildSprite(position, 50, getAppTexture("arrow.png")));
    target.draw(compass);
    target.draw(arrow);
}

void Weather::addFogAt(const Vec2d &p) {
    auto randomSize (uniform(getAppConfig().fog_min_size, getAppConfig().fog_max_size));
    auto fog (new Fog(p, randomSize, getAppConfig().fog_min_density));
    _fog.push_back(fog);
}

Wind Weather::getWind() const {
    return _wind;
}

void Weather::increaseTemperature() {
    ++_temperature;
    bool condition (_temperature < getAppConfig().max_temperature);
    _temperature = _temperature*condition + (!condition)*getAppConfig().max_temperature;
}

void Weather::decreaseTemperature() {
    --_temperature;
    bool condition (_temperature > getAppConfig().min_temperature);
    _temperature = _temperature*condition + (!condition)*getAppConfig().min_temperature;
}

void Weather::reset() {
    fogDestoryer();
}

void Weather::fogDestoryer() {
    for (auto& _f : _fog) {
        delete _f;
        _f = nullptr;
    }
    _fog.clear();
}
