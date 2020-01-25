cmake_minimum_required (VERSION 3.0.0)
project (N_Body_Problem)

add_library (abstract_body src/physics/abstract_body.h src/physics/abstract_body.cc)

add_library (gravity_simulation src/physics/gravity_simulation.h src/physics/gravity_simulation.cc)

add_library (body src/body.h src/body.cc)

add_library (nbp_gui src/nbp_gui.h src/nbp_gui.cc)
target_include_directories (nbp_gui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable (N_Body_Problem src/main.cc)
target_link_libraries (N_Body_Problem gravity_simulation nbp_gui body abstract_body
  sfml-graphics sfml-window sfml-system
)

add_executable (generate_scene src/scene_generator.cc)