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
    void action(sf::Time const& dt);
    virtual void onState(State const& state, sf::Time const& dt) = 0;
    virtual void onEnterState(State const& state) = 0;

private:
    States states_;
    size_t currentState_;
};


#endif //BEE_COLONY_SIMULATOR_SSV_2022_STEP5_CFSM_HPP
