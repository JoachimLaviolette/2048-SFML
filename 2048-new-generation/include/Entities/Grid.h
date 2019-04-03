#ifndef GRID_H
#define GRID_H

#include "Entities/Tile.h"
#include "AI/AI.h"

#include <SFML/Graphics.hpp>

using namespace sf;

class Tile;

class Grid
{
private:
	Grid(int AI);
	static Grid* self;

	// Attributes
	std::vector<std::vector<Tile*>> m_tiles;
	AI* m_AI;
	int m_size; // in tiles per line
	float m_size_pix; // in pixels
	int m_dir;
	RectangleShape m_shape;

	bool doMerge;

	// Ressources
	Font* m_font;

	// TEST context variables
	std::vector<int> directions = std::vector<int>(4);
	bool _isMovePossible = false;
	bool _isTest = false;

	// Setup/initialization
	void setupAI(int AI);
	void setupSize();
	void setupSizePix();
	void setupShape();
	void centerShape();
	void setupFont();
	void initializeDirections();
	void initializeTiles();
	void setupTilesStates();

	// Actions
	void move(int x, int y);
	void mergeTiles(int current_x, int current_y, Tile* t);
	int getRandomIndex();
	void resetTestVariables();

	// Querying
	bool isRoomAvailable();
	bool canMove(Tile* currentTile);
	bool isThereANeighbour(Tile *neighbourTile);
	bool isThereAnEdge(Vector2f* pos);

public:
	// Static
	static Grid* createGrid(int AI);
	static int getRandomValue();

	// Actions
	void refreshTiles();
	void newTile();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveTiles();
	void unnewTiles();

	// Querying
	int count();
	bool isMovePossible();

	// Getters
	Font* getFont();
	float getTileSize();
	RectangleShape* getShape();
	int getSize();
	Tile* getTile(int x, int y);

	// Setters
	void setTile(int x, int y, Tile* tile);
	void removeTile(int x, int y);
	void turnOnTest();

	// Engine
	void update();
	void draw(RenderWindow* w);

	// Debug
	void __toString();
};

#endif