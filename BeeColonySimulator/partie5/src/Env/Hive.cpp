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
      sizeFactor_ (getAppConfig().hiveable_factor) {}

void Hive::update(sf::Time dt) {
    for (auto const& b: bees_)
        b->update(dt);
    removeDeadBees();
}

void Hive::drawOn(sf::RenderTarget &targetWindow) const {
    auto hiveSprite = buildSprite(getPosition(), getRadius(), getAppTexture(getAppConfig().hive_texture));
    auto shape = buildAnnulus(getPosition(), getRadius()*getAppConfig().hiveable_factor, sf::Color::Green, 1);

    targetWindow.draw(hiveSprite);

    if (isDebugOn()) {
        showDebugNectar(targetWindow);
        targetWindow.draw(shape);
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
    Vec2d textPosition (getPosition().x(), getPosition().y()+20);
    auto const text = buildText(toWrite, textPosition, getAppFont(), 15, sf::Color::Magenta);
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

    if (bernoulli(scoutProb))
        bee = new ScoutBee(this, position, getAppConfig().scout_size, getAppConfig().scout_initial_energy, getAppConfig().scout_speed);
    else
        bee = new WorkerBee(this, position, getAppConfig().worker_size, getAppConfig().worker_initial_energy, getAppConfig().worker_speed);
    bees_.push_back(bee);
    return bee;
}
