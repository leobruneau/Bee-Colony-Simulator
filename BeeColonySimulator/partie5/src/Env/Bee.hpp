//
// Created by Léo Bruneau on 04/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
#include "Hive.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"
#include "CFSM.hpp"

enum class Movement : short {Rest, Random, Target};

class Bee : public Collider, public Drawable, public Updatable, public CFSM {
public:

    // TODO: finish commenting

    /*!
     * @brief User defined constructor
     *
     */
    Bee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed, States const& states);

    /*!
     * @brief Method for drawing the bees on the screen
     *
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief Update method
     *
     */
    void update(sf::Time dt) override;

    /*!
     * @brief Method that updates energy levels of the bee taking into account its current state
     *
     */
    void updateEnergy(sf::Time const& dt);

    /*!
     * @brief Method that regroups randomMove() and targetMove() to model the different types of movements of a bee
     *
     */
    void move(sf::Time const& dt, Vec2d const& p = {0,0});

    /*!
     * @brief Random movement method
     *
     */
    void randomMove(sf::Time const& dt);

    /*!
     * @brief Movement depending on a specific target (like the home Hive)
     *
     */
    void targetMove(sf::Time const& dt, Vec2d const& p);

    /*!
     *
     * @return
     */
    bool isDead() const;

    /*!
     *
     * @return
     */
    virtual j::Value const& getConfig() const = 0;

    /*!
     *
     * @return
     */
    void showDebugMovement(sf::RenderTarget& target) const;

    /*!
     * @brief
     *
     */
    Hive* getHomeHive() const;

    /*!
     *`@brief Method has been made static because it doesn't depend on existing objects. It was also made static to silence a compiler warning.
     *
     * @return Position of collider passed as a parameter
     */
    static Vec2d const* getCollidingFlowerPosition(Collider const& body);

    /*!
     * @brief
     *
     */
    void setCurrentMovement(Movement movement);

    /*!
     * @brief
     *
     */
    virtual void showSpecificDebugOptions(sf::RenderTarget& target) const = 0;

    /*!
     * @brief
     *
     */
    virtual void showDebugEnergy(sf::RenderTarget& target) const = 0;

    /*!
     * @brief
     *
     */
     virtual void learnFlowerLocation(const Vec2d& flowerPosition);

private:
    Hive* homeHive_;
    Vec2d velocity_;
    Vec2d* currentTarget_;
    Movement currentMovement_;
    sf::Time avoidanceClock_;

protected:
    double energy_;
    Vec2d const* memory_;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
