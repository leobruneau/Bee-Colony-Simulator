//
// Created by Léo Bruneau on 12/04/22.
//

#include <Application.hpp>
#include <iostream>
#include "Env.hpp"
#include "HelperFunctions.hpp"


Env::Env()
    : world_(),
      flowers_(std::vector<Flower*>()),
      newFlowers_(std::vector<Flower*>()),
      hives_(std::vector<Hive*>()) {

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
    hiveDestroyer();
}

void Env::update(sf::Time dt) {
    for (auto & f: flowers_) {
        f->update(dt);
    }
    flowerGenerator_.update(dt);
}

void Env::drawOn(sf::RenderTarget &target) const {
    world_.drawOn(target);
    for (auto const& f: flowers_)
        f->drawOn(target);
    for (auto const& nf: newFlowers_)
        nf->drawOn(target);
    for (auto const& h: hives_)
        h->drawOn(target);
}

void Env::reset() {
    flowerDestroyer();
    hiveDestroyer();
    world_.reset(true);
    flowerGenerator_.reset();
}

float Env::getSize() {
    return world_.getSize();
}

void Env::resetControls() {
    // to be continued...
}

void Env::loadWorldFromFile() {
    flowerDestroyer();
    hiveDestroyer();
    world_.loadFromFile();
}

void Env::saveWorldToFile() const {
    world_.saveToFile();
}

bool Env::addFlowerAt(const Vec2d &p, bool split) {
    double flowerSize (getAppConfig().flower_manual_size);
    double pollen (uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max));
    auto toAdd (new Flower(p, flowerSize, pollen));

    // Checking if the flower collides with any hive
    if (!hives_.empty()) {
        for (auto& h: hives_) {
            if (h->isColliding(*toAdd)) {
                delete toAdd;
                toAdd = nullptr;
                return false;
            }
        }
    }

    // checking that a flower can actually grow. Added the condition of strictly positive humidity for being able to grow
    if ((world_.isGrowable(p) and (int)flowers_.size() < getAppConfig().max_flowers) and world_.getHumidity(p) > 0) {

        // Dynamically allocating memory on the heap for a newly created flower
        if (split) {
            newFlowers_.push_back(toAdd);
            toAdd = nullptr;
        } else {
            flowers_.push_back(toAdd);
            toAdd = nullptr;
        }
        return true;
    } else { // deallocating memory if the flower cannot be added
        delete toAdd;
        toAdd = nullptr;
        return false;
    }
}

void Env::drawFlowerZone(sf::RenderTarget &target, const Vec2d &position) {

    auto size (getAppConfig().flower_manual_size);
    auto pollen (uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max));
    auto toAdd (new Flower(position, size, pollen));

    sf::Color color;
    if (world_.isGrowable(position) and world_.getHumidity(position) > 0) color = sf::Color::Green;
    else color = sf::Color::Red;

    if (!hives_.empty()) {
        for (auto& h: hives_)
            if (h->isColliding(*toAdd)) color = sf::Color::Red;
    }

    auto thickness (3.0);
    auto shape = buildAnnulus(position, size, color, thickness);
    target.draw(shape);

    delete toAdd;
    toAdd = nullptr;
}

void Env::flowerDestroyer() {
    if (!flowers_.empty())
        for (auto & flower : flowers_)
                delete flower;
    flowers_.clear();

    if (!newFlowers_.empty())
        for (auto & flower: newFlowers_)
            delete flower;
    newFlowers_.clear();
}

double Env::getCellHumidity(Vec2d const& position) {
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

bool Env::addHiveAt(const Vec2d &position) {
    double randomSize (uniform(getAppConfig().hive_min_size, getAppConfig().hive_max_size));

    if (isHiveable(position, randomSize)) {
        hives_.push_back(new Hive (position, randomSize));
        return true;
    } else return false;
}

Hive* Env::getCollidingHive(const Collider &body) {
    for (auto const& h: hives_)
        if (h->isColliding(body)) return h;
    return nullptr;
}

Flower* Env::getCollidingFlower(const Collider &body) {
    for (auto const& f: flowers_)
        if (f->isColliding(body)) return f;
    for (auto const& nf: newFlowers_)
        if (nf->isColliding(body)) return nf;
    return nullptr;
}

void Env::hiveDestroyer() {
    for (auto& h: hives_) {
        delete h;
        h = nullptr;
    }
    hives_.clear();
}

void Env::drawHiveableZone(sf::RenderTarget &target, const Vec2d &position) {
    double randomSize (uniform(getAppConfig().hive_min_size, getAppConfig().hive_max_size));
    double extRadius (randomSize*getAppConfig().hiveable_factor);
    sf::Color color (sf::Color::Green);
    Vec2d temp (extRadius, extRadius);
    sf::Vector2i topLeft(help::getX(position-temp, world_.getCellSize()), help::getY(position-temp, world_.getCellSize()));
    sf::Vector2i bottomRight(help::getX(position+temp, world_.getCellSize()), help::getY(position+temp, world_.getCellSize()));

    if (!world_.isHiveable(position, randomSize)) color = sf::Color::Red;
    if (!isHiveable(position, randomSize)) color = sf::Color::Blue;

    sf::RectangleShape shape(buildRectangle(topLeft, bottomRight, color, 5.0));
    target.draw(shape);
}

bool Env::isHiveable(const Vec2d &position, double radius) const {
    if (position.x() >= 0 and position.x() < world_.getSize()) {
        if (position.y() >= 0 and position.y() < world_.getSize()) {
            Hive* toAdd (new Hive (position, radius));

            if (!flowers_.empty()) {
                for (auto const& f: flowers_) {
                    if (toAdd->isColliding(*f)) {
                        delete toAdd; toAdd = nullptr;
                        return false;
                    }
                }
            }

            if (!newFlowers_.empty()) {
                for (auto const& nf: newFlowers_) {
                    if (toAdd->isColliding(*nf)) {
                        delete toAdd; toAdd = nullptr;
                        return false;
                    }
                }
            }

            if (!hives_.empty()) {
                for (auto const& h: hives_) {
                    if (toAdd->isColliding(*h)) {
                        delete toAdd; toAdd = nullptr;
                        return false;
                    }
                }
            }

            delete toAdd;
            toAdd = nullptr;
            return true;

        } else return false;
    } else return false;
}

























// end of file
