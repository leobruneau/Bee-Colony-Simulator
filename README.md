# Bee-Colony-Simulator
Simulation for a bee colony. 
It simulates the evolution of one or more hives (where each one has its proper set of bees) inside and environment where flowers are allowed
to grow randomly and evolve following humidity patterns given by the presence (or absence therefore) of water.
This is the possibility of randomly generating an environment.
The whole program relies on a specific configuration file (*.json) whose parameters can be tweaked to adjust the simulation.

## How To Run
### From IDE (with cmake)
   1) open project folder in your IDE
   2) open the folder `partie6` (open `partie7` instead if you want more functionalities)
   3) go in the src sub-folder
   4) load the `CMakeLists.txt` file
   5) run the target `application`

### From commandline
   1) open the project folder and go to `BeeColonySimulator/partie7/src/cmake-build-debug`
   2) run the command: `cmake name_of_target` (make sure to replace name_of_target by the name of the target you want to compile)
   3) then run the command: `./name_of_target name_of_config_file` (make sure to replace name_of_config_file by the *.json file of your choice)
   4) enjoy the program!

## Different targets
   1) `colliderTest` -> tests the good functioning of collision mechanics of the simulation
   2) `worldTest` -> tests the generation of the graphical world/environment in which the simulation will take place
   3) `flowerTest` -> tests the random (or manual) generation of flowers inside the environment (also used to test humidity levels)
   4) `hiveTest` -> tests the generation of hives inside the environment (also generates bees)
   5) `collaborationTest` -> tests the collaboration mechanics between worker and scout bees
   6) `scoutTest` -> specifically tests the behaviour of the scout bees
   7) `workerTest` -> specifically tests the behaviour of the worker bees
   8) `application` -> this is the target used to run the final application

## Comments
Commented files are present only inside the partie7 folder (which should be final).
The useful keyboard keys are displayed in a side panel that appears on a grey background once the program has started running.

## Configuration files modifications
Inside `partie7`, we change the `avoidance delay` constant inside the `app3.json` file to `2.5` (for worker bees) and `1.5` (for scout bees).
This gives the bees enough time to find a better way to go back to the hive (or towards the flower) when an obstacle is in the way.

## Extensions (inside `partie7` folder)
   1) The `Bee.cpp` file was modified in order to display bees only when they are not inside the hive.
   2) A new `*.png` file was added for the hives (looks better)
   3) We added some new classes to simulate diverse weather conditions: we added a Fog() and a FogGenerator() class to be able to create fog clouds as a function of humidity. We also added temperature controls
        and its effects on the environment. Finally, we added also the wind. Unfortunately the time wasn't enough to code the algorithms to simulate the effects of fog and wind on the bees and other elements of the environment,
        but we managed to add variable transparency to clouds and passive movement depending on the wind.
   4) We also added the possibility of manually adding a cloud by pressing the 'v' key on the keyboard. The cloud will be added only if there is enough humidity in the area.
   5) To simulate the effects of temperature on the environment we built a function (displayed in a given .png file inside the res folder of partie7) that, for a given temperature,
        returns the influence factor (a double ranging from .3 to 1, where 1 means there are no effects);
   6) In order to build those functions we use some curve-fitting software. The constants in the .json file regarding the min and max temperature should not be changed
        since we weren't able (not enough time) to create a function that dynamically adapts to those parameters.