//
// Created by Léo Bruneau on 10/05/22.
//

#include "ScoutBee.hpp"

ScoutBee::ScoutBee(Hive *homeHive, Vec2d &position, double radius, double energy, double speed)
    : Bee(homeHive,position, radius, energy, speed) {

}

j::Value const &ScoutBee::getConfig() const {
    return getValueConfig()["simulation"]["bees"]["scout"];
}
