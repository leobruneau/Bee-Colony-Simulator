//
// Created by Léo Bruneau on 29/04/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWER_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWER_HPP

#include "Collider.hpp"
#include "../Utility/Utility.hpp"
#include "Random/Random.hpp"

class Flower : public Collider {
public:
    Flower(Vec2d position, double size, double pollen);
    double takePollen();
    void drawOn(sf::RenderTarget& target) const;
private:
    double pollen_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWER_HPP
