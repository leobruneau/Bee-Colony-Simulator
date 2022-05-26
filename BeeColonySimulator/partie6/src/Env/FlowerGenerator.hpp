//
// Created by Léo Bruneau on 04/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWERGENERATOR_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWERGENERATOR_HPP
#include "../Interface/Updatable.hpp"

class FlowerGenerator : public Updatable {
public:
    FlowerGenerator();
    void update(sf::Time dt) override;
    void reset();

private:
    sf::Time elapsed_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWERGENERATOR_HPP
