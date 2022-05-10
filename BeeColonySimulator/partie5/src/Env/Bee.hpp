//
// Created by Léo Bruneau on 04/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
#include "Hive.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"

class Bee : public Collider, public Drawable, public Updatable {
public:
    Bee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed);
    void drawOn(sf::RenderTarget& target) const override;
    void update(sf::Time dt) override;
    void move(sf::Time dt);
    bool isDead() const;
    virtual j::Value const& getConfig() const = 0;

private:
    Hive* homeHive_;
    Vec2d velocity_;
    double energy_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
