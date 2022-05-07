//
// Created by Léo Bruneau on 04/05/22.
//

#include "Hive.hpp"
#include "Application.hpp"
#include "Bee.hpp"

Hive::Hive(Vec2d const& p, double radius = getAppConfig().hive_manual_size)
    : Collider(p, radius),
      nectar_ (getAppConfig().hive_initial_nectar),
      sizeFactor_ (getAppConfig().hiveable_factor) {}

void Hive::addBee() {
    Vec2d position (getPosition());
    bees_.push_back(new Bee(this, position, 10., 10., 10.));
}

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
    if((distanceTo(body)) <= radiusSum) {
        return true;
    } else {
        return false;
    }
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
