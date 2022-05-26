//
// Created by Léo Bruneau on 03/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
#include "Utility/Vec2d.hpp"

namespace help {

    int getIndex(Vec2d const& p, float const& size, int const& nb);
    int getX(Vec2d const& p, float const& size);
    int getY(Vec2d const& p, float const& size);

};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
