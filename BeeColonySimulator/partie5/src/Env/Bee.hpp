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
    Bee(Hive* homeHive, Vec2d& position, double radius, double energy, double speed, States const& states);
    void drawOn(sf::RenderTarget& target) const override;
    void update(sf::Time dt) override;
    void updateEnergy(sf::Time const& dt);
    void move(sf::Time const& dt);
    void randomMove(sf::Time const& dt);
    void targetMove(sf::Time const& dt);
    bool isDead() const;
    virtual j::Value const& getConfig() const = 0;
    void showDebugMovement(sf::RenderTarget& target) const;

private:
    Hive* homeHive_;
    Vec2d velocity_;
    double energy_;
    Vec2d* memory_;
    Vec2d* currentTarget_;
    Movement currentMovement_;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP4_BEE_HPP
