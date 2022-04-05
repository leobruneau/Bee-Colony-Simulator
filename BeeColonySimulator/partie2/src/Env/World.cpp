//
// Created by Léo Bruneau on 29/03/22.
//

#include "World.hpp"
#include "Application.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Utility/Vertex.hpp"
#include "Random/Random.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

void World::reloadConfig() {
    nbCells_ = getAppConfig().world_cells;
    cellSize_ = getAppConfig().world_size/(float)nbCells_;
    cells_ = std::vector<Kind> ( nbCells_*nbCells_, Kind::Rocks);
    initialGrassSeeds_ = getAppConfig().world_nb_grass_seeds;
    initialWaterSeeds_ = getAppConfig().world_nb_water_seeds;
    seeds_.clear();
}

void World::drawOn(sf::RenderTarget &target) {
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}

void World::updateCache() {
    sf::RenderStates grass; sf::RenderStates water; sf::RenderStates rock;
    renderingCache_.clear();
    grass.texture = &getAppTexture(getAppConfig().grass_texture);
    water.texture = &getAppTexture(getAppConfig().water_texture);
    rock.texture = &getAppTexture(getAppConfig().rock_texture);

    std::vector<size_t> vertexCoordinates;
    int counter(0);

    for (auto var: cells_) {

        vertexCoordinates.clear();
        vertexCoordinates = indexesForCellVertexes(counter%nbCells_, (int)trunc(counter/nbCells_), nbCells_);

        for (auto index: vertexCoordinates) {
            switch (var) {
                case Kind::Grass:
                    grassVertexes_[index].color.a = 255;
                    waterVertexes_[index].color.a = 0;
                    rockVertexes_[index].color.a = 0;
                    break;

                case Kind::Water:
                    grassVertexes_[index].color.a = 0;
                    waterVertexes_[index].color.a = 255;
                    rockVertexes_[index].color.a = 0;
                    break;

                case Kind::Rocks:
                    grassVertexes_[index].color.a = 0;
                    waterVertexes_[index].color.a = 0;
                    rockVertexes_[index].color.a = 255;
                    break;

                default:
                    break;
            }
        }

        ++counter;
    }

    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, grass);
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, water);
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rock);
    renderingCache_.display();
}

void World::reloadCacheStructure() {
    grassVertexes_ = generateVertexes(getValueConfig()["simulation"]["world"]["textures"], nbCells_, cellSize_);
    waterVertexes_ = grassVertexes_;
    rockVertexes_ = grassVertexes_;
    float size = (float)nbCells_*cellSize_;
    renderingCache_.create(size, size);
}

void World::reset(bool const& regenerate) {
    if (regenerate) {
        // still to code...
    } else {
        reloadConfig();
        reloadCacheStructure();

        Seed seed;
        int randomX_ (0), randomY_ (0);

        seed.seedNature_ = Kind::Grass;
        for (size_t i(0); i < initialGrassSeeds_; ++i) {
            randomX_ = uniform(0, nbCells_-1);
            randomY_ = uniform(0, nbCells_-1);
            seed.coordinates_ = sf::Vector2i(randomX_, randomY_);
            seeds_.push_back(seed);
        }

        seed.seedNature_ = Kind::Water;
        for (size_t i(0); i < initialWaterSeeds_; ++i) {
            randomX_ = uniform(0, nbCells_-1);
            randomY_ = uniform(0, nbCells_-1);
            seed.coordinates_ = sf::Vector2i(randomX_, randomY_);
            seeds_.push_back(seed);
        }

        int index(0);
        for (auto s: seeds_) {

            index = s.coordinates_.x + s.coordinates_.y*nbCells_;

            if (cells_[index] != Kind::Water) cells_[index] = s.seedNature_;

        }

        updateCache();

    }
}

float World::getSize() const {
    return (float)nbCells_*cellSize_;
}

void World::loadFromFile() {

    std::string fileName (getApp().getResPath() + getAppConfig().world_init_file);
    std::string nbCells, cellSize, mapStructure;

    try {

        std::ifstream worldMap (fileName);

        if (!worldMap.is_open()) {
            throw std::runtime_error("failed to open " + fileName + " - File doesn't exist.");
        } else {

            std::cerr << std::endl << "File opened - file name: " << fileName << std::endl;

            std::getline(worldMap, nbCells);
            std::getline(worldMap, cellSize);

            nbCells_ = stoi(nbCells);
            cellSize_ = (float)stoi(cellSize);

            short var(0);
            cells_.clear();

            for (int i(0); i < nbCells_*nbCells_; ++i) {
                worldMap >> var >> std::ws;
                cells_.push_back(static_cast<Kind>(var));
            }

            reloadCacheStructure();
            updateCache();

        }
    }

    catch (std::runtime_error&) {
        throw;
    }

}

void World::step() {

    Direction rDirection;
    int index (0);
    std::vector<Seed> newSeeds;

    for (auto s: seeds_) {
        if (s.seedNature_ == Kind::Grass) {

            moveSeed(s, index, newSeeds);
            cells_[index] = Kind::Grass;

        } else if (s.seedNature_ == Kind::Water) {

            if (bernoulli(getAppConfig().water_seeds_teleport_proba) == 0) {
                moveSeed(s, index, newSeeds);
                cells_[index] = Kind::Water;
            } else {
                int randomX (0), randomY (0);
                randomX = uniform(0, nbCells_-1); randomY = uniform(0, nbCells_-1);
                cells_[randomX + randomY*nbCells_] = Kind::Water;
                Seed seed;
                seed.coordinates_.x = randomX; seed.coordinates_.y = randomY;
                seed.seedNature_ = Kind::Water;
                newSeeds.push_back(seed);
            }
        }
    }

    for (auto newSeed: newSeeds) {
        seeds_.push_back(newSeed);
    }

}

void World::moveSeed(Seed const& s, int &index, std::vector<Seed> &add) {

    index = s.coordinates_.x + s.coordinates_.y*nbCells_;

    // we use numbers from 0 to 3 as
    Direction rDirection;
    rDirection = static_cast<Direction>(uniform(0, 3));

    Seed seedAdded;

    switch (rDirection) {
        case Direction::North:
            if (index - nbCells_ >= 0) index -= nbCells_;
            seedAdded = s;
            seedAdded.coordinates_.y -= 1;
            add.push_back(seedAdded);
            break;

        case Direction::East:
            if ((index % nbCells_) != (nbCells_-1)) ++index;
            seedAdded = s;
            seedAdded.coordinates_.x += 1;
            add.push_back(seedAdded);
            break;

        case Direction::South:
            if (index + nbCells_ < nbCells_*nbCells_) index += nbCells_;
            seedAdded = s;
            seedAdded.coordinates_.y += 1;
            add.push_back(seedAdded);
            break;

        case Direction::West:
            if ((index % nbCells_) != 0) --index;
            seedAdded = s;
            seedAdded.coordinates_.x -= 1;
            add.push_back(seedAdded);
            break;

        default:
            break;
    }

}

void World::steps(int nb, bool update) {

    if (update) {
        for (int i(1); i <= nb; ++i) {
            step();
        }
        updateCache();
    } else {
        for (int i(1); i <= nb; ++i) {
            step();
        }
    }

}
