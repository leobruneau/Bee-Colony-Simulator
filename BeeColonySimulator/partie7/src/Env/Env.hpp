//
// Created by Léo Bruneau on 12/04/22.
//
#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP

#include "World.hpp"
#include "Flower.hpp"
#include "../Interface/Updatable.hpp"
#include "FlowerGenerator.hpp"
#include "Hive.hpp"
#include "Weather.hpp"

struct Wind;

class Env : public Drawable, public Updatable {
public:

    /*!
     * @brief user defined default constructor
     */
    Env();

    /*!
     * @brief user defined default destructor
     */
    ~Env() override;

    /*!
     * @brief delete copy constructor to avoid unintentional copies of this "big" class
     */
    Env(const Env&) = delete;

    /*!
     * @brief handles the operations needed to update the environment at each dt
     */
    void update(sf::Time dt) override;

    /*!
     * @brief handles the operations to draw the graphical components to the main window
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief loads world cache from a preexisting configuration file
     */
    void loadWorldFromFile();

    /*!
     * @brief saves world cache to a new configuration file
     */
    void saveWorldToFile() const;

    /*!
     * @brief handles the resetting of the environment
     */
    void reset();

    /*!
     * @brief returns size of the environment
     *
     * @return nbCells_ * cellSize_ (world's attributes)
     */
    float getSize();

    /*!
     * @brief resets controls
     */
    void resetControls();

    /*!
     * @brief tries to add a new flower at a give position p
     *
     * @param p position for the new flower
     * @param split is true if the flower is reproducing (i.e. splitting)
     *
     * @return ture if the flower was succesfully added, false otherwise
     */
    bool addFlowerAt(Vec2d const& p, bool split = false);

    /*!
     * @brief draws a circle around the flower which is green if the position is valid and red otherwise
     *
     * @param target main window where graphical components are displayed
     * @param position position at which to draw the circle (i.e. the position of the corresponding flower)
     */
    void drawFlowerZone(sf::RenderTarget& target, Vec2d const& position);

    /*!
     * @brief deallocates memory of all flowers in the flowers_ and newFlowers_ collections when the destructor for Env is called
     */
    void flowerDestroyer();

    /*!
     * @brief returns humidity level at a give position
     *
     * @param position position of interest
     *
     * @return humidity level as a double
     */
    double getCellHumidity(Vec2d const& position);

    /*!
     * @brief handles the removal of flowers whose pollen level has reached 0 (hence dead flowers)
     */
    void removeDeadFlowers();

    /*!
     * @brief handles the addition of a hive at a give position
     *
     * @param position position of interest
     *
     * @return true if the hive was successfully added, false otherwise
     */
    bool addHiveAt(const Vec2d& position);

    /*!
     * @brief deallocates memory of all hives in the hives_ collection when the Env destructor is called
     */
    void hiveDestroyer();

    /*!
     * @brief returns a pointer to the hive with which the body parameter is colliding
     *
     * @param body collider body to test collision with
     *
     * @return pointer to colliding hive
     */
    Hive* getCollidingHive(const Collider& body);

    /*!
     * @brief returns a pointer to the flower with which the body parameter is colliding
     *
     * @param body collider body to test the collision
     *
     * @return pointer to colliding flower
     */
    Flower* getCollidingFlower(const Collider& body);

    /*!
     * @brief draws a rectangle around the position at which we want to add the hive. Green means the position is viable,
     * blue that the there is a colliding body in the zone and red means that the area contains some non grass cells
     *
     * @param target main window
     * @param position position of interest of the hive
     */
    void drawHiveableZone(sf::RenderTarget& target, Vec2d const& position);

    /*!
     * @brief returns true is the a hive can be successfully added at a position p
     *
     * @param p position of interest
     * @param radius radius of the collider we want to add (a hive in this case)
     *
     * @return true if the hive can be addedd and false otherwise
     */
    bool isHiveable(const Vec2d& p, double radius) const;

    /*!
     * @brief returns true if a bee can fly over the environment at a position p
     *
     * @param p position of interest
     *
     * @return true if the bee can fly over the position, false otherwise
     */
    bool isFlyable(const Vec2d& p) const;

    /*!
     * @brief returns the collection of flowers of the environment
     */
    const std::vector<Flower*>* getFlowers();

    /*!
     * @brief empty function. Still to be coded (is bonus)
     */
//    Bee* getBeeAt(Vec2d const& p) const;

    /*!
     * @brief returns an unordered map containing the new data points for the stats
     *
     * @param title name of the graph we are fetching data for. Depending on the graph, the data changes
     *
     * @return unordered_map containing new datapoints
     */
    std::unordered_map<std::string, double> fetchData(std::string const& title) const;

    /*!
     * @brief returns data points for the GENERAL graph
     *
     * @param map unordered map passed by reference so that the method can meaningfully manipulate it
     */
    void fetchGeneralData(std::unordered_map<std::string, double>& map) const;

    /*!
     * @brief returns data points for the HIVES graph
     *
     * @param map unordered map passed by reference so that the method can meaningfully manipulate it
     */
    void fetchHivesData(std::unordered_map<std::string, double>& map) const;

    /*!
     * @brief returns a vector of strings of the form "hive #X" where X is the number corresponding to the index in the collection hives_
     */
    std::vector<std::string> getHivesIds() const;

    /*!
     * @brief adds a cloud of fog at the mouse's position
     *
     * @param position of fog cloud
     * @return true if the cloud was successfully added
     */
    bool addFogAt(Vec2d const& position);

    /*!
     * @return the wind parameter of the weather of the environment
     */
    Wind getWind() const;

    /*!
     * @return current wind speed in the environment
     */
    double getWindSpeed() const;

    /*!
     * @brief increases temperature of environment (by 1 degree)
     */
    void increaseTemperature();

    /*!
     * @brief decreases temperature of environment (by 1 degree)
     */
    void decreaseTemperature();

    /*!
     * @brief checks if there is enough humidity in a given aera for fog to develop
     *
     * @return true if fog can develop, false otherwise
     */
    bool canFogSpawn(const Vec2d& position) const;

    /*!
     * @brief returns true if in the cloud active zone there is enough humidity to increase density of cloud
     *
     * @param position position of interest
     * @return true if humidity is enough, false otherwise
     */
    bool fogHumidityThreshold(const Vec2d& position) const;

    /*!
     * @return the value of the temperature
     */
    double getTemperature() const;

    /*!
     * @brief decrease wind speed by 1
     */
    void decreaseWindSpeed();

     /*!
      * @brief increase wind speed by 1
      */
    void increaseWindSpeed();

    /*!
     * @brief decreases wind direction's angle
     */
    void decreaseWindDirection();

    /*!
     * @brief increases wind direction's angle
     */
    void increaseWindDirection();

    /*!
     * @brief calls the temperatureEffects() methods for all interested objects in the environment
     */
    void temperatureEffects();

    /*!
     * @brief calculates the factor of influence of temperature (follows the curves displayed in a .png file
     * inside the res folder)
     *
     * @param temp current temperature of the environment
     * @return specific influence factor
     */
    static double getTemperatureFactor(double temp);

    /*!
     * @return the current wind velocity (i.e. _speed * _direction)
     */
    Vec2d getWindVelocity() const;

private:
    World world_;
    std::vector<Flower*> flowers_;
    std::vector<Flower*> newFlowers_;
    std::vector<Hive*> hives_;

    /*!
     * @brief random flower generator
     */
    FlowerGenerator flowerGenerator_;

    /*!
     * @brief weather object to simulate atmospheric components
     */
    Weather weather_;
};

#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP3_ENV_HPP
