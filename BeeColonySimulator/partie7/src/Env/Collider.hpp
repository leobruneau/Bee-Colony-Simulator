/*
 * POOSV EPFL 2022
 * @authors: 
 */

#pragma once

#include <Utility/Vec2d.hpp>
#include <iostream>
#include <fstream>

/* TO BE COMPLETED */

class Collider {
private:
    Vec2d position;
    double radius;
public:
    /*!
     * @brief User defined constructor
     *
     * @param coordinates position of the collider
     * @param rad dimension of the collider
     */
    Collider(Vec2d const& coordinates, double const& rad);

    /*!
     * @brief User defined copy constructor
     */
    Collider(Collider const& other);

    /*!
     * @brief cleaner implementation of operator= overloading since we are just copying
     *        the values from one object (the source) to another (the target)
     *
     * @param source just another collider
     */
    Collider& operator=(Collider const& source);

    /*!
     * @brief "+" operator overloading defining new position to the collider
     *
     * @param dx vector we want to overload with collider's position
     */
    Collider& operator+=(Vec2d const& dx);

    /*!
     * @brief method to get the position of the collider
     */
    Vec2d const& getPosition() const;

    /*!
     * @brief method to get the dimension of the collider
     */
    double getRadius() const;

    /*!
     * @brief chooses the shortest path of from the actual to the desired position in the toric world
     *
     * @param to position we'd like to reach
     *
     * @return the shortest vector
     */
    Vec2d directionTo(Vec2d const& to) const;

    /*!
     * @brief does the same work but takes a collider as an argument
     *
     * @param to position of the collider we'd like to reach
     *
     * @return the shortest vector
     */
    Vec2d directionTo(Collider const& to) const;

    /*!
     * @param to position we'd like to reach
     *
     * @return length of the vector got by the method "directionTo()"
     */
    double distanceTo(Vec2d const& to) const;

    /*!
     * @param to position of the collider we'd like to reach
     *
     * @return length of the vector got by the method "directionTo()"
     */
    double distanceTo(Collider const& to) const;

    /*!
     * @brief add the Vec2d "dx" to the instance's position thanks to the += operator overloading
     *
     * @param dx the vector we'd like to add
     */
    void move(Vec2d const& dx);

    /*!
     * @brief checks if a collider is inside another collider
     *
     * @return true if it is and false if it's not
     */
    bool isColliderInside(Collider const& other) const;

    /*!
     * @brief checks if both of the colliders instance's radius are touching each other
     *
     * @return true if it is and false if it's not
    */
     virtual bool isColliding(Collider const& other) const;

    /*!
     * @brief checks if a point is inside a collider
     *
     * @param point the point
     *
     * @return true if it is and false if it's not
    */
    bool isPointInside(Vec2d const& point) const;

    /*!
     * @brief ">" operator overloading using the "isColliderInside()" method
    */
    bool operator>(Collider const& other) const;

    /*!
     * @brief "|" operator overloading using the "isColliding()" method
    */
    bool operator|(Collider const& other) const;

    /*!
     * @brief ">" operator overloading using the "isPointInside()" method
    */
    bool operator>(Vec2d const& point) const;

    /*!
     * @brief get the enlargement factor which is useful to adjust the collider's radius
     *
     * @return 1. but is modified due to polymorphism
    */
    virtual double getFactor() const;
};

    /*!
     * @brief "<<" operator overloading showing the coordonates and the radius of a collider (passed in parameter)
     */
std::ostream& operator<<(std::ostream&, Collider const& other);

    /*!
     * @brief if the body gets out of the world, this method is used to get it back
     * following the logic of the toric world
     *
     * @param position actual position of the body
     */
void clamping(Vec2d& position);
