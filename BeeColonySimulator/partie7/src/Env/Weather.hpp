//
// Created by Léo Bruneau on 27/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP7_WEATHER_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP7_WEATHER_HPP
#include "Fog.hpp"


struct Wind {
    /*!
     * @brief direction of wind
     */
    Vec2d _velocity;

    /*!
     * @brief strength of wind
     */
    double _speed;
};

class Weather : public Updatable, public Drawable {
public:
    Weather() = default;
    Weather(const Weather& other) = delete;
    Weather& operator=(const Weather& other) = delete;
    Weather(double temperature = 25.0, Vec2d windDirection = {1, 0}, double windSpeed = 0);
    ~Weather() override;

    void update(sf::Time dt) override;
    void drawOn(sf::RenderTarget& target) const override;

private:
    double _temperature;
    Wind _wind;
    std::vector<std::unique_ptr<Fog>> _fog;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP7_WEATHER_HPP
