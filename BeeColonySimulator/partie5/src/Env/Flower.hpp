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
    Flower(Vec2d position, double size, double pollen);
    void takePollen(double qte);
    void drawOn(sf::RenderTarget& target) const override;
    void update(sf::Time dt) override;
    double getPollen() const;
    double getFactor() const override;
    void showDebugPollen(sf::RenderTarget& target) const;

private:
    double pollen_;
    sf::Texture texture_;
    double sizeFactor_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_FLOWER_HPP
