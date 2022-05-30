//
// Created by Léo Bruneau on 04/05/22.
//

#include "Hive.hpp"
#include "Application.hpp"
#include "WorkerBee.hpp"
#include "ScoutBee.hpp"
#include "Utility/Vec2d.hpp"

Hive::Hive(Vec2d const& p, double radius = getAppConfig().hive_manual_size)
    : Collider(p, radius),
      nectar_ (getAppConfig().hive_initial_nectar),
      sizeFactor_ (getAppConfig().hiveable_factor),
      currentWorkers_ (0),
      currentScouts_ (0) {}

void Hive::update(sf::Time dt) {
    for (auto const& b: bees_)
        b->update(dt);

    for (auto const& a : bees_) {
        for (auto const& b : bees_) {
            if (a != b) a->interact(b);
        }
    }

    currentScouts_ = (currentScouts_ > 0) * currentScouts_;
    currentWorkers_ = (currentWorkers_ > 0) * currentWorkers_;

    beeReproduction();
    removeDeadBees();
}

void Hive::drawOn(sf::RenderTarget &targetWindow) const {
    auto hiveSprite = buildSprite(getPosition(), getRadius(), getAppTexture(getAppConfig().hive_texture));
//    auto shape = buildAnnulus(getPosition(), getRadius()*getAppConfig().hiveable_factor, sf::Color::Green, 1);

    targetWindow.draw(hiveSprite);

    if (isDebugOn()) {
        showDebugNectar(targetWindow);
//        targetWindow.draw(shape);
    }

    // Drawing the bees of the Hive
    for (auto const& b: bees_)
        b->drawOn(targetWindow);
}

void Hive::dropPollen(double qte) {
    nectar_ += qte;
}

bool Hive::takeNectar(double qte) {
    if (qte > nectar_) return false;
    else {
        nectar_ -= qte;
        return true;
    }
}

Hive::~Hive() {
    for (auto& b: bees_) {
        delete b;
        b = nullptr;
    }
    bees_.clear();
}

void Hive::showDebugNectar(sf::RenderTarget &target) const {
    std::string toWrite (to_nice_string(nectar_));
    Vec2d textPosition (getPosition().x(), getPosition().y()-20);
    auto const text = buildText(toWrite, textPosition, getAppFont(), 15, sf::Color::Red);
    target.draw(text);
}

bool Hive::isColliding(const Collider &body) const {

    // Remark: if the other "body" is a Hive too, we need to consider getRadius()*getAppConfig().hiveable_factor as its radius
    double radiusSum(getRadius()*getAppConfig().hiveable_factor + body.getRadius()*body.getFactor());
//    if((distanceTo(body)) <= radiusSum) {
//        return true;
//    } else {
//        return false;
//    }
    return ((distanceTo(body)) <= radiusSum);
}

double Hive::getFactor() const {
    return sizeFactor_;
}

void Hive::removeDeadBees() {
    for (auto& b: bees_) {
        if (b->isDead()) {
            b->decreaseNumber();
            delete b;
            b = nullptr;
        }
    }

    bees_.erase(std::remove(bees_.begin(), bees_.end(), nullptr), bees_.end());
}

Bee* Hive::addBee(double scoutProb) {
    double randomRadius (uniform(.0, getRadius()));
    Vec2d randomVector (Vec2d::fromRandomAngle()*randomRadius);
    Vec2d position (getPosition()+randomVector);
    Bee* bee (nullptr);

    if (bernoulli(scoutProb)) {
        bee = new ScoutBee(this, position, getAppConfig().scout_size, getAppConfig().scout_initial_energy,
                           getAppConfig().scout_speed);
        ++currentScouts_;
    } else {
        bee = new WorkerBee(this, position, getAppConfig().worker_size, getAppConfig().worker_initial_energy,
                            getAppConfig().worker_speed);
        ++currentWorkers_;
    }
    bees_.push_back(bee);
    return bee;
}

void Hive::beeReproduction() {
    if (nectar_ > getAppConfig().hive_reproduction_nectar_threshold and (int)bees_.size() < getAppConfig().hive_reproduction_max_bees)
        addBee(getAppConfig().hive_reproduction_scout_proba);
}

int Hive::getCurrentWorkers() const {
    return currentWorkers_;
}

int Hive::getCurrentScouts() const {
    return currentScouts_;
}

void Hive::decreaseWorkers() {
    --currentWorkers_;
}

void Hive::decreaseScouts() {
    --currentScouts_;
}

double Hive::getCurrentNectar() const {
    return nectar_;
}

void Hive::temperatureEffects() {
    for (auto& _b : bees_)
        _b -> temperatureEffects();
}

