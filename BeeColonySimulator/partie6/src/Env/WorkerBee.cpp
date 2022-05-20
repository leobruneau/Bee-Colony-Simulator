//
// Created by Léo Bruneau on 10/05/22.
//

#include "WorkerBee.hpp"

State const WorkerBee::IN_HIVE = createUid();
State const WorkerBee::TOWARDS_FLOWER = createUid();
State const WorkerBee::NECTAR_HARVESTING = createUid();
State const WorkerBee::GO_HOME = createUid();

WorkerBee::WorkerBee(Hive *homeHive, Vec2d &position, double radius, double energy, double speed)
    : Bee(homeHive,position, radius, energy, speed, {IN_HIVE, TOWARDS_FLOWER, NECTAR_HARVESTING, GO_HOME}), harvestedPollen_(.0) {

}

j::Value const &WorkerBee::getConfig() const {
    return getValueConfig()["simulation"]["bees"]["worker"];
}

void WorkerBee::onState(const State &state, const sf::Time &dt) {
    if (state == IN_HIVE) {

        if (harvestedPollen_ > 0) {
            double pTransRate (getAppConfig().worker_transfer_rate);
            double qte (dt.asSeconds()*pTransRate);
            harvestedPollen_ -= qte;
            getHomeHive()->dropPollen(qte);

            // Use of boolean algebra to set the harvestedPollen_ attribute to 0 if it becomes negative
            harvestedPollen_ = (harvestedPollen_ > 0)*harvestedPollen_;

        } else {
            if (getEnergy() > getAppConfig().worker_energy_to_leave_hive) {
                if (getMemory() != nullptr) nextState();
            } else {
                double eatingRate (getAppConfig().worker_eating_rate);
                double qte (dt.asSeconds()*eatingRate);
                getHomeHive()->takeNectar(qte);
                setEnergy(getEnergy()+qte);
            }
        }
    } else if (state == TOWARDS_FLOWER) {
        if (*this > *getMemory()) nextState();
        move(dt, *getMemory());

    } else if (state == NECTAR_HARVESTING) {
        if (harvestedPollen_ < getAppConfig().worker_pollen_capacity) {
            Flower* targetFlower (getFlowerAt(*getMemory()));

            if (targetFlower != nullptr and targetFlower->getPollen() > 0) {
                double pGatherRate (getAppConfig().worker_harvest_rate);
                double qte (dt.asSeconds()*pGatherRate);
                targetFlower->takePollen(qte);
                harvestedPollen_ += qte;
            }
        } else nextState();

    } else if (state == GO_HOME) {
        if (*getHomeHive() > *this) nextState();
        move(dt, getHomeHive()->getPosition());
    }
}

void WorkerBee::onEnterState(const State &state) {
    if (state == IN_HIVE) setCurrentMovement(Movement::Rest);
    if (state == TOWARDS_FLOWER) setCurrentMovement(Movement::Target);
    if (state == NECTAR_HARVESTING) setCurrentMovement(Movement::Rest);
    if (state == GO_HOME) {
        setCurrentMovement(Movement::Target);
        setMemory(nullptr);
    }
}

void WorkerBee::showSpecificDebugOptions(sf::RenderTarget &target) const {
    showDebugEnergy(target);
    showDebugPollen(target);
    showDebugState(target);
}

Flower *WorkerBee::getFlowerAt(const Vec2d &p) {
    std::vector<Flower*> const* flowers (getAppEnv().getFlowers());
    for (auto& f: *flowers) {
        if (f->getPosition() == p) return f;
    }
    return nullptr;
}

void WorkerBee::learnFlowerLocation(const Vec2d &flowerPosition) {
    setMemory(&flowerPosition);
}

void WorkerBee::showDebugState(sf::RenderTarget &target) const {
    std::string debugState(" ");
    State state (getState());
    if (state == IN_HIVE) {
        if (harvestedPollen_ > 0) debugState = "in_hive_pollen";
        else if (getEnergy() < getAppConfig().worker_energy_to_leave_hive) {
            if (getMemory() != nullptr) debugState = "in_hive_???";
            else debugState = "in_hive_eating";
        }
        else if (getMemory() == nullptr) debugState = "in_hive_no_flower";

    } else if (state == TOWARDS_FLOWER) {
        debugState = "to_flower";

    } else if (state == NECTAR_HARVESTING) {
        debugState = "collecting_pollen";

    } else if (state == GO_HOME) {
        debugState = "back_to_hive";

    }

    Vec2d textPosition(getPosition().x(), getPosition().y() + 40);
    auto const text = buildText(debugState, textPosition, getAppFont(), 13, sf::Color::White);
    target.draw(text);
}

void WorkerBee::showDebugEnergy(sf::RenderTarget &target) const {
    std::string energy(to_nice_string(getEnergy()));
    energy.insert(0, "Worker: energy ");
    Vec2d textPosition(getPosition().x(), getPosition().y() + 20);
    auto const text = buildText(energy, textPosition, getAppFont(), 13, sf::Color::White);
    target.draw(text);
}

void WorkerBee::showDebugPollen(sf::RenderTarget &target) const {
    std::string pollen(to_nice_string(harvestedPollen_));
    pollen.insert(0, "Worker: pollen ");
    Vec2d textPosition(getPosition().x(), getPosition().y() + 30);
    auto const text = buildText(pollen, textPosition, getAppFont(), 13, sf::Color::White);
    target.draw(text);
}

void WorkerBee::interact(Bee *other) {
    if (this->isInHive() and other->isInHive())
        other->interactWith(this);
}

void WorkerBee::interactWith(ScoutBee *scouting) { }

void WorkerBee::interactWith(WorkerBee *working) { }

bool WorkerBee::isInHive() const {
    return this->getState() == IN_HIVE;
}
