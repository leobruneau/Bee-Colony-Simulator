//
// Created by Léo Bruneau on 03/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
#include "Utility/Vec2d.hpp"

namespace help {

    /*!
     * @param p position of cursor
     * @param size dimensions of the environment
     * @param nb number of cells in a row of the world cache
     *
     * @return the index of the cell that contains the position p
     */
    int getIndex(Vec2d const& p, float const& size, int const& nb);

    /*!
     *
     * @param p position of cursor
     * @param size dimensions of the environment
     *
     * @return x coordinate, on the plane, of the cell containing p
     */
    int getX(Vec2d const& p, float const& size);

    /*!
     *
     * @param p position of cursor
     * @param size dimensions of the environment
     *
     * @return y coordinate, on the plane, of the cell containing p
     */
    int getY(Vec2d const& p, float const& size);

};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
