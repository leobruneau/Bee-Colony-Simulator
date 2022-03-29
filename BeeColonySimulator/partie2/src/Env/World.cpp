//
// Created by Léo Bruneau on 29/03/22.
//

#include "World.hpp"
#include "Application.hpp"
#include <vector>

void World::reloadConfig() {
    nbCells_ = getAppConfig().world_cells;
    cellSize_ = getAppConfig().world_size/nbCells_;
    cells_ = std::vector<Kind> ( nbCells_*nbCells_, Kind::Rocks);
}

void World::drawOn(sf::RenderTarget &target) {

}
