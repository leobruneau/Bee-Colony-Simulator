//
// Created by Léo Bruneau on 10/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
#include "Bee.hpp"

class ScoutBee : public Bee {
public:

    /*!
     * @brief Scout Bee Constructor
     *
     */
    ScoutBee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed);

    /*!
     * @return The configuration of the current bee (with polymorphic behaviour)
     *
     */
    j::Value const& getConfig() const override;

    /*!
     * @brief Method to display debug options of a ScoutBee on the screen
     *
     */
    void showSpecificDebugOptions(sf::RenderTarget& target) const override;

    /*!
     * @brief Method modeling the behaviour as a function of the current state
     *
     */
    void onState(State const& state, sf::Time const& dt) override;

    /*!
     * @brief Actions that need to be taken when switching state
     *
     */
    void onEnterState(State const& state) override;

    /*!
     * @brief Display the current state of a ScoutBee when debug mode is ON
     *
     */
    void showDebugState(sf::RenderTarget& target) const;

    /*!
     * @brief Display the current energy of a ScoutBee when debug mode is ON
     *
     */
    void showDebugEnergy(sf::RenderTarget& target) const override;

    /*!
     * @brief this method sets memory_ to the position of the flower
     *        (in visibility range) which has the most pollen
     */
    bool findBestFlower();

    void interact(Bee* other) override;

    void interactWith(ScoutBee* scouting) override;

    void interactWith(WorkerBee* working) override;

    bool isInHive() const override;

    void decreaseNumber() const override;

private:
    static const State IN_HIVE;
    static const State FLOWER_QUEST;
    static const State GO_HOME;
    double pollenMemory_;
    size_t informedWorkerBees;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
