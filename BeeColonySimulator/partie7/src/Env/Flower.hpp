//
// Created by Léo Bruneau on 29/04/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWER_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWER_HPP

#include "Collider.hpp"
#include "../Utility/Utility.hpp"
#include "Random/Random.hpp"
#include "../Interface/Drawable.hpp"
#include "../Interface/Updatable.hpp"

class Flower : public Collider, public Drawable, public Updatable {
public:

    /*!
     * @brief user defined constructor
     *
     * @param position position of the flower
     * @param size radius of the collider
     * @param pollen amount of pollen (which is also used as life energy)
     */
    Flower(Vec2d position, double size, double pollen);

    /*!
     * @brief removes an amount qte from the attribute pollen_
     *
     * @param qte amount to be removed (because of a worker bee or because of low humidity)
     */
    void takePollen(double qte);

    /*!
     * @brief draws graphical elements to main window
     *
     * @param target main window
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief handles the updating algorithm for the flower
     *
     * @param dt time span
     */
    void update(sf::Time dt) override;

    /*!
     * @brief returns the current amount of pollen of the flower
     */
    double getPollen() const;

    /*!
     *
     * @return
     */
    double getFactor() const override;

    /*!
     * @brief displays informative text about the amount of pollen in a given flower
     *
     * @param target main window
     */
    void showDebugPollen(sf::RenderTarget& target) const;

private:
    double pollen_;
    sf::Texture texture_;
    double sizeFactor_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWER_HPP
