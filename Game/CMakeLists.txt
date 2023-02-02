cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(game)

set(BUILD_SHARED_LIBS OFF)

include(cmake/CPM.cmake)
CPMAddPackage("gh:SFML/SFML#2.5.1")

add_executable(game main.cpp graphics/TileOnMap.cpp application/RPG_Object.cpp application/Game.cpp application/Level.cpp application/Cell.cpp
application/Item.cpp application/Weapon.cpp application/Ammo_container.cpp application/Medicine_Kit.cpp
application/Unit.cpp application/Forager.cpp application/Wild.cpp application/Rational.cpp
application/Operative.cpp application/Table.cpp application/help_func.h)

include_directories(app application)

target_link_libraries(game sfml-graphics)