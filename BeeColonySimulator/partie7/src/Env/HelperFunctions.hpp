//
// Created by Léo Bruneau on 03/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
#include "Utility/Vec2d.hpp"

namespace help {

    /*!
     * @brief set of constants needed for the functions simulating the effects of temperature on the environment
     */
    const double ALPHA (0.4579537);
    const double BETA (0.02364845 / 0.1483637);
    const double GAMMA (0.1483637);
    inline float debugWindRotation(.0);

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


    /*!
     * @brief first mathematical function used to simulate the effects of temperature on the environment
     *
     * @param temp of the environment
     * @return specific influence factor (depends on temperature of course)
     */
    double temperatureFunction1(double temp);

    /*!
     * @brief second mathematical function used to simulate the effects of temperature on the environment
     *
     * @param temp of the environment
     * @return specific influence factor (depends on temperature of course)
     */
    double temperatureFunction2(double temp);

};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_HELPERFUNCTIONS_HPP
