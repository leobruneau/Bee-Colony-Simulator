//
// Created by Léo Bruneau on 27/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP7_WEATHER_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP7_WEATHER_HPP

#include "Fog.hpp"
#include "FogGenerator.hpp"


struct Wind {
    /*!
     * @brief direction of wind
     */
    Vec2d _direction;

    /*!
     * @brief strength of wind
     */
    double _speed;
};

class Weather : public Updatable, public Drawable {
public:

    /*!
     * @brief copy constructor deleted to prevent duplication (there can be only one weather in an environment)
     *
     * @param other target weather object to copy
     */
    Weather(const Weather& other) = delete;

    /*!
     * @brief deleted operator= for this class to prevent duplication (same reason as above)
     *
     * @param other target weather object to copy
     * @return reference on a weather object (definition of operator= overloading)
     */
    Weather& operator=(const Weather& other) = delete;

    /*!
     * @brief user defined constructor
     *
     * @param temperature of the environment (changes during simulation)
     * @param windDirection varies during simulation
     * @param windSpeed varies during simulation
     */
    Weather(double temperature = 25.0, Vec2d windDirection = {1, 0}, double windSpeed = 0);

    /*!
     * @brief user defined destructor
     */
    ~Weather() override;

    /*!
     * @brief handles evolution of weather object
     *
     * @param dt
     */
    void update(sf::Time dt) override;

    /*!
     * @brief resets the weather to default
     */
    void reset();

    /*!
     * @brief displays to the main window the drawable components of the weather (like the fog clouds)
     *
     * @param target main window
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief displays debug information about the weather
     *
     * @param target main window
     */
    void showDebugInfo(sf::RenderTarget& target) const;

    /*!
     * @brief adds a fog cloud at a given position p
     *
     * @param p position of interest
     * @return true if the cloud was successfully added, false otherwise
     */
    void addFogAt(const Vec2d& p);

    /*!
     * @brief deallocates memory of fog clouds (i.e. removes them from weather)
     */
    void fogDestoryer();

    /*!
     * @return wind attribute of the weather
     */
    Wind getWind() const;

    /*!
     * @brief increases temperature attribute by 1
     */
    void increaseTemperature();

    /*!
     * @brief decreases temperature attribute by 1
     */
    void decreaseTemperature();

private:
    double _temperature;
    Wind _wind;

    /*!
     * @brief collection of fog clouds
     */
    std::vector<Fog*> _fog;

    FogGenerator _fogGenerator;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP7_WEATHER_HPP
