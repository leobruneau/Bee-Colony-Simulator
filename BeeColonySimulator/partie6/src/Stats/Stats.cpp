//
// Created by Léo Bruneau on 20/05/22.
//

#include "Stats.hpp"

#include <utility>

Stats::Stats()
    : activeId_(0) {
}

void Stats::setActive(int id) {
    bool condition (id >= 0 and (size_t)id < graphs_.size());
    activeId_ = id*condition + activeId_*(!condition);
}

std::string Stats::getCurrentTitle() {
    return (graphs_.at(activeId_)).first;
}

void Stats::next() {
    ++activeId_;
    bool condition (activeId_ >= (int)graphs_.size());
    activeId_ = activeId_*(!condition);
}

void Stats::previous() {
    --activeId_;
    bool condition (activeId_ < 0);
    activeId_ = (int)(graphs_.size() - 1)*condition;
}

void Stats::update(sf::Time dt) {

}

void Stats::drawOn(sf::RenderTarget &target) const {

}

void Stats::reset() {
    for (auto& g : graphs_)
        (g.second).second->reset(); // the first "second" refers to the pair and the second "second" refers to the unique_ptr inside the pair
}

void Stats::addGraph(int id,
                     const std::string &title,
                     const std::vector<std::string> &series,
                     double min,
                     double max,
                     const Vec2d &size) {

    graphs_.insert(graphs_.end() ,std::make_pair(id, std::make_pair(title, new Graph(series, size, min, max))));
    activeId_ = id;
}
