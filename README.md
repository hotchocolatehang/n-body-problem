# N-body problem
## Summary
A simple program to simulate gravitational interaction of N given bodies. 

I've made this application as my project for "summer practice" in my university.

## Building
As long as the application requires SFML make sure you have all libsfml packages installed, including libsfml-dev.

To build the app just type
```
$ make
```
and it appears as bin/N_Body_Problem .

To create model generator execute
```
$ make generator
```
which will create bin/generate .

Everything works fine on Debian 9.9 Stretch x64.

## Usage

### Apps

N_Body_problem gets just one argument - the relative path to a scene file. Something like
```
$ ./bin/N_Body_Problem path/to/scene.simsetup
```

This repo provides some prepared scene files in the scenes/ folder. 

You can configure window size and length of trajectory of bodies
in the config/n-body-problem.conf .

Generator needs at least 2 parameters: -f \<filename> -n \<number of bodies>. Also you can tell the program
what timescale you need by adding -t \<time scale>:
```
$ ./bin/generate -f new_scene -n 100 -t 0.03
$ ./bin/N_Body_Problem new_scene.simsetup
```
As you can see the scene generator adds .simsetup to a given filename and saves generated result in scenes/ folder.

### Scene format

Every scene is a text file which looks like
```
<N which stands for number of bodies> <Time scale> <Just number one lol>\n
// and N strings of the following format:
<initial coordinates> <velocity vector> <mass> <radius> <color>\n
// or more detailed:
<x> <y> <vx> <vy> <m> <r> <R> <G> <B>\n

```
For example:
```
3 1 1
500 275 0.1 0.03 5e+9 15 0 255 0
570 275 0 0.575 1000000 5 255 0 0
700 275 0.0 0.581 2000000 7 0 0 255
```
This example can be found as 'scenes/orbits.simsetup'.

### Modules

If you need something to gravitate (or you really like BDSM) you can try using my
module for gravitational interacton. It doesn't have any dependencies but STL so to include it to your project you don't have to include SFML.

All you need after
```c++
#include "abstract_body.h"
#include "gravity_simulation.h"
```
is to inherit your class from n_body_problem::AbstractBody
and then construct your class properly.
After this you can enjoy all the possibilities of gravity just by creating an object of
n_body_problem::GravitySimulation class and using it's methods with correct parameters.
Keep in mind that every vectorized value is an instance of n_body_problem::vector2<> class./
