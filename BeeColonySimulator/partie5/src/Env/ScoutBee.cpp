//
// Created by Léo Bruneau on 10/05/22.
//

#include "ScoutBee.hpp"

State const ScoutBee::IN_HIVE = createUid();
State const ScoutBee::FLOWER_QUEST = createUid();
State const ScoutBee::GO_HOME = createUid();

ScoutBee::ScoutBee(Hive *homeHive, Vec2d &position, double radius, double energy, double speed)
    : Bee(homeHive,position, radius, energy, speed, {IN_HIVE, FLOWER_QUEST, GO_HOME}),
    pollenMemory_(.0),
    informedWorkerBees(0) {
    // constructor body
}

j::Value const &ScoutBee::getConfig() const {
    return getValueConfig()["simulation"]["bees"]["scout"];
}

void ScoutBee::onState(const State &state, const sf::Time &dt) {
    if (state == IN_HIVE) {

        if (energy_ > getAppConfig().scout_energy_to_leave_hive) {
            if (memory_ == nullptr or informedWorkerBees >= 1) nextState();
            else informWorkerBees();
        } else {
            double eatingRate (getAppConfig().scout_eating_rate);
            double qte (dt.asSeconds()*eatingRate);
            getHomeHive()->takeNectar(qte);
            energy_ += qte;
        }

    } else if (state == FLOWER_QUEST) {

        if (energy_ > getAppConfig().scout_energy_seek_flowers) {
            if (memory_ == nullptr) move(dt);
            if (findBestFlower()) nextState();
        } else nextState();

    } else if (state == GO_HOME) {
        if (*getHomeHive() > *this) nextState();
        else move(dt, getHomeHive()->getPosition());
    }
}

void ScoutBee::onEnterState(const State &state) {
    if (state == IN_HIVE) setCurrentMovement(Movement::Rest);
    if (state == FLOWER_QUEST) {
        memory_ = nullptr;
        setCurrentMovement(Movement::Random);
    }
    if (state == GO_HOME) setCurrentMovement(Movement::Target);
}

void ScoutBee::showSpecificDebugOptions(sf::RenderTarget& target) const {
    showDebugEnergy(target);
    showDebugState(target);
}

void ScoutBee::showDebugEnergy(sf::RenderTarget &target) const {
    std::string energy(to_nice_string(energy_));
    energy.insert(0, "Scout: energy ");
    Vec2d textPosition(getPosition().x(), getPosition().y() + 20);
    auto const text = buildText(energy, textPosition, getAppFont(), 13, sf::Color::White);
    target.draw(text);
}

void ScoutBee::showDebugState(sf::RenderTarget &target) const {

    std::string debugState(" ");
    State state (getState());

    if (state == IN_HIVE) {

        if (energy_ > getAppConfig().scout_energy_to_leave_hive) {

            if (memory_ == nullptr or informedWorkerBees != 0) debugState = "in_hive_leaving";
            else debugState = "in_hive_sharing[" + to_nice_string((double)informedWorkerBees) + "]";

        } else debugState = "in_hive_eating";

    } else if (state == FLOWER_QUEST) {

        if (energy_ > getAppConfig().scout_energy_seek_flowers)
            if (memory_ == nullptr) debugState = "seeking_flower";

    } else if (state == GO_HOME) {
        debugState = "back_to_hive";
    }

    Vec2d textPosition(getPosition().x(), getPosition().y() + 30);
    auto const text = buildText(debugState, textPosition, getAppFont(), 13, sf::Color::White);
    target.draw(text);
}

void ScoutBee::informWorkerBees() {
    // TODO: to code
}

bool ScoutBee::findBestFlower() {
    Flower* collidingFlower(getAppEnv().getCollidingFlower(*this));
    if (collidingFlower != nullptr) {
        memory_ = getCollidingFlowerPosition(*collidingFlower);
        return true;
    } else return false;


// TODO TO IMPLEMENT ALGORITHM TO KEEP MEMORY ONLY OF THE BEST FLOWER

//    if (collidingFlower != nullptr and memory_ != nullptr) {
//        if (collidingFlower->getPollen() >= pollenMemory_) {
//            const Vec2d *colliderPosition(getCollidingFlowerPosition(*collidingFlower));
//            memory_ = colliderPosition;
//            return true;
//        }
//    }
//    return false;
}



