all: sim_debug sim_release

project_files = src/abstract_body.cpp src/body.cpp src/gravity_simulation.cpp
dependencies = -lsfml-graphics -lsfml-window -lsfml-system

sim_debug:
	g++ -g -std=c++17 -Wall -o bin/simulation_debug src/main.cpp \
$(project_files) \
$(dependencies)

sim_release:
	g++ -O2 -std=c++17 -Wall -o bin/simulation_release src/main.cpp \
$(project_files) \
$(dependencies)
