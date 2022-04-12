//
// Created by Léo Bruneau on 12/04/22.
//

#include "Env.hpp"
#include <iostream>


Env::Env() {
    try {
        Env::loadWorldFromFile();
    }
    catch (std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
        Env::reset();
    }
}

void Env::update(sf::Time dt) {
    // to be continued...
}

void Env::drawOn(sf::RenderTarget &target) {
    world_.drawOn(target);
}

void Env::reset() {
    world_.reset(true);
}

float Env::getSize() {
    return world_.getSize();
}

void Env::resetControls() {
    // to be continued...
}

void Env::loadWorldFromFile() {
    world_.loadFromFile();
}
