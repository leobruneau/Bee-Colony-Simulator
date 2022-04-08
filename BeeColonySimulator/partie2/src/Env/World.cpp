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

            std::cerr << std::endl << "File opened. File name: " << fileName << std::endl;

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

    int index (0);

    for (auto &s: seeds_) {
        if (s.seedNature_ == Kind::Grass) {

            moveSeed(s, index);
            if (cells_[index] != Kind::Water)
                cells_[index] = Kind::Grass;

        } else if (s.seedNature_ == Kind::Water) {

            if (bernoulli(getAppConfig().water_seeds_teleport_proba) == 0) {

                moveSeed(s, index);
                cells_[index] = Kind::Water;

            } else {

                int randomX (0), randomY (0);
                randomX = uniform(0, nbCells_-1); randomY = uniform(0, nbCells_-1);
                cells_[randomX + randomY*nbCells_] = Kind::Water;
                s.coordinates_.x = randomX;
                s.coordinates_.y = randomY;

            }
        }

    }
}

void World::moveSeed(Seed &s, int &index) const {

    index = s.coordinates_.x + s.coordinates_.y*nbCells_;
//    int index_check(index);

    Direction rDirection;
    rDirection = static_cast<Direction>(uniform(0, 3));

    switch (rDirection) {
        case Direction::North:
            if (index - nbCells_ >= 0) {
                index -= nbCells_;
                --s.coordinates_.y;
            }
            break;

        case Direction::East:
            if ((index % nbCells_) != (nbCells_ - 1)) {
                ++index;
                ++s.coordinates_.x;
            }
            break;

        case Direction::South:
            if (index + nbCells_ < nbCells_*nbCells_) {
                index += nbCells_;
                ++s.coordinates_.y;
            }
            break;

        case Direction::West:
            if ((index % nbCells_) != 0) {
                --index;
                --s.coordinates_.x;
            }
            break;

        default:
            break;
    }

//    if (s.seedNature_ == Kind::Grass and cells_[index] == Kind::Water) index = index_check;

}

void World::steps(int nb, bool update) {
    for (int i(0); i < nb; ++i) {step();}
    if (update) updateCache();
}

void World::smooth() {

    auto cellsCopy_(cells_);
    unsigned int waterCount(0), grassCount(0);
    std::vector<unsigned int> neighborhoodCells;

    for (size_t i(0); i < cellsCopy_.size(); ++i) {
        if (cells_[i] == Kind::Grass or cells_[i] == Kind::Rocks) {
            neighborhoodCells.clear();
            neighborhoodCells = findViableCells((int)i);
            for (auto w: neighborhoodCells) {
                if (cells_[w] == Kind::Water) ++waterCount;
            }
            if ((double)waterCount/(double)neighborhoodCells.size() > getAppConfig().smoothness_water_neighbor_ratio)
                cellsCopy_[i] = Kind::Water;
        }
    }

    std::swap(cells_, cellsCopy_);

    for (size_t i(0); i < cellsCopy_.size(); ++i) {
        if (cells_[i] == Kind::Rocks) {
            neighborhoodCells.clear();
            neighborhoodCells = findViableCells((int)i);
            for (auto g: neighborhoodCells) {
                if (cells_[g] == Kind::Grass) ++grassCount;
            }
            if ((double)grassCount/(double)neighborhoodCells.size() > getAppConfig().smoothness_grass_neighbor_ratio)
                cellsCopy_[i] = Kind::Grass;
        }
    }

    std::swap(cells_, cellsCopy_);

}

std::vector<unsigned int> World::findViableCells(int x) const {

    std::vector<unsigned int> viableIndexes;

    if (isViable(x-1-nbCells_, x)) viableIndexes.push_back(x-nbCells_-1);
    if (isViable(x-nbCells_, x)) viableIndexes.push_back(x-nbCells_);
    if (isViable(x-nbCells_+1, x)) viableIndexes.push_back(x-nbCells_+1);
    if (isViable(x-1, x)) viableIndexes.push_back(x-1);
    if (isViable(x+1, x)) viableIndexes.push_back(x+1);
    if (isViable(x+nbCells_-1, x)) viableIndexes.push_back(x+nbCells_-1);
    if (isViable(x+nbCells_, x)) viableIndexes.push_back(x+nbCells_);
    if (isViable(x+nbCells_+1, x)) viableIndexes.push_back(x+nbCells_+1);

    return viableIndexes;

}

bool World::isViable(int index, int x) const{

    if (index < 0 or index > nbCells_*nbCells_-1) {
       return false;
    } else if (x%nbCells_ == 0) {
        if (index%nbCells_ == nbCells_-1) {
            return false;
        } else return true;
    } else if (x%nbCells_ == nbCells_-1) {
        if (index%nbCells_ == 0) {
            return false;
        } else return true;
    } else return true;
}

void World::smooths(int nb, bool update) {
    for (int i(0); i < nb; ++i) {smooth();}
    if (update) updateCache();
}
