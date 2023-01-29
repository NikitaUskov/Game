cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
project(game)

#set(SFML_DIR SFML-2.5.1/lib/cmake/SFML)

add_compile_options(-DSFML_STATIC)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(game main.cpp graphics/TileOnMap.cpp)

include_directories(app application)

add_library(objects application/RPG_Object.cpp application/Game.cpp application/Level.cpp application/Cell.cpp
        application/Item.cpp application/Weapon.cpp application/Ammo_container.cpp application/Medicine_Kit.cpp
        application/Unit.cpp application/Forager.cpp application/Wild.cpp application/Rational.cpp
        application/Operative.cpp application/Table.cpp application/help_func.h)

target_link_libraries(game sfml-graphics objects)
