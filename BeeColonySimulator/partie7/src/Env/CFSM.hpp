//
// Created by Léo Bruneau on 12/05/22.
//

#ifndef BEE_COLONY_SIMULATOR_SSV_2022_STEP5_CFSM_HPP
#define BEE_COLONY_SIMULATOR_SSV_2022_STEP5_CFSM_HPP
#include "Utility/Utility.hpp"

typedef Uid State;
typedef std::vector<State> States;

class CFSM {
public:
    /*!
     * @brief user defined constructor
     *
     * @param states collection of possible state of the automate
     */
    explicit CFSM(States  states);

    /*!
     * @brief return the current state's unique id
     */
    State getState() const;

    /*!
     * @brief increments currentState_ to change state
     */
    void nextState();

    /*!
     * @brief method that handles actions that need to be taken whilst in a specific state
     */
    void action(sf::Time const& dt);

    /*!
     * @brief handles actions during a specific state (called by action())
     *
     * @param state current state
     */
    virtual void onState(State const& state, sf::Time const& dt) = 0;

    /*!
     * @brief handles necessary actions when changing states
     *
     * @param state state into which the automate is entering
     */
    virtual void onEnterState(State const& state) = 0;

private:

    /*!
     * @brief vector of States (unique ids)
     */
    States states_;
    size_t currentState_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_CFSM_HPP
