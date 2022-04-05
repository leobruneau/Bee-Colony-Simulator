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
#include <stdexcept>

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
            throw std::runtime_error("ERROR: failed to open " + fileName + ". File doesn't exist.");
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
