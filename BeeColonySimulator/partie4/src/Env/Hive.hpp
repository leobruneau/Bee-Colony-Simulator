//
// Created by Léo Bruneau on 04/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP4_HIVE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP4_HIVE_HPP
#include "Collider.hpp"
#include "Bee.hpp"

class Hive : public Collider {
public:
private:
    std::vector<Bee*> bees_;
    double nectar_; // = pollen (the two are the same)
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP4_HIVE_HPP
