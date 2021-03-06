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

class ScoutBee;
class WorkerBee;

class Bee : public Collider, public Drawable, public Updatable, public CFSM {
public:

    /*!
     * @brief User defined constructor
     *
     * @param homeHive is the pointer to the hive to which be bee is bound
     * @param position position of the bee
     * @param radius dimension of the collider
     * @param energy "life" of the bee
     * @param speed speed of the bee
     * @param states states of the bee
     */
    Bee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed, States const& states);

    /*!
     * @brief User defined destructor (which is default)
     */
    virtual ~Bee();

    /*!
     * @brief Method for drawing the bees on the screen
     *
     * @param target target window to which draw the content
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief Update method
     *
     * @param dt time span
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
     * @param p target position (which is then used if this method happens to call the targetMove() method)
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
     * @return true if the energy level of the current bee is <= 0
     */
    bool isDead() const;

    /*!
     *
     * @return configuration relative to the type of bee (polymorphic behaviour)
     */
    virtual j::Value const& getConfig() const = 0;

    /*!
     * @brief displays to the screen a ring around the current bee (either blue or black)
     * depending on the type of movement she is currently in
     */
    void showDebugMovement(sf::RenderTarget& target) const;

    /*!
     *
     * @return the pointer to the hive to which the bee is bound
     */
    Hive* getHomeHive() const;

    /*!
     *
     * @return the energy level of the current bee
     */
    double getEnergy() const;

    /*!
     *
     * @param energy level to which we want to set the energy of the current bee
     */
    void setEnergy(double energy);

    /*!
     * @return a pointer to a constant position of type Vec2d
     */
    const Vec2d* getMemory() const;

    /*!
     *
     * @param position used to update the memory of the current bee
     */
    void setMemory(Vec2d const* position);

    /*!
     *`@brief Method has been made static because it doesn't depend on existing objects. It was also made static to silence a compiler warning.
     *
     * @return Position of collider passed as a parameter
     */
    static Vec2d const* getCollidingFlowerPosition(Collider const& body);

    /*!
     * @brief sets the movement the bee has to follow
     *
     */
    void setCurrentMovement(Movement movement);

    /*!
     * @brief polymorphic method to display type-related debug data
     *
     */
    virtual void showSpecificDebugOptions(sf::RenderTarget& target) const = 0;

    /*!
     * @brief displays the energy level of the bee
     *
     */
    virtual void showDebugEnergy(sf::RenderTarget& target) const = 0;

    /*!
     * @brief updates the memory of the bee
     *
     */
     virtual void learnFlowerLocation(const Vec2d& flowerPosition);

    /*!
    * @brief method to simulate interaction with other bees
    *
    */
     virtual void interact(Bee* other) = 0;

    /*!
    * @brief called inside interact()
    *
    */
    virtual void interactWith(ScoutBee* scouting) = 0;

    /*!
     * @brief called inside interact()
     *
     */
     virtual void interactWith(WorkerBee* working) = 0;

    /*!
    * @brief returns true if the bee has returned to the hive
    *
    */
    virtual bool isInHive() const = 0;

    /*!
    * @brief used to decrease the number of bees (type-specific) inside the hive.
    * Need to see stats from the main program window
    */
    virtual void decreaseNumber() const = 0;



private:
    double energy_;
    const Vec2d* memory_;
    Hive* homeHive_;
    Vec2d velocity_;
    Vec2d* currentTarget_;
    Movement currentMovement_;

    /*!
     * @brief time during which the bee is allowed to switch to random movement to avoid obstacles
     */
    sf::Time avoidanceClock_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
