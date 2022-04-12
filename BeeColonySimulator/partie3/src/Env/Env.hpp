//
// Created by Léo Bruneau on 12/04/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
#include "World.hpp"

class Env {
public:
    Env();
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& target);
    void loadWorldFromFile();
    void reset();
    float getSize();
    void resetControls();
private:
    World world_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
