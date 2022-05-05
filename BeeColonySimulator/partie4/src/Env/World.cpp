//
// Created by Léo Bruneau on 29/03/22.
//

#include "World.hpp"
#include "Application.hpp"
#include "Utility/Vertex.hpp"
#include "Random/Random.hpp"
#include "Utility/Utility.hpp"
#include "HelperFunctions.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>


void World::reloadConfig() {
    nbCells_ = getAppConfig().world_cells;
    cellSize_ = (float)getAppConfig().world_size/(float)nbCells_;
    cells_ = std::vector<Kind> ( nbCells_*nbCells_, Kind::Rocks);
    cellsHumidity_ = std::vector<double> (nbCells_*nbCells_,.0);
    initialGrassSeeds_ = getAppConfig().world_nb_grass_seeds;
    initialWaterSeeds_ = getAppConfig().world_nb_water_seeds;
    seeds_.clear();

    double eta(getAppConfig().world_humidity_init_level);
    double lambda(getAppConfig().world_humidity_decay_rate);
    double threshold(getAppConfig().world_humidity_threshold);
    humidityRange_ = 0;

    while (eta*exp(-humidityRange_/lambda) > threshold) {
        ++humidityRange_;
    }
}

void World::drawOn(sf::RenderTarget &target) const {
    if (getAppConfig().showHumidity()) {
        target.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads);
        if (isDebugOn()) showDebugHumidity(target);
    } else {
        sf::Sprite cache(renderingCache_.getTexture());
        target.draw(cache);
    }
}

void World::updateCache() {
    sf::RenderStates grass; sf::RenderStates water; sf::RenderStates rock;
    renderingCache_.clear();
    renderingHumidity_.clear();
    grass.texture = &getAppTexture(getAppConfig().grass_texture);
    water.texture = &getAppTexture(getAppConfig().water_texture);
    rock.texture = &getAppTexture(getAppConfig().rock_texture);

    std::vector<size_t> vertexCoordinates;
    int counter(0);
    double blueLevel(.0), minHumidity(9999999999.9), maxHumidity(.0);

    for (auto const& hLevel: cellsHumidity_) {
        if (hLevel > maxHumidity) maxHumidity = hLevel;
        if (hLevel < minHumidity) minHumidity = hLevel;
    }

    for (size_t i(0); i < cells_.size(); ++i) {

        vertexCoordinates.clear();
        vertexCoordinates = indexesForCellVertexes(counter%nbCells_, (int)trunc(counter/nbCells_), nbCells_);

        blueLevel = (cellsHumidity_[i]-minHumidity)/(maxHumidity-minHumidity) * 255;

        for (auto index: vertexCoordinates) {

            switch (cells_[i]) {
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

            humidityVertexes_[index].color = sf::Color(0, 0, int(blueLevel));
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
    humidityVertexes_ = grassVertexes_;
    float size = (float)nbCells_*cellSize_;
    renderingCache_.create((unsigned int)size, (unsigned int)size);
    renderingHumidity_.create((unsigned int)size, (unsigned int)size);
}

void World::reset(bool const& regenerate) {
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

    // Setting the initial state of humidity levels when the initial seeds are generated
//    setHumidity(getAppConfig().world_humidity_init_level, getAppConfig().world_humidity_decay_rate);

    if (regenerate) {
        steps(getAppConfig().world_generation_steps, false);
        smooths(getAppConfig().world_generation_smoothness_level, false);
    }

    // Evaluating final humidity after generating random environment
    setHumidity(getAppConfig().world_humidity_init_level, getAppConfig().world_humidity_decay_rate);

    updateCache();

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
            double varH(0);
            cells_.clear();

            for (int i(0); i < nbCells_*nbCells_; ++i) {
                worldMap >> var >> std::ws;
                cells_.push_back(static_cast<Kind>(var));
            }

            // Read Humidity levels from existing file
            cellsHumidity_.clear();
            for (int i(0); i < nbCells_*nbCells_; ++i) {
                worldMap >> varH >> std::ws;
                cellsHumidity_.push_back(varH);
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
    for (int i(0); i < nb; ++i) {
        step();
//        setHumidity(getAppConfig().world_humidity_init_level, getAppConfig().world_humidity_decay_rate);
    }
    if (update) updateCache();
}

void World::smooth() {

    auto cellsCopy_(cells_);
    unsigned int waterCount(0), grassCount(0);
    std::vector<int> neighborhoodCells;

    for (size_t i(0); i < cellsCopy_.size(); ++i) {
        if (cells_[i] == Kind::Grass or cells_[i] == Kind::Rocks) {
            waterCount = 0;
            neighborhoodCells.clear();
            neighborhoodCells = findViableCells((int)i);
            for (auto const& w: neighborhoodCells) {
                if (cells_[w] == Kind::Water) ++waterCount;
            }
            if ((double)waterCount/(double)neighborhoodCells.size() > getAppConfig().smoothness_water_neighbor_ratio) cellsCopy_[i] = Kind::Water;
        }
    }

    for (size_t i(0); i < cells_.size(); ++i) {
        if (cells_[i] == Kind::Rocks) {
            grassCount = 0;
            neighborhoodCells.clear();
            neighborhoodCells = findViableCells((int)i);
            for (auto const& g: neighborhoodCells) {
                if (cells_[g] == Kind::Grass) ++grassCount;
            }
            if ((double)grassCount/(double)neighborhoodCells.size() > getAppConfig().smoothness_grass_neighbor_ratio) cellsCopy_[i] = Kind::Grass;
        }
    }

    std::swap(cells_, cellsCopy_);
}

std::vector<int> World::findViableCells(int x) const {

    std::vector<int> viableIndexes;

    if (isViable(x-1-nbCells_, x)) viableIndexes.push_back(x-nbCells_-1);   // top left tile
    if (isViable(x-nbCells_, x)) viableIndexes.push_back(x-nbCells_);       // top middle tile
    if (isViable(x-nbCells_+1, x)) viableIndexes.push_back(x-nbCells_+1);   // top right tile
    if (isViable(x-1, x)) viableIndexes.push_back(x-1);                     // middle left tile
    if (isViable(x+1, x)) viableIndexes.push_back(x+1);                     // middle right tile
    if (isViable(x+nbCells_-1, x)) viableIndexes.push_back(x+nbCells_-1);   // bottom left tile
    if (isViable(x+nbCells_, x)) viableIndexes.push_back(x+nbCells_);       // bottom middle tile
    if (isViable(x+nbCells_+1, x)) viableIndexes.push_back(x+nbCells_+1);   // bottom right tile

    return viableIndexes;
}

bool World::isViable(int index, int x) const{

    if (index < 0 or index > nbCells_*nbCells_-1) {

       return false;

    } else if (x%nbCells_ == 0) {

        if (index%nbCells_ == nbCells_-1)
            return false;
        else
            return true;

    } else if (x%nbCells_ == nbCells_-1) {

        if (index%nbCells_ == 0)
            return false;
        else
            return true;

    } else return true;
}

void World::smooths(int nb, bool update) {
    for (int i(0); i < nb; ++i) {smooth();}
    if (update) updateCache();
}

void World::saveToFile() const {
    try {

        std::string mapName(getApp().getResPath() + "userRandomWorld.map");
        std::ofstream newMap (mapName.c_str());

        if (newMap.fail()) {
            throw std::runtime_error("failed to create and open file. Not enough storage.");
        }

        newMap << nbCells_ << std::endl;
        newMap << cellSize_ << std::endl;

        for (auto const& tileType: cells_) {
            switch (tileType) {
                case Kind::Grass:
                    newMap << "0 ";
                    break;
                case Kind::Water:
                    newMap << "1 ";
                    break;
                case Kind::Rocks:
                    newMap << "2 ";
                    break;
                default:
                    break;
            }
        }

        newMap << std::endl;

        // Writing humidity levels to file
        for (auto const& hLevel: cellsHumidity_) {
            newMap << hLevel << " ";
        }

        newMap << std::endl;
        newMap.close();
    }

    catch(std::runtime_error&) {
        throw;
    }
}

// Main version of setHumidity() that is called each time a water seed moves
//void World::setHumidity(double eta = getAppConfig().world_humidity_init_level, double lambda = getAppConfig().world_humidity_decay_rate) {
//
//    int xStart, xStop, yStart, yStop;
//
//    for (auto const& seed: seeds_) {
//        if (seed.seedNature_ == Kind::Water) {
//
//            cellsHumidity_[seed.coordinates_.x + seed.coordinates_.y*nbCells_] += eta;
//
//            xStart = seed.coordinates_.x - humidityRange_; xStop = seed.coordinates_.x + humidityRange_;
//            yStart = seed.coordinates_.y - humidityRange_; yStop = seed.coordinates_.y + humidityRange_;
//
//            for (int x(xStart); x <= xStop; ++x) {
//                for (int y(yStart); y <= yStop; ++y) {
//                    if ((x >= 0 and x < nbCells_) and (y >= 0 and y < nbCells_))
//                        cellsHumidity_[y*nbCells_+x] += eta*exp(-std::hypot(seed.coordinates_.x-x,seed.coordinates_.y-y)/lambda);
//                }
//            }
//            break;
//        }
//    }
//}

// Version of setHumidity() that is called only one time at the end of the reset() method
void World::setHumidity(double eta = getAppConfig().world_humidity_init_level, double lambda = getAppConfig().world_humidity_decay_rate) {

    int xStart, xStop, yStart, yStop;

    for (int x(0); x < nbCells_; ++x) {
        for (int y(0); y < nbCells_; ++y) {

            if (cells_[y*nbCells_+x] == Kind::Water) {

                cellsHumidity_[y*nbCells_+x] += eta;

                xStart = x - humidityRange_; xStop = x + humidityRange_ + 1;
                yStart = y - humidityRange_; yStop = y + humidityRange_ + 1;

                for (int a(xStart); a < xStop; ++a) {
                    for (int b(yStart); b < yStop; ++b) {
                        if ((a >= 0 and a < nbCells_) and (b >= 0 and b < nbCells_))
                            cellsHumidity_[b*nbCells_+a] += eta*exp(-std::hypot(x-a,y-b)/lambda);
                    }
                }
            }
        }
    }
}

void World::showDebugHumidity(sf::RenderTarget &target) const {
    auto mouse = getApp().getCursorPositionInView();
    Vec2d textPosition (mouse.x()-15, mouse.y()-15);
    size_t index(help::getIndex(mouse, cellSize_, nbCells_));

    int x ((int)trunc(mouse.x()/cellSize_)), y ((int)trunc(mouse.y()/cellSize_));
    std::string toWrite(" ");
    if ((x >= 0 and x < nbCells_) and (y >= 0 and y < nbCells_))
        toWrite = to_nice_string(cellsHumidity_[index]);
    else
        toWrite = "-";

    auto const text = buildText(toWrite, textPosition, getAppFont(), 25, sf::Color::Red);
    target.draw(text);
}

bool World::isGrowable(const Vec2d &p) const {
    size_t index (help::getIndex(p, cellSize_, nbCells_));
    int x ((int)trunc(p.x()/cellSize_)), y ((int)trunc(p.y()/cellSize_));

    if ((x >= 0 and x < nbCells_) and (y >= 0 and y < nbCells_)) {
        if (cells_[index] == Kind::Grass) return true;
        else return false;
    } else { return false; }
}

double World::getHumidity(const Vec2d &p) const {
    return cellsHumidity_[help::getIndex(p, cellSize_, nbCells_)];
}

bool World::isHiveable(const Vec2d &p, double radius) const {
    auto toAdd (new Hive(p, radius));
    if (getAppEnv().getCollidingHive(*toAdd) != nullptr) {
        delete toAdd; toAdd = nullptr;
        return false;
    }
    if (getAppEnv().getCollidingFlower(*toAdd) != nullptr) {
        delete toAdd; toAdd = nullptr;
        return false;
    }
    return true;
}










