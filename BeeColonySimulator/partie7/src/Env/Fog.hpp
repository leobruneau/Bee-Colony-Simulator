//
// Created by Léo Bruneau on 27/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOG_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOG_HPP
#include "../Interface/Updatable.hpp"
#include "../Interface/Drawable.hpp"
#include "../Utility/Utility.hpp"
#include "Collider.hpp"

class Fog : public Updatable, public Drawable, public Collider {
public:

    /*!
     * @brief user defined specific constructor
     *
     * @param position position of the fog cloud (of the collider)
     * @param radius size of the collider
     * @param density transparency level of the cloud
     */
    Fog(Vec2d position, double radius, double density = .0);

    /*!
     * @brief user defined destructor
     */
    ~Fog() override;

    /*!
     * @brief handles the evolution algorithm for the cloud
     *
     * @param dt time span
     */
    void update(sf::Time dt) override;

    /*!
     * @brief displays the fog sprite to the main window
     *
     * @param target main window
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief displays debug information to main window
     *
     * @param target main window
     */
    void showDebugInfo(sf::RenderTarget& target) const;

    /*!
     * @brief handles the passive movement of the cloud (depends on the wind in the environment)
     *
     * @param dt time span
     */
    void passiveMovement(sf::Time dt);

    /*!
     * @return true if density attribute is below a critical threshold (defined inside .json file), false otherwise
     */
    bool isDead() const;

private:

    /*!
     * @brief level of transparency of the cloud of fog (should go from .0 to 255.0)
     */
    double _density;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOG_HPP
