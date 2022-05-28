//
// Created by Léo Bruneau on 28/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOGGENERATOR_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOGGENERATOR_HPP

#include "Interface/Updatable.hpp"

class FogGenerator : public Updatable {

public:
    FogGenerator();

    /*!
     * @brief avoid copies (there can be only one fog generator per weather (i.e. per environment))
     *
     * @param other other FogGenerator object (not) to copy
     */
    FogGenerator(const FogGenerator& other) = delete;

    /*!
     * @brief avoid copies (there can be only one fog generator per weather (i.e. per environment))
     *
     * @param other other FogGenerator object (not) to copy
     */
    FogGenerator& operator=(const FogGenerator& other) = delete;

    /*!
     * @brief handles evolution of the FogGenerator object
     *
     * @param dt time span
     */
    void update(sf::Time dt) override;


private:
    sf::Time _elapsed;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOGGENERATOR_HPP
