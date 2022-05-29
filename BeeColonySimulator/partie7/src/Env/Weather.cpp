//
// Created by Léo Bruneau on 27/05/22.
//

#include "Weather.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"
#include "HelperFunctions.hpp"

Weather::Weather(double temperature, Vec2d windDirection, double windSpeed)
    : _temperature(temperature), _wind({windDirection, windSpeed}) {
    // empty by default
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
    _fogGenerator.update(dt);
    removeInsufficientFog();
}

void Weather::drawOn(sf::RenderTarget &target) const {
    for (auto const& _f : _fog)
        _f->drawOn(target);
    if (isDebugOn()) showDebugInfo(target);
}

void Weather::showDebugInfo(sf::RenderTarget &target) const {
    // empty by default
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
    getAppEnv().temperatureEffects();
}

void Weather::decreaseTemperature() {
    --_temperature;
    bool condition (_temperature > getAppConfig().min_temperature);
    _temperature = _temperature*condition + (!condition)*getAppConfig().min_temperature;
    getAppEnv().temperatureEffects();
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

void Weather::removeInsufficientFog() {
    for (auto& _f : _fog) {
        if (_f -> isDead()) {
            delete _f;
            _f = nullptr;
        }
    }

    _fog.erase(std::remove(_fog.begin(), _fog.end(), nullptr), _fog.end());
}

double Weather::getTemperature() const {
    return _temperature;
}

void Weather::decreaseWindSpeed() {
    --_wind._speed;
    bool condition (_wind._speed > 0);
    _wind._speed = _wind._speed*condition;
}

void Weather::increaseWindSpeed() {
    ++_wind._speed;
    bool condition (_wind._speed > getAppConfig().max_wind_speed);
    _wind._speed = _wind._speed*(!condition) + getAppConfig().max_wind_speed*condition;
}

void Weather::decreaseWindDirection() {
    double step (-PI/16);
    _wind._direction.rotate(step);
    help::debugWindRotation += step;
}

void Weather::increaseWindDirection() {
    double step (PI/16);
    _wind._direction.rotate(step);
    help::debugWindRotation += step;
}
