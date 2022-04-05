//
// Created by Léo Bruneau on 29/03/22.
//

#include "World.hpp"
#include "Application.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Utility/Vertex.hpp"
#include <iostream>
#include <fstream>

void World::reloadConfig() {
    nbCells_ = getAppConfig().world_cells;
    cellSize_ = getAppConfig().world_size/(float)nbCells_;
    cells_ = std::vector<Kind> ( nbCells_*nbCells_, Kind::Rocks);
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

//    #### Commented out code must be reviewed ####

    std::vector<size_t> vertexCoordinates;
    int counter(0);

    std::cerr << "size cells_: " << cells_.size() << std::endl;
    std::cerr << "grass size: " << grassVertexes_.size() << std::endl;
    std::cerr << "water size: " << waterVertexes_.size() << std::endl;
    std::cerr << "rocks size: " << rockVertexes_.size() << std::endl;

    for (auto var: cells_) {

        vertexCoordinates.clear();
        vertexCoordinates = indexesForCellVertexes(counter, (int)trunc(counter/nbCells_), nbCells_);

        switch (var) {
            case Kind::Grass:
                for (auto i: vertexCoordinates) {
                    grassVertexes_[i].color.a = 255;
                    waterVertexes_[i].color.a = 0;
                    rockVertexes_[i].color.a = 0;
                }
                break;

            case Kind::Water:
                for (auto i: vertexCoordinates) {
                    grassVertexes_[i].color.a = 0;
                    waterVertexes_[i].color.a = 255;
                    rockVertexes_[i].color.a = 0;
                }
                break;

            case Kind::Rocks:
                for (auto i: vertexCoordinates) {
                    grassVertexes_[i].color.a = 0;
                    waterVertexes_[i].color.a = 0;
                    rockVertexes_[i].color.a = 255;
                }
                break;

            default:
                break;
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
    int size = nbCells_*cellSize_;
    renderingCache_.create(size, size);
}

void World::reset(bool const& regenerate) {
    if (regenerate) {
        // still to code...
    } else {
        reloadConfig();
        reloadCacheStructure();
        updateCache();
    }
}

float World::getSize() const {
    return (float)nbCells_*cellSize_;
}

void World::loadFromFile() {

    std::string filePath_ (getApp().getResPath() + getAppConfig().world_init_file);
    std::ifstream mapFile_;
    std::string nbCells, cellSize, mapStructure;

    try {
        mapFile_.open(filePath_);
        if (mapFile_.is_open()) {
            std::getline(mapFile_, nbCells);
            std::getline(mapFile_, cellSize);

            // #### Everything after this line must be reviewed ####

            std::getline(mapFile_, mapStructure);
            nbCells_ = stoi(nbCells);
            cellSize_ = (float)stoi(cellSize);

            for (auto var: mapStructure) {
                switch (var) {
                    case '0':
                        cells_.push_back(static_cast<Kind>(0));
                        break;
                    case '1':
                        cells_.push_back(static_cast<Kind>(1));
                        break;
                    case '2':
                        cells_.push_back(static_cast<Kind>(2));
                        break;
                    default:
                        break;
                }
            }
            for(auto var:cells_) {
                if (var==Kind::Grass) {
                    std::cout << '0';
                } else if (var==Kind::Water) {
                    std::cout << '1';
                } else if (var==Kind::Rocks) {
                    std::cout << '2';
                }
            }

            reset(false);
            mapFile_.close();

        } else {
            throw std::runtime_error("ERROR: Unable to open " + filePath_ + ". File does not exist.");
        }
    }
    catch (std::runtime_error&) {
        throw;
    }
}
