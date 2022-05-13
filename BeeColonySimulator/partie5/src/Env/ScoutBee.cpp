//
// Created by Léo Bruneau on 10/05/22.
//

#include "ScoutBee.hpp"

State const ScoutBee::IN_HIVE = createUid();

ScoutBee::ScoutBee(Hive *homeHive, Vec2d &position, double radius, double energy, double speed)
    : Bee(homeHive,position, radius, energy, speed, {IN_HIVE}) {

}

j::Value const &ScoutBee::getConfig() const {
    return getValueConfig()["simulation"]["bees"]["scout"];
}
