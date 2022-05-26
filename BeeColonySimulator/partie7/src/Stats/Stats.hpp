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

    /*!
     * @brief user-defined default constructor
     */
    Stats();

    /*!
     * @brief sets the active graph we want to be displaying on the screen
     *
     * @param id we want to set as active
     */
    void setActive(int id);

    /*!
     * @return returns the title of the current graph that is being displayed
     */
    std::string getCurrentTitle();

    /*!
     * @brief switches the active graph to the next one
     */
    void next();

    /*!
     * @brief switches the active graph to the previous one
     */
    void previous();

    /*!
     * @brief calls the update() method for each graph in the collection
     *
     * @param dt time span
     */
    void update(sf::Time dt) override;

    /*!
     * @brief calls the drawOn() method for each graph in the collection
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief calls the reset() method for each graph in the collection
     */
    void reset();

    /*!
     * @brief adds a new graph to the collection
     *
     * @param title is the name of the graph
     * @param id is the integer identifier for the graph
     * @param series is the collection of titles of the plots making up the graph
     * @param min is the minimum value starting from which the data is displayed
     * @param max is the maximum value up to which the data is displayed
     */
    void addGraph(
            int id,
            const std::string &title,
            const std::vector<std::string> &series,
            double min,
            double max,
            const Vec2d &size
    );

private:

    /*!
     * @brief map containing the integer id of the graph and a pair {title of the graph + pointer to the graph}
     */
    std::map<int, std::pair<std::string, std::unique_ptr<Graph>>> graphs_;
    int activeId_;

    /*!
     * @brief time elapsed between one update and the next
     */
    sf::Time elapsedTime_;

};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP6_STATS_HPP
