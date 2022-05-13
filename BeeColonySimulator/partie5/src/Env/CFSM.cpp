//
// Created by Léo Bruneau on 12/05/22.
//

#include "CFSM.hpp"
#include <utility>

CFSM::CFSM(States states)
    : states_(std::move(states)), currentState_(states_.begin()) {

}

State CFSM::getState() const {
    return *currentState_;
}

void CFSM::nextState() {
    if (currentState_ == states_.end()) currentState_ = states_.begin();
    else ++currentState_;
    onEnterState(*currentState_);
}

void CFSM::onState(State const& state, sf::Time const& dt) {
//    action(dt);
}

void CFSM::action(sf::Time const& dt) {

}

void CFSM::onEnterState(State const& state) {

}
