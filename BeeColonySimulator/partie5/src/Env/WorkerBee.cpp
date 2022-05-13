//
// Created by Léo Bruneau on 10/05/22.
//

#include "WorkerBee.hpp"

State const WorkerBee::IN_HIVE = createUid();

WorkerBee::WorkerBee(Hive *homeHive, Vec2d &position, double radius, double energy, double speed)
    : Bee(homeHive,position, radius, energy, speed) {

}

j::Value const &WorkerBee::getConfig() const {
    return getValueConfig()["simulation"]["bees"]["worker"];
}
