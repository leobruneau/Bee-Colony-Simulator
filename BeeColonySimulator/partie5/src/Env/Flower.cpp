//
// Created by Léo Bruneau on 29/04/22.
//

#include "Flower.hpp"
#include <Application.hpp>



Flower::Flower(Vec2d position, double size, double pollen)
    : Collider (position, size), pollen_ (pollen), sizeFactor_ (1.0) {
    auto textures = getValueConfig()["simulation"]["flower"]["textures"];
    int index (uniform(0, (int)textures.size()-1));
    texture_ = getAppTexture(textures[index].toString());
}

double Flower::takePollen() {
    return 0;
}

void Flower::drawOn(sf::RenderTarget &target) const {
    auto flowerSprite = buildSprite(getPosition(), getRadius(), texture_);
    target.draw(flowerSprite);
}

void Flower::update(sf::Time dt) {
    double humidityThreshold (getAppConfig().flower_growth_threshold);
    double humidity (getAppEnv().getCellHumidity(getPosition()));
    pollen_ += dt.asSeconds() * log(humidity/humidityThreshold);

    double splitThreshold (getAppConfig().flower_growth_split);
    if (pollen_ >= splitThreshold) {
        pollen_ /= 2;
        double randomDistance(uniform(1.5*getRadius(), 2.5*getRadius()));
        Vec2d position (getPosition() + Vec2d::fromRandomAngle()*randomDistance);

        for (int i (0); i < 100; ++i)
            if (getAppEnv().addFlowerAt(position, true)) break;
    }
    getAppEnv().removeDeadFlowers();
}

double Flower::getPollen() const {
    return pollen_;
}

double Flower::getFactor() const {
    return sizeFactor_;
}

