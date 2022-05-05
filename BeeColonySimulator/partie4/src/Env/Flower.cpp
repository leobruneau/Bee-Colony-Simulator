//
// Created by Léo Bruneau on 29/04/22.
//

#include "Flower.hpp"
#include <Application.hpp>



Flower::Flower(Vec2d position, double size, double pollen)
    : Collider (position, size), pollen_ (pollen) {
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
        int i(100);

        double randomDistance(uniform(1.5*getRadius(), 2.5*getRadius()));
        Vec2d position (getPosition() + Vec2d::fromRandomAngle()*randomDistance);

        while (i != 0) {
            if (getAppEnv().addFlowerAt(position, true)) {
                pollen_ /= 2;
                break;
            }
            else --i;
        }
    }
    getAppEnv().removeDeadFlowers();
}

double Flower::getPollen() const {
    return pollen_;
}

