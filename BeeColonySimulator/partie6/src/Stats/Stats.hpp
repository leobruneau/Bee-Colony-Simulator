//
// Created by Léo Bruneau on 20/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP6_STATS_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP6_STATS_HPP
#include <memory>
#include <map>
#include "../Interface/Updatable.hpp"
#include "Graph.hpp"

//struct NameAndGraph {
//    std::string title_;
//    std::unique_ptr<Graph> graph_;
//};


class Stats : public Drawable, public Updatable {
public:
    Stats();
    void setActive(int id);
    std::string getCurrentTitle();
    void next();
    void previous();
    void update(sf::Time dt) override;
    void drawOn(sf::RenderTarget& target) const override;
    void reset();
    void addGraph(
            int id,
            const std::string &title,
            const std::vector<std::string> &series,
            double min,
            double max,
            const Vec2d &size
    );

private:
    std::map<int, std::pair<std::string, std::unique_ptr<Graph>>> graphs_;
    int activeId_;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP6_STATS_HPP
