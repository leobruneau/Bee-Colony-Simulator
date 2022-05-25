//
// Created by Léo Bruneau on 12/05/22.
//

#include "CFSM.hpp"
#include <utility>

CFSM::CFSM(States states)
    : states_(std::move(states)), currentState_(0) {

}

State CFSM::getState() const {
    return states_.at(currentState_);
}

void CFSM::nextState() {
    if (currentState_ == states_.size()-1) currentState_ = 0;
    else ++currentState_;
    onEnterState(states_.at(currentState_));
}

void CFSM::action(const sf::Time &dt) {
    onState(getState(), dt);
}