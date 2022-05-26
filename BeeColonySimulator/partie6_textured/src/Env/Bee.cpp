//
// Created by Léo Bruneau on 04/05/22.
//

#include "Bee.hpp"

Bee::Bee(Hive* homeHive, Vec2d &position, double radius, double energy, double speed, States const& states)
    : Collider(position, radius), CFSM(states),
    energy_(energy),
    memory_(nullptr),
    homeHive_(homeHive),
    velocity_(Vec2d::fromRandomAngle()*speed),
    currentTarget_(nullptr),
    currentMovement_(Movement::Rest) {
    // Constructor's body
}

void Bee::drawOn(sf::RenderTarget &target) const {
    auto beeSprite(buildSprite(getPosition(), getRadius(), getAppTexture(getConfig()["texture"].toString())));

    if (velocity_.angle() >= PI/2 or velocity_.angle() <= -PI/2) beeSprite.scale(1, -1);
    beeSprite.rotate(velocity_.angle()/DEG_TO_RAD);

    if (isDebugOn()) {
        showDebugMovement(target);
        this->showSpecificDebugOptions(target);
    }

    target.draw(beeSprite);
}

void Bee::update(sf::Time dt) {
    this->action(dt);
    updateEnergy(dt);
}

void Bee::randomMove(sf::Time const& dt) {
    double p (getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
    double alpha_max (getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
    double alpha;

    if (bernoulli(p)) {
        alpha = uniform(-alpha_max, alpha_max);
        velocity_.rotate(alpha);
    }

    auto dx = velocity_*dt.asSeconds();
    auto newPosition (getPosition() + dx);
    clamping(newPosition);

    if (getAppEnv().isFlyable(newPosition)) Collider::move(velocity_*dt.asSeconds());
    else {
        double beta(-PI/4);
        if (bernoulli(0.5)) beta = PI/4;
        velocity_.rotate(beta);
    }
}

void Bee::move(const sf::Time &dt, Vec2d const& p) {
    if (currentMovement_ == Movement::Random) randomMove(dt);
    else if (currentMovement_ == Movement::Target) targetMove(dt, p);
}

bool Bee::isDead() const {
//    if (energy_ == 0) return true;
//    else return false;
    return (energy_ == 0);
}

void Bee::targetMove(const sf::Time &dt, Vec2d const& p) {

    if (avoidanceClock_ <= sf::Time::Zero) {
        avoidanceClock_ = sf::Time::Zero;
        Vec2d directionToTarget (directionTo(p));
        double length(directionToTarget.length());
        Vec2d unitToTarget (directionToTarget.x()/length, directionToTarget.y()/length);
        velocity_ = velocity_.length()*unitToTarget;

    } else avoidanceClock_ -= dt;

    auto dx = velocity_*dt.asSeconds();
    auto newPosition (getPosition() + dx);
    clamping(newPosition);

    if (getAppEnv().isFlyable(newPosition)) Collider::move(velocity_*dt.asSeconds());
    else {
        avoidanceClock_ = sf::seconds((float)getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble());

        double beta(-PI/4);
        if (bernoulli(0.5)) beta = PI/4;

        velocity_.rotate(beta);
    }
}

void Bee::updateEnergy(sf::Time const& dt) {
    double idle (this->getConfig()["energy"]["consumption rates"]["idle"].toDouble());
    double moving (this->getConfig()["energy"]["consumption rates"]["moving"].toDouble());

    // branch-less programming implementation
    energy_ -= dt.asSeconds()*idle*(currentMovement_ == Movement::Rest)
            +  dt.asSeconds()*moving*(currentMovement_ == Movement::Target or currentMovement_ == Movement::Random);
    if (energy_ <= 0) energy_ = 0;
}

void Bee::showDebugMovement(sf::RenderTarget& target) const {
    sf::Color color (sf::Color::Black);
    unsigned int thickness (0);

    if (currentMovement_ == Movement::Random) thickness = 5;
    else if (currentMovement_ == Movement::Target) {
        color = sf::Color::Blue;
        thickness = 3;
    }

    auto shape = buildAnnulus(getPosition(), getRadius()*getAppConfig().hiveable_factor, color, thickness);
    target.draw(shape);
}

Hive* Bee::getHomeHive() const {
    return homeHive_;
}

Vec2d const* Bee::getCollidingFlowerPosition(const Collider &body) {
    return &body.getPosition();
}

void Bee::setCurrentMovement(Movement movement) {
    currentMovement_ = movement;
}

// To ignore the "unused parameter" warning
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void Bee::learnFlowerLocation(const Vec2d &flowerPosition) { }
#pragma clang diagnostic pop

double Bee::getEnergy() const {
    return energy_;
}

void Bee::setEnergy(double energy) {
    energy_ = energy;
}


Vec2d const* Bee::getMemory() const {
    return memory_;
}

void Bee::setMemory(Vec2d const* position) {
    memory_ = position;
}

Bee::~Bee() {

}
