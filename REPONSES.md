#PROJECT ANSWERS

##A1.1
In order to use the clamping function also in other parts of the 
program, we defined it inside the `Collider.hpp/cpp` files outside the class.
We also could've defined it inside the class since all of our objects in the program
will inherit from Collider, but we decided not to go with that option.
The last option, very similar to the first one, would be to define the function
inside another file that we would `#include` whenever needed, but it would only clutter
the project environment.

##A1.2
In this function we didn't use `for` loops because we found a "more efficient"
way of figuring out which is the real representation of the `to` vector.
We used to `if() {} else if () {}` statements which checked if the distance
between `x` and `y` coordinates where smaller than half of the width and height (respectively) 
of the world.

##A1.3
We think that all the arguments in our methods can be passed as `constant references`.

##A1.4
All the methods in our class were declared as `const` unless the `move(Vec2d dx)` method, since
it does change the position of our object.

##A1.5
In their implementation we just call the previous methods: `isColliderInside`, `isColliding` and 
`isPointInside`.

##A1.6
We chose, for (almost) all our overloaded operators, a "surcharge interne" because of the order
in which the operands are called in each operation.
The only operator which, by necessity, was implemented outside the class was `<<` (this
because we cannot access C++'s classes).

##A1.7
All of them.

##A1.8
All of them.