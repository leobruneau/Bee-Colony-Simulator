//
// Created by Léo Bruneau on 28/05/22.
//

#include "FogGenerator.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"

FogGenerator::FogGenerator()
    : _elapsed(sf::Time::Zero) {
    // body of constructor
}

void FogGenerator::update(sf::Time dt) {

    _elapsed += dt;
    if (_elapsed > sf::seconds((float)getAppConfig().fog_generator_delay) and getAppConfig().active_fog_generator) {
        _elapsed = sf::Time::Zero;

        auto randomX(uniform(.0, getApp().getEnvSize().x()));
        auto randomY (uniform(.0, getApp().getEnvSize().y()));
        Vec2d rPos(randomX, randomY);

        getAppEnv().addFogAt(rPos);
    }
}
