//
// Created by Léo Bruneau on 29/04/22.
//

#include "Flower.hpp"
#include <Application.hpp>


Flower::Flower(Vec2d position, double size, double pollen)
    : Collider (position, size), pollen_ (pollen) {
    // code
}

double Flower::takePollen() {
    return 0;
}

void Flower::drawOn(sf::RenderTarget &target) const {
    auto textures = getValueConfig()["simulation"]["flower"]["textures"];
    int index (uniform(0, (int)textures.size()-1));
    auto const& texture = getAppTexture(textures[index].toString());
    auto flowerSprite = buildSprite(getPosition(), getRadius(), texture);
    target.draw(flowerSprite);
}

