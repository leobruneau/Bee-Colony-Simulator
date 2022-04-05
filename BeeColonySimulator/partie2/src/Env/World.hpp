//
// Created by Léo Bruneau on 29/03/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>

enum class Kind : short {Grass, Water, Rocks};

// North: 0, East: 1, South: 2, West: 3 - used to define the direction of propagation of seeds
enum class Direction : short {North, East, South, West};

struct Seed {
    sf::Vector2i coordinates_;
    Kind seedNature_;
};

class World {
public:
    void reloadConfig();
    void drawOn(sf::RenderTarget& target);
    void updateCache();
    void reloadCacheStructure();
    void reset(bool const& regenerate);
    float getSize() const;
    void loadFromFile();
    void moveSeed(Seed const& seed, int &ind, std::vector<Seed> &add);
    void step();
    void steps(int nb, bool update = false);

private:
    int nbCells_;
    float cellSize_;
    std::vector<Kind> cells_;
    std::vector<sf::Vertex> grassVertexes_;
    std::vector<sf::Vertex> waterVertexes_;
    std::vector<sf::Vertex> rockVertexes_;
    sf::RenderTexture renderingCache_;
    std::vector<Seed> seeds_;
    unsigned int initialWaterSeeds_;
    unsigned int initialGrassSeeds_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
