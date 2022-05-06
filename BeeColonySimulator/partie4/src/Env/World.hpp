//
// Created by Léo Bruneau on 29/03/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>
#include "Utility/Vec2d.hpp"
#include "../Interface/Drawable.hpp"


enum class Kind : short {Grass, Water, Rocks};

// North: 0, East: 1, South: 2, West: 3 - used to define the direction of propagation of seeds
enum class Direction : short {North, East, South, West};

struct Seed {
    sf::Vector2i coordinates_;
    Kind seedNature_;
};

class World : public Drawable {
public:
    World() = default;
    World(const World&) = delete;
    void reloadConfig();
    void drawOn(sf::RenderTarget& target) const override;
    void updateCache();
    void reloadCacheStructure();
    void reset(bool const& regenerate);
    float getSize() const;
    float getCellSize() const;
    void loadFromFile();
    void moveSeed(Seed &seed, int &ind) const;
    void step();
    void steps(int nb, bool update = false);
    void smooth();
    void smooths(int nb, bool update = false);
    std::vector<int>findViableCells(int x) const;
    bool isViable(int index, int x) const;
    void saveToFile() const;
    void setHumidity(double eta, double lambda);
    void showDebugHumidity(sf::RenderTarget &target) const;
    bool isGrowable(const Vec2d& p) const;
    double getHumidity(Vec2d const& p) const;
    bool isHiveable(const Vec2d& p, double radius) const;
    std::vector<std::size_t> indexesForRect(sf::Vector2i const& topLeft, sf::Vector2i const& bottomRight) const;

private:
    int nbCells_;
    float cellSize_;
    std::vector<Kind> cells_;
    std::vector<sf::Vertex> grassVertexes_;
    std::vector<sf::Vertex> waterVertexes_;
    std::vector<sf::Vertex> rockVertexes_;
    std::vector<sf::Vertex> humidityVertexes_;
    sf::RenderTexture renderingCache_;
    sf::RenderTexture renderingHumidity_;
    std::vector<Seed> seeds_;
    unsigned int initialWaterSeeds_;
    unsigned int initialGrassSeeds_;

    std::vector<double> cellsHumidity_;
    int humidityRange_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
