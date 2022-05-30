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
    for (auto& f: flowers_) {
        f->update(dt);
    }

    for (auto& h: hives_) {
        h->update(dt);
    }

    flowerGenerator_.update(dt);
    removeDeadFlowers();
    flowers_.insert(flowers_.end(), newFlowers_.begin(), newFlowers_.end());
    newFlowers_.clear();
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

    // Checking if the flower's position isn't inside any other flower's collider range
    // We don't check if the flower we want to add is "colliding" because this way we allow more flexibility.
    // In real life we can have flowers really close to one another : )
    if (!flowers_.empty()) {
        for (auto const& f : flowers_) {

            // 7 is actually a random number we found was satisfying through testing
            if (distance(f->getPosition(), p) <= 7) return false;
        }
    }

    double flowerSize (getAppConfig().flower_manual_size);
    double pollen (uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max));
    auto toAdd (new Flower(p, flowerSize, pollen));

    // Checking if the flower collides with any hive
    if (!hives_.empty()) {
        for (auto const& h: hives_) {
            if (*h > *toAdd) {
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
    if (world_.isGrowable(position) and world_.getHumidity(position) > 0 and (int)flowers_.size() < getAppConfig().max_flowers) color = sf::Color::Green;
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
//    double randomSize (uniform(getAppConfig().hive_min_size, getAppConfig().hive_max_size));
    double hiveSize (getAppConfig().hive_manual_size);
    double hiveDrawSize (getAppConfig().hive_max_size);
    if (position.x() < 0 or position.x() >= getSize() or position.y() < 0 or position.y() >= getSize()) return false;
    else {
        if (isHiveable(position, hiveSize) and world_.isHiveable(position, hiveSize)) {
            hives_.push_back(new Hive (position, hiveDrawSize));
            return true;
        } else return false;
    }
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
    if ((position.x() < 0 or position.x() >= getSize()) or (position.y() < 0 or position.y() >= getSize()));
    else {
        double size (getAppConfig().hive_manual_size);
//        double size (uniform(getAppConfig().hive_manual_size, getAppConfig().hive_max_size));
        double extRadius (size*getAppConfig().hiveable_factor);
        Vec2d temp (extRadius, extRadius), topLeft (position - temp), bottomRight (position + temp);
        clamping(topLeft);
        clamping(bottomRight);

        sf::Color color (sf::Color::Green);
        if (!world_.isHiveable(position, size)) color = sf::Color::Red;
        if (!isHiveable(position, size)) color = sf::Color::Blue;

        if (topLeft.y() < bottomRight.y()) {
            if (topLeft.x() < bottomRight.x()) { // case 1
                sf::RectangleShape shape(buildRectangle(topLeft, bottomRight, color, 4.0));
                target.draw(shape);
            } else { // case 3
                sf::RectangleShape shape1(buildRectangle(Vec2d(0, topLeft.y()), bottomRight, color, 4.0));
                sf::RectangleShape shape2(buildRectangle(topLeft, Vec2d(getSize(), bottomRight.y()), color, 4.0));
                target.draw(shape1);
                target.draw(shape2);
            }
        } else {
            if (topLeft.x() < bottomRight.x()) { // case 4
                sf::RectangleShape shape1(buildRectangle(Vec2d(topLeft.x(), 0), bottomRight, color, 4.0));
                sf::RectangleShape shape2(buildRectangle(topLeft, Vec2d(bottomRight.x(), getSize()), color, 4.0));
                target.draw(shape1);
                target.draw(shape2);
            } else { // case 2
                sf::RectangleShape shape1(buildRectangle(Vec2d(0,0), bottomRight, color, 4.0));
                sf::RectangleShape shape2(buildRectangle(Vec2d(topLeft.x(), 0), Vec2d(getSize(), bottomRight.y()), color, 4.0));
                sf::RectangleShape shape3(buildRectangle(Vec2d(0, topLeft.y()), Vec2d(bottomRight.x(), getSize()), color, 4.0));
                sf::RectangleShape shape4(buildRectangle(topLeft, Vec2d(getSize(), getSize()), color, 4.0));
                target.draw(shape1);
                target.draw(shape2);
                target.draw(shape3);
                target.draw(shape4);
            }
        }
    }
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

bool Env::isFlyable(const Vec2d &p) const {
    return world_.isFlyable(p);
}

const std::vector<Flower*>* Env::getFlowers() {
    return &flowers_;
}

Bee *Env::getBeeAt(const Vec2d &p) const {
    // TODO to code for extensions
    return nullptr;
}

std::unordered_map<std::string, double> Env::fetchData(const std::string &title) const {
    std::unordered_map<std::string, double> newData;

    if (title == s::GENERAL)
        fetchGeneralData(newData);
    else if (title == s::HIVES)
        fetchHivesData(newData);

    return newData;
}

void Env::fetchGeneralData(std::unordered_map<std::string, double>& map) const {
    double currentFlowers(.0), currentHives(.0), currentWorkerBees(.0), currentScoutBees(.0);

    currentFlowers = (double)(flowers_.size() + newFlowers_.size());
    currentHives = (double)(hives_.size());

    for (auto const& h_ : hives_) {
        currentWorkerBees += h_->getCurrentWorkers();
        currentScoutBees += h_->getCurrentScouts();
    }

    map.insert({s::FLOWERS, currentFlowers});
    map.insert({s::HIVES, currentHives});
    map.insert( {s::WORKERS, currentWorkerBees});
    map.insert({s::SCOUTS, currentScoutBees});
}

std::vector<std::string> Env::getHivesIds() const {
    if (hives_.empty()) {
        return std::vector<std::string> (0);
    } else {
        std::vector<std::string> hivesIds_(hives_.size(), "hive #");

        for (size_t i(0); i < hives_.size(); ++i)
            (hivesIds_.at(i)).append(to_nice_string(i));

        return hivesIds_;
    }
}

void Env::fetchHivesData(std::unordered_map<std::string, double> &map) const {
    if (!hives_.empty()) {
        std::vector<std::string> hivesIds_ (getHivesIds());

        size_t i(0);
        for (auto const& _h : hives_) {
            map.insert({hivesIds_.at(i), _h->getCurrentNectar()});
            ++i;
        }
    }
}
























// end of file
