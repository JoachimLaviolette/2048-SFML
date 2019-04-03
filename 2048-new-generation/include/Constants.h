#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>

// VALUES
const int GHOST_VAL = -1;

// DIRECTIONS INPUT
const int DIR_LEFT = 0;
const int DIR_RIGHT = 1;
const int DIR_UP = 2;
const int DIR_DOWN = 3;
const int DIR_NONE = -1;

// SCREEN DIMENSION
const int SCREEN_W = 1920;
const int SCREEN_H = 1080;

// GRID SIZES
const int SIZE_AI_EASY = 3;
const int SIZE_AI_NORMAL = 4;
const int SIZE_AI_HARD = 5;
const int GRID_EDGE_WIDTH = 3;

// AI
const int AI_EASY = 0;
const int AI_NORMAL = 1;
const int AI_HARD = 2;

// TILES COLORS
const sf::Color TILE_COLOR_2 = sf::Color(255, 250, 265);
const sf::Color TILE_COLOR_4 = sf::Color(252, 245, 118);
const sf::Color TILE_COLOR_8 = sf::Color(255, 245, 66);
const sf::Color TILE_COLOR_16 = sf::Color(219, 208, 13);
const sf::Color TILE_COLOR_32 = sf::Color(252, 210, 121);
const sf::Color TILE_COLOR_64 = sf::Color(252, 183, 80);
const sf::Color TILE_COLOR_128 = sf::Color(255, 179, 15);
const sf::Color TILE_COLOR_256 = sf::Color(219, 151, 4);
const sf::Color TILE_COLOR_512 = sf::Color(255, 161, 84);
const sf::Color TILE_COLOR_1024 = sf::Color(255, 128, 25);
const sf::Color TILE_COLOR_2048 = sf::Color(255, 25, 25);
const sf::Color TILE_COLOR_GHOST = sf::Color(219, 219, 219);

// TILES TEXT STRINGS
const std::string TEXT_STRING_GHOST = " ";

// TILES TEXT SIZES
const unsigned int TEXT_SIZE_STD = 60U; // for 1 and 2-digit numbers 
const unsigned int TEXT_SIZE_3DIGIT = 50U;
const unsigned int TEXT_SIZE_4DIGIT = 35U;
const unsigned int TEXT_SIZE_GHOST = 0U;

#endif