//
// Created by Léo Bruneau on 27/05/22.
//

#include "Weather.hpp"

Weather::Weather(double temperature, Vec2d windDirection, double windSpeed)
    : _temperature(temperature), _wind({windDirection, windSpeed}) {

}

Weather::~Weather() {

}

void Weather::update(sf::Time dt) {

}

void Weather::drawOn(sf::RenderTarget &target) const {

}
