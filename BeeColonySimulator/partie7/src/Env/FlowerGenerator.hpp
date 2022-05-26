//
// Created by Léo Bruneau on 04/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWERGENERATOR_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWERGENERATOR_HPP
#include "../Interface/Updatable.hpp"

class FlowerGenerator : public Updatable {
public:

    /*!
     * @brief user defined default constructor
     */
    FlowerGenerator();

    /*!
     * @brief update method for the random generator
     *
     * @param dt time span
     */
    void update(sf::Time dt) override;

    /*!
     * @brief reset method for the generator
     */
    void reset();

private:

    /*!
     * @brief time span between one random generation and the previous (or the next)
     */
    sf::Time elapsed_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWERGENERATOR_HPP
