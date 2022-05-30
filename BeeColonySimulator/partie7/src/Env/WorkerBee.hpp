//
// Created by Léo Bruneau on 10/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
#include "Bee.hpp"

class WorkerBee : public Bee {
public:

    /*!
     * @brief user defined constructor
     *
     * @param homeHive pointer to the hive
     * @param position of the worker bee
     * @param radius size of the collider
     * @param energy energy level of the bee
     * @param speed movement speed of the bee
     */
    WorkerBee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed);

    /*!
     * @brief default destructor
     */
    ~WorkerBee() override;

    /*!
     * @brief The configuration of the current bee (with polymorphic behaviour)
     */
    j::Value const& getConfig() const override;

    /*!
     * @brief Method modeling the behaviour depending on the current state
     *
     * @param state current state
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
     * @brief Method to display debug options of a WorkerBee on the screen
     *
     * @param target main window
     */
    void showSpecificDebugOptions(sf::RenderTarget& target) const override;

    /*!
     * @brief Displays the current state of a WorkerBee when debug mode is ON
     *
     * @param target main window
     */
    void showDebugState(sf::RenderTarget& target) const;

    /*!
     * @brief Displays the current energy of a WorkerBee when debug mode is ON
     *
     */
    void showDebugEnergy(sf::RenderTarget& target) const override;

    /*!
     * @brief Displays the current amount of pollen the bee is transporting
     *
     * @param target main window
     */
    void showDebugPollen(sf::RenderTarget& target) const;

    /*!
     * @brief returns pointer to flower at a position p
     *
     * @param p position of the flower
     */
     static Flower* getFlowerAt(Vec2d const& p);

    /*!
     * @brief stores position of a flower in memory_
     *
     * @param flowerPosition position to memorize
     */
    void learnFlowerLocation(const Vec2d& flowerPosition) override;

    /*!
     * @brief models interaction behaviour with other bees
     *
     * @param other bee
     */
    void interact(Bee* other) override;

    /*!
     * @brief specific algorithm for interaction with a scout bee
     *
     * @param scouting scout bee
     */
    void interactWith(ScoutBee* scouting) override;

    /*!
     * @brief specific algorithm for interaction with another worker bee
     *
     * @param working worker bee
     */
    void interactWith(WorkerBee* working) override;

    /*!
     * @return true if the current bee is inside the hive, false otherwise
     */
    bool isInHive() const override;

    /*!
     * @brief decreases number of worker bees (calls method of Env)
     */
    void decreaseNumber() const override;

private:

    double harvestedPollen_;

    /*!
     * @brief possible states of a worker bee
     */
    static const State IN_HIVE;
    static const State TOWARDS_FLOWER;
    static const State NECTAR_HARVESTING;
    static const State GO_HOME;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
