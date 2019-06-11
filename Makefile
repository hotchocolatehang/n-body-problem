project_files = src/abstract_body.cpp src/body.cpp src/gravity_simulation.cpp src/nbp_gui.cpp
dependencies = -lsfml-graphics -lsfml-window -lsfml-system
app_name = N_Body_Problem

all: release

debug:
	g++ -g -std=c++17 -Wall -o bin/$(app_name)_debug src/main.cpp \
$(project_files) \
$(dependencies)

release:
	g++ -O2 -std=c++17 -Wall -o bin/$(app_name) src/main.cpp \
$(project_files) \
$(dependencies) \
2> errors.log

generator:
	g++ -std=c++17 -Wall -o bin/generate src/scene_generator.cpp