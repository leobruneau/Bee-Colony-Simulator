//
// Created by Léo Bruneau on 12/04/22.
//

#include <Application.hpp>
#include <iostream>
#include "Env.hpp"


Env::Env() {
    try {
        Env::loadWorldFromFile();
    }
    catch (std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
        Env::reset();
    }
}

Env::~Env() {
    flowerDestroyer();
}

void Env::update(sf::Time dt) {
    for (auto & f: flowers_) {
        f->update(dt);
    }
}

void Env::drawOn(sf::RenderTarget &target) {
    world_.drawOn(target);
    for (auto const& f: flowers_) {
        f->drawOn(target);
    }
    for (auto const& f: newFlowers_) {
        f->drawOn(target);
    }
}

void Env::reset() {
    flowerDestroyer();
    world_.reset(true);
}

float Env::getSize() {
    return world_.getSize();
}

void Env::resetControls() {
    // to be continued...
}

void Env::loadWorldFromFile() {
    flowerDestroyer();
    world_.loadFromFile();
}

bool Env::addFlowerAt(const Vec2d &p, bool split) {
    if (world_.isGrowable(p) and (int)flowers_.size() < getAppConfig().max_flowers) {
        double flowerSize (getAppConfig().flower_manual_size);
        double pollen (uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max));

        // Dynamically allocating memory on the heap for a newly created flower
        if (split) {
            newFlowers_.push_back(new Flower(p, flowerSize, pollen));
        } else {
            flowers_.push_back(new Flower(p , flowerSize, pollen));
        }
        return true;
    } else return false;
}

void Env::drawFlowerZone(sf::RenderTarget &target, const Vec2d &position) {
    auto size = getAppConfig().flower_manual_size;
    sf::Color color;
    if (world_.isGrowable(position)) color = sf::Color::Green;
    else color = sf::Color::Red;
    auto thickness (3.0);

    auto shape = buildAnnulus(position, size, color, thickness);
    target.draw(shape);
}

void Env::flowerDestroyer() {
    if (!flowers_.empty())
        for (auto & flower : flowers_) {
                delete flower;
        }
    flowers_.clear();

    if (!newFlowers_.empty())
        for (auto & flower: newFlowers_) {
            delete flower;
        }
    newFlowers_.clear();
}

double Env::getPixelHumidity(Vec2d const& position) {
    return world_.getHumidity(position);;
}

void Env::removeDeadFlowers() {
    for (auto & f: flowers_) {
        if (f->getPollen() <= 0) {
            delete f;
            f = nullptr;
        }
    }

    for (auto & f: newFlowers_) {
        if (f->getPollen() <= 0) {
            delete f;
            f = nullptr;
        }
    }

    flowers_.erase(std::remove(flowers_.begin(), flowers_.end(), nullptr), flowers_.end());
    newFlowers_.erase(std::remove(newFlowers_.begin(), newFlowers_.end(), nullptr), newFlowers_.end());
}

























// end of file
