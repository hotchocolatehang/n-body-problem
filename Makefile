project_files = src/physics/abstract_body.cc src/body.cc src/physics/gravity_simulation.cc src/nbp_gui.cc
dependencies = -lsfml-graphics -lsfml-window -lsfml-system
app_name = N_Body_Problem

all: release

debug:
	g++ -g -std=c++17 -Wall -o bin/$(app_name)_debug src/main.cc \
$(project_files) \
$(dependencies)

release:
	g++ -O2 -std=c++17 -Wall -o bin/$(app_name) src/main.cc \
$(project_files) \
$(dependencies) \
2> errors.log

generator:
	g++ -std=c++17 -Wall -o bin/generate src/scene_generator.cc