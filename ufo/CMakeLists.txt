# Author: Jeroen de Haas <jah.dehaas@avans.nl>
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wpedantic)#  -Werror)
    add_compile_options("$<$<CONFIG:DEBUG>:-g>")
endif()

add_library(ufo-common STATIC
  src/kmint/ufo/resources.cpp
  src/kmint/ufo/tank.cpp
  src/kmint/ufo/andre.cpp
  src/kmint/ufo/door.cpp
  src/kmint/ufo/saucer.cpp
  src/kmint/ufo/building.cpp
  src/kmint/ufo/human.cpp
  src/kmint/ufo/population.cpp
  src/kmint/ufo/dna_string.cpp
  src/kmint/ufo/dna_entity.cpp
  src/kmint/ufo/node_algorithm.cpp
  src/kmint/ufo/play.cpp
  src/kmint/ufo/tankstatemanager.cpp
  src/kmint/ufo/ufostatemanager.cpp)

target_compile_features(ufo-common PUBLIC cxx_std_17)
target_link_libraries(ufo-common PRIVATE kmint)
target_include_directories(ufo-common PRIVATE 
  src
  PUBLIC
  include)

add_executable(ufo-executable src/main.cpp)
target_link_libraries(ufo-executable PRIVATE ufo-common)

add_custom_target(ufo-resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
