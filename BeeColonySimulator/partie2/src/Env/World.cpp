//
// Created by Léo Bruneau on 29/03/22.
//

#include "World.hpp"
#include "Application.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

void World::reloadConfig() {
    nbCells_ = getAppConfig().world_cells;
    cellSize_ = getAppConfig().world_size/nbCells_;
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
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, grass);
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, water);
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rock);
}

void World::reloadCacheStructure() {
    for (size_t i(0); i < nbCells_; ++i) {
        for (size_t j(0); j < nbCells_; ++j) {

            grassVertexes_.emplace_back(sf::Vertex(sf::Vector2f(i*cellSize_, j*cellSize_)));
            grassVertexes_.emplace_back(sf::Vertex(sf::Vector2f(cellSize_*(i+1), j*cellSize_)));
            grassVertexes_.emplace_back(sf::Vertex(sf::Vector2f(cellSize_*(i+1), cellSize_*(j+1))));
            grassVertexes_.emplace_back(sf::Vertex(sf::Vector2f(cellSize_*i, cellSize_*(j+1))));

        }
    }

    waterVertexes_ = grassVertexes_;
    rockVertexes_ = grassVertexes_;
}
