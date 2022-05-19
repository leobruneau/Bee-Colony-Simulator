//
// Created by Léo Bruneau on 10/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
#include "Bee.hpp"

class WorkerBee : public Bee {
public:

    /*!
     * @brief
     *
     */
    WorkerBee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed);

    /*!
     * @brief
     *
     */
    j::Value const& getConfig() const override;

    /*!
     * @brief
     *
     */
    void onState(State const& state, sf::Time const& dt) override;

    /*!
     * @brief
     *
     */
    void onEnterState(State const& state) override;

    /*!
     * @brief
     *
     */
    void showSpecificDebugOptions(sf::RenderTarget& target) const override;

    /*!
     * @brief
     *
     */
    void showDebugState(sf::RenderTarget& target) const;

    /*!
     * @brief
     *
     */
    void showDebugEnergy(sf::RenderTarget& target) const override;

    /*!
     * @brief
     *
     */
    void showDebugPollen(sf::RenderTarget& target) const;

    /*!
     * @brief
     *
     */
     static Flower* getFlowerAt(Vec2d const& p);

    /*!
    * @brief
    *
    */
    void learnFlowerLocation(const Vec2d& flowerPosition) override;

    void interact(Bee* other) override;

    void interactWith(ScoutBee* scouting) override;

    void interactWith(WorkerBee* working) override;

    bool isInHive() const override;

private:

    double harvestedPollen_;
    static const State IN_HIVE;
    static const State TOWARDS_FLOWER;
    static const State NECTAR_HARVESTING;
    static const State GO_HOME;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_WORKERBEE_HPP
