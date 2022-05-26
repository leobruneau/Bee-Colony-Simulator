//
// Created by Léo Bruneau on 29/03/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>
#include "Utility/Vec2d.hpp"
#include "../Interface/Drawable.hpp"

// Grass: 0, Water: 1, Rocks: 2 - used to define the type of terrain of a specific cell
enum class Kind : short {Grass, Water, Rocks};

// North: 0, East: 1, South: 2, West: 3 - used to define the direction of propagation of seeds
enum class Direction : short {North, East, South, West};

struct Seed {
    sf::Vector2i coordinates_;
    Kind seedNature_;
};

class World : public Drawable {
public:

    /*!
    * @brief user defined constructor
    */
    World() = default;

    /*!
     * @brief deleted copy-constructor to avoid unintentional copies of a "big" class
     */
    World(const World&) = delete;

    /*!
     * @brief reloads world configurations
     */
    void reloadConfig();

    /*!
     * @brief displays graphical components to the main window
     *
     * @param target main window
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief updates the cache storing the graphical information to be displayed
     */
    void updateCache();

    /*!
     * @brief reloads the cache structure
     */
    void reloadCacheStructure();

    /*!
     * @brief randomly recreates a new world
     *
     * @param regenerate true for automatic regeneration and false for manual
     */
    void reset(bool const& regenerate);

    /*!
     * @return returns nbCells_*cellSize_ (i.e. the size of the world)
     */
    float getSize() const;

    /*!
     * @brief returns cellSize_
     */
    float getCellSize() const;

    /*!
     * @brief loads world data from a file
     */
    void loadFromFile();

    /*!
     * @brief chooses randomly the seed movement (i.e. right, left, up, down)
     *
     * @param seed current seed
     * @param ind future position of seed
     */
    void moveSeed(Seed &seed, int &index) const;

    /*!
     * @brief handles movement of seeds in the world (single step)
     */
    void step();

    /*!
     * @brief handles multiple movements of the seeds in the world
     *
     * @param nb number of step we want the seeds to take
     * @param update true if we want to update the world at each step, false if we update only after nb steps
     */
    void steps(int nb, bool update = false);

    /*!
     * @brief smooths the "edges" of grass, water and rock patches in the world (fills lone tiles with the same type of surroundings)
     */
    void smooth();

    /*!
     * @brief applies the method "smooth()" a number of times equal to the parameter "nb"
     *
     * @param update tells us if we need to use "updateCache()" or not
     */
    void smooths(int nb, bool update = false);

    /*!
     * @brief returns a vector containing the indexes of cells that are viable (i.e. that don't go outside the world's boundaries)
     *
     * @param x index of the cell around which we are looking for viable cells
     *
     * @return vector of indexes
     */
    std::vector<int>findViableCells(int x) const;

    /*!
     * @brief checks if the potential new seed stays inside the boundaries of the world (not toric in this case)
     *
     * @param index position of the potential new seed
     * @param x x coordonate of the potential new seed
     *
     * @return
     */
    bool isViable(int index, int x) const;

    /*!
     * @brief saves current world structure to a new file
     */
    void saveToFile() const;

    /*!
     * @brief sets humidity level as a function of "eta" and "lambda" (configuration constants)
     *
     * @param eta constant in configuration file
     * @param lambda constant in configuration file
     */
    void setHumidity(double eta, double lambda);

    /*!
     * @brief displays humidity level at a specific position (of the mouse)
     *
     * @param target main window
     */
    void showDebugHumidity(sf::RenderTarget &target) const;

    /*!
     * @brief checks if a given is growable (i.e. if a flower can spawn at p)
     *
     * @param p position of interest
     *
     * @return true if is growable, false otherwise
     */
    bool isGrowable(const Vec2d& p) const;

    /*!
     * @brief returns humidity level at a point p
     *
     * @param p point of interest (position of mouse)
     *
     * @return humidity level
     */
    double getHumidity(Vec2d const& p) const;

    /*!
     * @brief checks if the area around a point p is hive-able (i.e. if a hive can spawn at p)
     *
     * @param p position of interest (position of mouse)
     * @param radius radius of the hive collider
     *
     * @return true if is hive-able, false otherwise
     */
    bool isHiveable(const Vec2d& p, double radius) const;

    /*!
     * @brief tells us if the bees can fly over the seed
     *
     * @param p position of the body
     *
     * @return false if the seed's "Rocks", true in all other cases
     */
    bool isFlyable(const Vec2d& p) const;

    /*!
     * @brief returns vector containing indexes of cells inside the rectangle of vertices topLeft and bottomRight
     *
     * @param topLeft index of top left corner
     * @param bottomRight index of bottom right corner
     *
     * @return vector of indexes
     */
    std::vector<std::size_t> findIndexes(const sf::Vector2i &topLeft, const sf::Vector2i &bottomRight) const;

    /*!
     * @brief find all the indexes of the cells that are inside the rectangle
     * considering toric conditions (defined by the relative positions of the "topLeft" and "bottomRight corners)
     *
     * @param topLeft index of top left corner
     * @param bottomRight index of bottom right corner
     *
     * @return a vector with the indexes of the cells inside the rectangle
     */
    std::vector<std::size_t> indexesForRect(sf::Vector2i const& topLeft, sf::Vector2i const& bottomRight) const;

private:
    int nbCells_;
    float cellSize_;
    std::vector<Kind> cells_;

    /*!
     * @brief SFML vertexes for grass texture
     */
    std::vector<sf::Vertex> grassVertexes_;

    /*!
     * @brief SFML vertexes for water texture
     */
    std::vector<sf::Vertex> waterVertexes_;

    /*!
     * @brief SFML vertexes for rock texture
     */
    std::vector<sf::Vertex> rockVertexes_;

    /*!
     * @brief SFML vertexes for humidity levels (shades of blue when in debug mode)
     */
    std::vector<sf::Vertex> humidityVertexes_;
    sf::RenderTexture renderingCache_;
    std::vector<Seed> seeds_;
    unsigned int initialWaterSeeds_;
    unsigned int initialGrassSeeds_;

    /*!
     * @brief humidity levels for each cell
     */
    std::vector<double> cellsHumidity_;
    int humidityRange_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP2_WORLD_HPP
