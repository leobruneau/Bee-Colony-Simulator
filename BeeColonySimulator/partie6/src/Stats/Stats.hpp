//
// Created by Léo Bruneau on 20/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP6_STATS_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP6_STATS_HPP
#include <memory>
#include <map>
#include "../Interface/Updatable.hpp"
#include "Graph.hpp"


class Stats : public Drawable, public Updatable {
public:
    void setActiveId(int id);
    std::string getCurrentTitle();
    void next();
    void update(sf::Time dt) override;
    void drawOn(sf::RenderTarget& target) const override;

private:
    std::map<size_t, std::string, Graph> graphs_;
    size_t activeId_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP6_STATS_HPP
