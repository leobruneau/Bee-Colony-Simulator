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
    Hive(Vec2d const& p, double radius);
    Hive(Hive const& source) = delete;
    Hive& operator=(Hive const& source) = delete;
    ~Hive() override;
    void update(sf::Time dt) override;
    void drawOn(sf::RenderTarget& targetWindow) const override;
    bool isColliding(const Collider& body) const override;
    void dropPollen(double qte);
    bool takeNectar(double qte);
    void showDebugNectar(sf::RenderTarget& target) const;
    double getFactor() const override;
    void removeDeadBees();

protected:
    Bee* addBee(double scoutProb = 0);

private:
    std::vector<Bee*> bees_;
    double nectar_; // = pollen (the two are the same)
    double sizeFactor_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP4_HIVE_HPP
