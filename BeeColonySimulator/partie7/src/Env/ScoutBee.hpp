//
// Created by Léo Bruneau on 10/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
#include "Bee.hpp"

class ScoutBee : public Bee {
public:

    /*!
     * @brief user defined constructor
     *
     * @param homeHive pointer to the hive
     * @param position position of the collider
     * @param radius size of the collider
     * @param energy energy level of the scout bee
     * @param speed movement speed of the scout bee
     */
    ScoutBee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed);

    /*!
     * @return The configuration of the current bee (with polymorphic behaviour)
     */
    j::Value const& getConfig() const override;

    /*!
     * @brief Method to display debug options of a ScoutBee on the screen
     *
     * @param target main window
     */
    void showSpecificDebugOptions(sf::RenderTarget& target) const override;

    /*!
     * @brief Method modeling the behaviour depending on the current state
     *
     * @param state current state of the bee
     * @param dt time span
     */
    void onState(State const& state, sf::Time const& dt) override;

    /*!
     * @brief Actions that need to be taken when switching state
     *
     * @param state state in which the bee is entering
     */
    void onEnterState(State const& state) override;

    /*!
     * @brief Displays the current state of a ScoutBee when debug mode is ON
     *
     * @param target main window
     */
    void showDebugState(sf::RenderTarget& target) const;

    /*!
     * @brief Displays the current energy of a ScoutBee when debug mode is ON
     *
     * @param target main window
     */
    void showDebugEnergy(sf::RenderTarget& target) const override;

    /*!
     * @brief this method sets memory_ to the position of the flower
     *        (in visibility range) which has the most pollen
     */
    bool findBestFlower();

    /*!
     * @brief simulates interaction with other bees (worker bees in particular)
     *
     * @param other bee
     */
    void interact(Bee* other) override;

    /*!
     * @brief specific interaction algorithm with another scout bee
     *
     * @param scouting a scout bee
     */
    void interactWith(ScoutBee* scouting) override;

    /*!
     * @brief specific interaction algorithm with a worker bee
     *
     * @param working a worker bee
     */
    void interactWith(WorkerBee* working) override;

    /*!
     * @brief checks if the current bee is in the hive or not
     *
     * @return true if it is, false otherwise
     */
    bool isInHive() const override;

    /*!
     * @brief decrease number of scouts (calls method of Env)
     */
    void decreaseNumber() const override;

private:

    /*!
     * @brief possible states of a scout bee
     */
    static const State IN_HIVE;
    static const State FLOWER_QUEST;
    static const State GO_HOME;

    /*!
     * @brief stores amount of the flower she has in memory. Used to memorize the position of the flower with the highest pollen content
     */
    double pollenMemory_;

    /*!
     * @brief number of worker bees the current scout bee has shard a flower's position with
     */
    size_t informedWorkerBees;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_SCOUTBEE_HPP
