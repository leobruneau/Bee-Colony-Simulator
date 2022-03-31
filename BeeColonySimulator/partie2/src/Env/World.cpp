//
// Created by Léo Bruneau on 29/03/22.
//

#include "World.hpp"
#include "Application.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Utility/Vertex.hpp"

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
//    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, grass);
//    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, water);
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
