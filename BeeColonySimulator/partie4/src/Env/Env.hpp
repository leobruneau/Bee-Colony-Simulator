//
// Created by Léo Bruneau on 12/04/22.
//
#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP

#include "World.hpp"
#include "Flower.hpp"
#include "../Interface/Updatable.hpp"
#include "FlowerGenerator.hpp"
#include "Hive.hpp"

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
    double getCellHumidity(Vec2d const& position);
    void removeDeadFlowers();
    bool addHiveAt(const Vec2d& position);
    void hiveDestroyer();
    Hive* getCollidingHive(const Collider& body);
    Flower* getCollidingFlower(const Collider& body);

private:
    World world_;
    std::vector<Flower*> flowers_;
    std::vector<Flower*> newFlowers_;
    std::vector<Hive*> hives_;
    FlowerGenerator flowerGenerator_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
