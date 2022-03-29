//
// Created by Léo Bruneau on 29/03/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#include <vector>

enum class Kind : short {Grass, Water, Rocks};

class World {
public:
    void reloadConfig();

private:
    int nbCells_;
    float cellSize_;
    std::vector<Kind> cells_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP