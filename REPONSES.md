# PROJECT ANSWERS

##  FIRST PART (1)

### A1.1
In order to use the clamping function also in other parts of the 
program, we defined it inside the `Collider.hpp/cpp` files outside the class.
We also could've defined it inside the class since all of our objects in the program
will inherit from Collider, but we decided not to go with that option.
The last option, very similar to the first one, would be to define the function
inside another file that we would `#include` whenever needed, but it would only clutter
the project environment.

### A1.2
In this function we didn't use `for` loops because we found a "more efficient"
way of figuring out which is the real representation of the `to` vector.
We used to `if() {} else if () {}` statements which checked if the distance
between `x` and `y` coordinates where smaller than half of the width and height (respectively) 
of the world.

### A1.3
We think that all the arguments in our methods can be passed as `constant references`.

### A1.4
All the methods in our class were declared as `const` unless the `move(Vec2d dx)` method, since
it does change the position of our object.

### A1.5
In their implementation we just call the previous methods: `isColliderInside`, `isColliding` and 
`isPointInside`.

### A1.6
We chose, for (almost) all our overloaded operators, a "surcharge interne" because of the order
in which the operands are called in each operation.
The only operator which, by necessity, was implemented outside the class was `<<` (this
because we cannot access C++'s classes).

### A1.7
All of them.

### A1.8
All of them.

## SECOND PART (2)

### A2.1
Well the size of `cells_` is going to be `(nbCells_)^2` because the grid is said to
be a square.

### A2.2
For `getAppConfig().world_size` it would be the sequence `["simulation"]["world"]["default size"]`
and for `getAppConfig().world_cells` it would be the sequence `["simulation"]["world"]["default cells"]`

### A2.3
The methods' calls will be 1)`reloadConfig()` 2)`reloadCacheStructure()`
and 3)`updateCache()`. The order is justified by the fact that 

### A2.4
The advantage is to be able to change the name of the target file directly in the app*.json configuration file. This allows
us to change the file without stopping the program. We can thus do it while it is running and then update the newly configured file
in the running program (using the `c` key)

### A2.5
The `loadFromFile()` method must call the `reloadCacheStructure()` and the `updateCache()` methods (in the order presented).

### A2.6
We used a `std::vector<Seed>` type for the `seeds_` attribute. We could have used an array (because the number of seeds doesn't change after their
initialization), but we decided to use the vector in order to use the useful class methods

## THIRD PART (3)

### A3.1
In order to implement those functions, we will call the respective ones that we declared in the `World` class. For instance, we will call `World::drawOn()` inside
`Env::drawOn()` (on the World attribute of the Env class) and `World::reset()` inside `Env::reset()` (on the World attribute of the Env class). For now the 
`Env::update()` method will remain empty since once the World has been created, it doesn't change in time.

### A3.2
We just call the `loadFromFile()` method from the World class. The file that is going to be used to load the world is present in the *.json configuration
file. The code associated with the `r` key is in the `handleEvent()` method in the `Application.cpp/hpp` file(s).

### A3.3
The algorithm to update (set) the humidity has to be called each time a water seed 
moves and transmits its nature to the new cell. This happens in the method `Env::step()`
in two different places: where `bernoulli(...) == 0` and where `bernoulli(...) != 0`

### A3.4
To avoid looping again on the `humidity vertexes`, we just implement their initialization inside the already existing loop.


### A3.5
Since a Flower is a Collider the class Flower will inherit from the class Collider.

### A3.6
To solve the problem we add a `st::Texture` attribute to the flower class and initialize it in the Flower constructor. The in the `drawOn()` method
we just call the already initialized texture stored inside the attribute.

### A3.7
It is going to be a dynamic collection (i.e. a vector) of pointers on `Flower` objects in order to implement polymorphism behaviour.

### A3.8
The `~Env()` destructor must deallocate all the memory linked to the `flowers_` collection and then clear the vector to get rid of all
the dangling pointers and start afresh.

### A3.9
The `reset()` method in the Env class will call a method (that we called `flowerDestoryer()` and that does what we said in the previous answer) and
deallocate all the memory linked to the `flowers_` collection pointers and then clear the vector.

### A3.10
The already existing `Env::drawOn()` method has to be modified by adding a range based forLoop that call the `Flower::drawOn()` method for each 
flower in the `flowers_` collection.

### A3.11
To get the humidity at a given cell, we added a function in the World class (called `getHumidity()`) which is then called in the method `Env::getCellHumidity`
which we can finally call in the method `Flowe::update()` thanks to the syntax `getAppEnv().getCellHumidity()`

### A3.12
We need to modify the `Env::drawOn()` method and in it iterate through the `newFlowers_` collection (that we used to differentiate between flowers
that are able to split and flowers that do not have that ability) and call, for each flower, the `Flower::drawOn()` method.
(if `f` is our pointer containing the address of a flower, we will access the wanted method writing `f->drawOn()`)

### A3.13
We need to modify the `Flower::update()` method, in the Flower class, that will call the `Env::removeDeadFlowers()` method in Env
(writing `getAppEnv().removeDeadFlowers()`) which will handle the deletion of the flowers that don't have enough pollen.

### A3.14
The class that will inherit from `Drawable` and `Updatable` will be: World, Env & Flower.

### A3.15
We use the expression `World(const World&) = delete` in the World class and `Env(const Env&) = delete` in the Env class. These two prevent the 
copy of an object (World or Env) from an existing one.

### A3.16
We need to modify the `Env::update()` method so that every time it is called we can call `flowerGenerator_.update(dt)` and
randomly (and automatically) generate our flowers.

## FOURTH PART (4)

### A4.1
Because, since we will have different types of bees inheriting from the same Bee class, we'll want to implement a polymorphic behaviour
which needs virtual functions that operate on pointers

### A4.2
The Hive class will inherit from the following classes: Collider, Updatable, Drawable. For the attributes we will have a collection of
pointers on Bees, a double representing the amount of nectar of the hive. (We also decided to add an attribute which will contain the texture of the Hive.
It could be in fact fun (in the future) to have different types of Hives corresponding to different species of bees. We also did this so that
when we call the `Env::drawOn()` method we don't have to use `getAppTexture(getAppConfig().hive_texture)` each time, but we can just use the `texture_`
attribute.)

### A4.3
We need to use the following lines of code: `Hive(Hive const& source) = delete` and `Hive& operator=(Hive const& source) = delete`

### A4.4
The `~Hive()` destructor must, each time it is called, deallocate the memory linked to the pointers in the `bees_` collection and then
clear the vector itself (just to be sure not to have any dangling pointers that may point to sensible zones in memory).

### A4.5
No. If the environment dies, so do all the hives, flowers and bees in it.

### A4.6


