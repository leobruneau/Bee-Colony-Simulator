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
    explicit CFSM(States  states);
    State getState() const;
    void nextState();
    void onState(State const& state, sf::Time const& dt);
    void action(sf::Time const& dt);
    void onEnterState(State const& state);

private:
    States states_;
    States::iterator currentState_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_CFSM_HPP
