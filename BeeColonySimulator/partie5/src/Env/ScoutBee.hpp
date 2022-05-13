//
// Created by Léo Bruneau on 10/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
#include "Bee.hpp"

class ScoutBee : public Bee {
public:
    ScoutBee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed);
    j::Value const& getConfig() const override;

private:
    static const State IN_HIVE;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
