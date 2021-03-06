//
// Created by Léo Bruneau on 10/05/22.
//

#include "ScoutBee.hpp"
#include "WorkerBee.hpp"

State const ScoutBee::IN_HIVE = createUid(); // NOLINT(cert-err58-cpp)
State const ScoutBee::FLOWER_QUEST = createUid(); // NOLINT(cert-err58-cpp)
State const ScoutBee::GO_HOME = createUid(); // NOLINT(cert-err58-cpp)

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

        if (getEnergy() > getAppConfig().scout_energy_to_leave_hive) {
            if (getMemory() == nullptr or informedWorkerBees >= 1) nextState();
            else//TODO ;
            ;
        } else {
            double eatingRate (getAppConfig().scout_eating_rate);
            double qte (dt.asSeconds()*eatingRate);
            getHomeHive()->takeNectar(qte);
            setEnergy(getEnergy() + qte);
        }

    } else if (state == FLOWER_QUEST) {

        if (getEnergy() > getAppConfig().scout_energy_seek_flowers) { // increased threshold to avoid having bees with not enough energy to go back
//            if (getMemory() == nullptr) move(dt);
                move(dt);
//            if (findBestFlower()) nextState();
                findBestFlower();
        } else {
            nextState();
            pollenMemory_ = .0;
        }

    } else if (state == GO_HOME) {
        if (*getHomeHive() > *this) nextState();
        else move(dt, getHomeHive()->getPosition());
    }
}

void ScoutBee::onEnterState(const State &state) {
    if (state == IN_HIVE) setCurrentMovement(Movement::Rest);
    if (state == FLOWER_QUEST) {
        setMemory(nullptr);
        informedWorkerBees = 0;
        setCurrentMovement(Movement::Random);
    }
    if (state == GO_HOME) setCurrentMovement(Movement::Target);
}

void ScoutBee::showSpecificDebugOptions(sf::RenderTarget& target) const {
//    showDebugEnergy(target);
//    showDebugState(target);
}

void ScoutBee::showDebugEnergy(sf::RenderTarget &target) const {
    std::string energy(to_nice_string(getEnergy()));
    energy.insert(0, "Scout: energy ");
    Vec2d textPosition(getPosition().x(), getPosition().y() + 20);
    auto const text = buildText(energy, textPosition, getAppFont(), 13, sf::Color::White);
    target.draw(text);
}

void ScoutBee::showDebugState(sf::RenderTarget &target) const {

    std::string debugState(" ");
    State state (getState());

    if (state == IN_HIVE) {

        if (getEnergy() > getAppConfig().scout_energy_to_leave_hive) {

            if (getMemory() == nullptr or informedWorkerBees != 0) debugState = "in_hive_leaving";
            else debugState = "in_hive_sharing[" + to_nice_string((double)informedWorkerBees) + "]";

        } else debugState = "in_hive_eating";

    } else if (state == FLOWER_QUEST) {

        if (getEnergy() > getAppConfig().scout_energy_seek_flowers)
            if (getMemory() == nullptr) debugState = "seeking_flower";

    } else if (state == GO_HOME) {
        debugState = "back_to_hive";
    }

    Vec2d textPosition(getPosition().x(), getPosition().y() + 30);
    auto const text = buildText(debugState, textPosition, getAppFont(), 13, sf::Color::White);
    target.draw(text);
}

bool ScoutBee::findBestFlower() {
    Flower* collidingFlower(getAppEnv().getCollidingFlower(*this));
//    if (collidingFlower != nullptr) {
//        setMemory(getCollidingFlowerPosition(*collidingFlower));
//        return true;
//    } else return false;


    // this algorithm implementation lets the scout bee wonder around, until she attains the energy threshold (that tells her to go home),
    // and let her keep in memory only the position of the flower with the highest pollen content
    if (collidingFlower != nullptr) {
        if (collidingFlower->getPollen() >= pollenMemory_) {
            const Vec2d *colliderPosition(getCollidingFlowerPosition(*collidingFlower));
            setMemory(colliderPosition);
            return true;
        }
    }
    return false;
}

void ScoutBee::interact(Bee *other) { }

void ScoutBee::interactWith(ScoutBee *scouting) { }

void ScoutBee::interactWith(WorkerBee *working) {
    if (getMemory()!= nullptr and (int)informedWorkerBees < getAppConfig().scout_max_sharing) working->learnFlowerLocation(*(this->getMemory()));
    ++informedWorkerBees;
}

bool ScoutBee::isInHive() const {
    return this->getState() == IN_HIVE;
}

void ScoutBee::decreaseNumber() const {
    getHomeHive()->decreaseScouts();
}


