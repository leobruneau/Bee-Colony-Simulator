//
// Created by Léo Bruneau on 27/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOG_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOG_HPP
#include "../Interface/Updatable.hpp"
#include "../Interface/Drawable.hpp"
#include "../Utility/Utility.hpp"

class Fog : public Updatable, public Drawable {
public:
    Fog() = default;
    ~Fog() override;

    void update(sf::Time dt) override;
    void drawOn(sf::RenderTarget& target) const override;

private:
    double _density;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP7_FOG_HPP
