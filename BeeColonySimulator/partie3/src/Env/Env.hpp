//
// Created by Léo Bruneau on 12/04/22.
//
#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP

#include "World.hpp"
#include "Flower.hpp"

class Env {
public:
    Env();
    ~Env();
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& target);
    void loadWorldFromFile();
    void reset();
    float getSize();
    void resetControls();
    bool addFlowerAt(Vec2d const& p);
    void drawFlowerZone(sf::RenderTarget& target, Vec2d const& position);
    void flowerDestroyer();
private:
    World world_;
    std::vector<Flower*> flowers_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
