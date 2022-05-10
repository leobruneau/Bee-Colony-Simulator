//
// Created by Léo Bruneau on 10/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
#include "Bee.hpp"

class WorkerBee : public Bee {
public:
    WorkerBee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed);
    j::Value const& getConfig() const override;

private:

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
