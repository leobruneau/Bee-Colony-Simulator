//
// Created by Léo Bruneau on 04/05/22.
//

#include "FlowerGenerator.hpp"
#include "Application.hpp"

FlowerGenerator::FlowerGenerator()
    : elapsed(sf::Time::Zero) {}

void FlowerGenerator::update(sf::Time dt) {

    elapsed += dt;
    if (elapsed > sf::seconds((float)getAppConfig().flower_generator_delay) and getAppConfig().active_flower_generator) {
        elapsed = sf::Time::Zero;

        auto randomX(uniform(.0, getApp().getEnvSize().x()));
        auto randomY (uniform(.0, getApp().getEnvSize().y()));
        Vec2d rPos(randomX, randomY);

        getAppEnv().addFlowerAt(rPos);
    }
}

void FlowerGenerator::reset() {
    elapsed = sf::Time::Zero;
}
