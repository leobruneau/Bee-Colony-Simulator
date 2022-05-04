//
// Created by Léo Bruneau on 12/04/22.
//
#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP

#include "World.hpp"
#include "Flower.hpp"
#include "../Interface/Updatable.hpp"
#include "FlowerGenerator.hpp"

class Env : public Drawable, public Updatable {
public:
    Env();
    ~Env() override;
    Env(const Env&) = delete;
    void update(sf::Time dt) override;
    void drawOn(sf::RenderTarget& target) const override;
    void loadWorldFromFile();
    void saveWorldToFile() const;
    void reset();
    float getSize();
    void resetControls();
    bool addFlowerAt(Vec2d const& p, bool split = false);
    void drawFlowerZone(sf::RenderTarget& target, Vec2d const& position);
    void flowerDestroyer();
    double getPixelHumidity(Vec2d const& position);
    void removeDeadFlowers();

private:
    World world_;
    std::vector<Flower*> flowers_;
    std::vector<Flower*> newFlowers_;
    FlowerGenerator flowerGenerator_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
