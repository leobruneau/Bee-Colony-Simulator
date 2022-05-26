//
// Created by Léo Bruneau on 04/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP4_HIVE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP4_HIVE_HPP
#include "Collider.hpp"
#include "../Interface/Updatable.hpp"
#include "../Interface/Drawable.hpp"

class Bee;

class Hive : public Collider, public Updatable, public Drawable {
public:

    /*!
     * @brief user defined constructor
     *
     * @param p position of the hive
     * @param radius radius of the collider
     */
    Hive(Vec2d const& p, double radius);

    /*!
     * @brief deleted copy-constructor to avoid unintentional copies of a "big" class
     *
     * @param source hive we (don't) want to copy
     */
    Hive(Hive const& source) = delete;

    /*!
     * @brief deleted overloaded operator= to avoid copies
     *
     * @param source hive we (don't) want to copy
     */
    Hive& operator=(Hive const& source) = delete;

    /*!
     * @brief user defined destructor
     */
    ~Hive() override;

    /*!
     * @brief updates the hive
     *
     * @param dt time span
     */
    void update(sf::Time dt) override;

    /*!
     * @brief draws graphical components to main window
     *
     * @param targetWindow main window
     */
    void drawOn(sf::RenderTarget& targetWindow) const override;

    /*!
     * @brief checks the collision of the current hive with body
     *
     * @param body with which we test the collision
     *
     * @return true if there is a collision, false otherwise
     */
    bool isColliding(const Collider& body) const override;

    /*!
     * @brief method that increases nectar_ by qte
     *
     * @param qte amount to add
     */
    void dropPollen(double qte);

    /*!
     * @brief removes an amount qte from nectar_
     *
     * @param qte amount to remove
     *
     * @return true if the amount was successfully removed (i.e. if at removal qte <= nectar_)
     */
    bool takeNectar(double qte);

    /*!
     * @brief displays above the current hive the nectar_ level
     *
     * @param main window
     */
    void showDebugNectar(sf::RenderTarget& target) const;

    /*!
     * @return factor needed to specifically increment the collider's radius
     */
    double getFactor() const override;

    /*!
     * @brief deallocates memory of bees whose energy has reached 0 (i.e. are dead)
     */
    void removeDeadBees();

    /*!
     * @brief simulates the proliferation inside the bee colony
     */
    void beeReproduction();

    /*!
     * @return current number of worker bees in the hive
     */
    int getCurrentWorkers() const;

    /*!
     * @return current number of scout bees in the hive
     */
    int getCurrentScouts() const;

    /*!
     * @brief decreases the number of worker bees by 1
     */
    void decreaseWorkers();

    /*!
     * @brief decreases the number of scout bees by 1
     */
    void decreaseScouts();

    /*!
     * @return current amount of nectar
     */
    double getCurrentNectar() const;

protected:

    /*!
     * @brief adds a bee to the colony
     * @param scoutProb probability of the new bee being a scout
     * @return pointer to the newly added bee
     */
    Bee* addBee(double scoutProb = 0);

private:
    std::vector<Bee*> bees_;
    double nectar_; // = pollen (the two are the same)
    double sizeFactor_;
    int currentWorkers_;
    int currentScouts_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP4_HIVE_HPP
