#include "pch.h"

#include "Constants.h"
#include "Entities/Grid.h"
#include "AI/AI_Easy.h"
#include "AI/AI_Normal.h"
#include "AI/AI_Hard.h"

#include <iostream>

Grid *Grid::self = false;

/**
	Static function called to instantiate a new grid with a specific AI
	Create a new grid only if none has been created

	@param The AI chosen for the game
	@return The current grid instance
*/
Grid* Grid::createGrid(int AI) {
	if (!self) {
		self = new Grid(AI);
	}

	return self;
}

/**
	Private constructor
*/
Grid::Grid(int AI)
{	
	initializeDirections();

	doMerge = false;
	setupSizePix();
	setupShape();
	setupAI(AI);
	setupSize();
	setupFont();
	centerShape();
	initializeTiles();

	// debug
	//__toString();
}

/**
	Setup the grid's AI
*/
void Grid::setupAI(int AI)
{
	switch (AI) {
	case AI_EASY:
		m_AI = new AI_Easy();
		break;
	case AI_NORMAL:
		m_AI = new AI_Normal();
		break;
	default:
		m_AI = new AI_Hard();
	}
}

/**
	Setup the size of the grid according to its AI
*/
void Grid::setupSize()
{
	m_size = m_AI->getGridSize();
}

/**
	Setup the grid's size (in pixels)
*/
void Grid::setupSizePix()
{
	m_size_pix = VideoMode::getDesktopMode().width / 3.0f;
}

/**
	Setup the grid's shape
*/
void Grid::setupShape()
{
	m_shape = sf::RectangleShape(
		sf::Vector2f(m_size_pix, m_size_pix)
	);
}

/**
	Center the shape of the grid on the screen
*/
void Grid::centerShape()
{
	float screenWidth = VideoMode::getDesktopMode().width;
	float screenHeight = VideoMode::getDesktopMode().height;

	sf::FloatRect shapeRect = m_shape.getLocalBounds();

	m_shape.setOrigin(
		shapeRect.left + shapeRect.width / 2.0f,
		shapeRect.top + shapeRect.height / 2.0f
	);

	m_shape.setPosition(
		screenWidth / 2.0f,
		screenHeight / 2.0f
	);
}

/**
	Initialize the vector storing the directions
*/
void Grid::initializeDirections()
{
	directions.push_back(DIR_LEFT);
	directions.push_back(DIR_RIGHT);
	directions.push_back(DIR_UP);
	directions.push_back(DIR_DOWN);
}

/**
	Initialize properly the grid according to its size
*/
void Grid::initializeTiles()
{
	using namespace std;

	for (int i = 0; i < m_size; i++) {
		m_tiles.push_back(vector<Tile*>()); // Add an empty row
	}

	for (int x = 0; x < m_size; x++) {
		for (int y = 0; y < m_size; y++) {
			m_tiles[x].push_back(Tile::createGhost(x, y, this));
		}
	}

	setupTilesStates();
	refreshTiles();
}

/**
	Turn two ghost tiles into valued tiles
*/
void Grid::setupTilesStates()
{
	int a = getRandomIndex();
	int b = getRandomIndex();
	
	while (count() < 2) {
		if (m_tiles[a][b]->isGhost()) {
			delete(m_tiles[a][b]);
			m_tiles[a][b] = Tile::createRandom(a, b, this);
		}

		a = getRandomIndex();
		b = getRandomIndex();
	}
}

/**
	Refresh every grid's tile's state
*/
void Grid::refreshTiles()
{
	for (int x = 0; x < m_size; x++) {
		for (int y = 0; y < m_size; y++) {
			m_tiles[x][y]->refresh();
		}
	}
}

/**
	Randomly generate a new valued tile on the grid
 */
void Grid::newTile()
{
	if (!isRoomAvailable()) {
		return;
	}

	int a = getRandomIndex();
	int b = getRandomIndex();
	int currentCount = count();

	while (currentCount == count()) {
		if (m_tiles[a][b]->isGhost()) {
			m_tiles[a][b] = Tile::createRandom(a, b, this);
		}

		a = getRandomIndex();
		b = getRandomIndex();
	}
}

/**
	Check if any room is still available on the grid

	@return If there is room available on the grid
*/
bool Grid::isRoomAvailable()
{
	return count() != (m_size * m_size);
}

/**
	Randomly get an index between 0 and the size of the grid (excluded) 

	@return The random index
*/
int Grid::getRandomIndex()
{
	return rand() % m_size;
}

/**
	[WARNING : Nothing to do here, should be managed by a Ressource Manager]
*/
Font* Grid::getFont()
{
	return m_font;
}

/**
	[WARNING : Nothing to do here, should be managed by a Ressource Manager]
*/
void Grid::setupFont()
{
	if (!m_font) {
		m_font = new Font();

		if (!m_font->loadFromFile("assets\\fonts\\Superfats.ttf")) {
			// debug
			//std::cout << "......... cannot load font ---->" << std::endl;
		}
	}
}

/**
	Computes the proper tile size according to the screen's size and the grid's size

	@return The tile size
*/
float Grid::getTileSize()
{
	return (m_size_pix / (float) m_size);
}

/**
	[NOTE : Maybe should be handled by the AI]
	Randomly get a value between 2 and 4

	@return The random value
*/
int Grid::getRandomValue()
{
	return round((double) rand() / RAND_MAX) == 0 ?
		2 : 4;
}

/**
	Check if the provided tile can move according in the input direction

	@param The tile we try to move
	@return If the tile can move
*/
bool Grid::canMove(Tile* currentTile)
{
	if (currentTile->isGhost()) {
		return false;
	}

	doMerge = false;

	// The tile can not be moved if there's an edge in the trajectory
	// Or if there is a tile it can not be merged with
	Vector2f pos = currentTile->getIndex();

	// Step 1 : check if there is an edge
	if (isThereAnEdge(&pos)) {
		return false;
	}

	// Step 2 : check if there is a neighbour tile
	// If yes, retrieve the tile
	Tile *neighbourTile = getTile((int)pos.x, (int)pos.y);
	if (!isThereANeighbour(neighbourTile)) { // If no neighbour tile, then there is a free space to move to
		return true;
	}

	// Check if the neighbour tile and the current one can be merged
	if (doMerge = currentTile->isMergeable(neighbourTile) && !_isTest) {
		currentTile->setIndex(neighbourTile->getIndex());
	}

	return false;
}

/**
	Check if there is an edge in the input direction
	If not, the function save the targeted tile's coordinates in the provided pos param

	@param pos Coordinates of the tile we try to move 
	@return If there is an edge in the input direction
*/
bool Grid::isThereAnEdge(Vector2f* pos)
{
	switch (m_dir) {
		case DIR_LEFT:			
			if ((int)pos->x == 0) {
				return true;
			}

			pos->x -= 1.0f;
			break;
		case DIR_RIGHT:
			if ((int)pos->x == (m_size - 1)) {
				return true;
			}
			pos->x += 1.0f;
			break;
		case DIR_UP:
			if ((int)pos->y == 0) {
				return true;
			}

			pos->y -= 1.0f;
			break;
		default: // DIR_DOWN			
			if ((int)pos->y == (m_size - 1)) {
				return true;
			}

			pos->y += 1.0f;
	}

	return false;
}

/**
	Check if there is a neighbour tile in the input direction

	@param targetPos Coordinates of the target tile
	@param targetPos Reference to the neighbour tile
	@return If there is a neighbour tile in the input direction
*/
bool Grid::isThereANeighbour(Tile* neighbourTile)
{	
	if (neighbourTile->isGhost()) {
		return false;
	}

	return true;
}

/**
	Save the input direction as LEFT
*/
void Grid::moveLeft()
{
	m_dir = DIR_LEFT;
}

/**
	Save the input direction as RIGHT
*/
void Grid::moveRight()
{
	m_dir = DIR_RIGHT;
}

/**
	Save the input direction as UP
*/
void Grid::moveUp()
{
	m_dir = DIR_UP;
}

/**
	Save the input direction as DOWN
*/
void Grid::moveDown()
{
	m_dir = DIR_DOWN;
}

/**
	Move each tile of the grid in the input direction
	And organize the order we move the tiles in 
	It will not necessarily be the same according to the direction
	That mainly allows us to avoid tiles blocking
*/
void Grid::moveTiles()
{
	if (m_dir == DIR_LEFT) {
		for (int x = 0; x < m_size; x++) {
			for (int y = 0; y < m_size; y++) {
				move(x, y);

				// [TEST CONTEXT]
				// If we have found that a move is possible
				// Break the searching process here
				if (_isTest && _isMovePossible) {
					return;
				}
			}
		}
	}

	else if (m_dir == DIR_RIGHT) {
		for (int x = (m_size - 1); x > -1; x--) {
			for (int y = 0; y < m_size; y++) {
				move(x, y);

				if (_isTest && _isMovePossible) {
					return;
				}
			}
		}
	}

	else if (m_dir == DIR_UP) {
		for (int x = 0; x < m_size; x++) {
			for (int y = 0; y < m_size; y++) {
				move(x, y);

				if (_isTest && _isMovePossible) {
					return;
				}
			}
		}
	}

	else { // (m_dir = DIR_DOWN) 
		for (int x = 0; x < m_size; x++) {
			for (int y = (m_size - 1); y > -1; y--) {
				move(x, y);

				if (_isTest && _isMovePossible) {
					return;
				}
			}
		}
	}

	// [TEST CONTEXT]
	// If here that means no move action will be doable by the player next turn
	// We can stop the process here
	if (_isTest) {
		return;
	}

	m_dir = DIR_NONE;
	unnewTiles();
	refreshTiles();
}

/**
	Move the tile at the provided coordinates in the input direction

	@param x column coordinate
	@param y line coordinate
*/
void Grid::move(int x, int y)
{
	Tile *tile = m_tiles[x][y];
	
	// Those variable are used to store the current tile coordinates as long as the tile CAN MOVE
	// Because canMove() function actually overrides the current tile coordinates in case it can be merged with a neighbour and returns false
	// We can notice a coordinates offset so we make sure to use the right synced ones
	int a = x, b = y;

	// This loop aims to move the tile as long as there is no obstacle in its trajectory
	while (canMove(tile)) {
		// [TEST CONTEXT]
		// We have found that at least one move will be doable by the player next turn
		// We can then save the information and stop the searching process
		if (_isTest) {
			_isMovePossible = true;

			return;
		}

		switch (m_dir) {
		case DIR_LEFT:
			tile->moveLeft();
			break;
		case DIR_RIGHT:
			tile->moveRight();
			break;
		case DIR_UP:
			tile->moveUp();
			break;
		default:
			tile->moveDown();
		}

		a = tile->getX();
		b = tile->getY();
	}

	// [TEST CONTEXT]
	// If a merge is pending that means the player will be able to perform an action
	// We can then stop the process here
	if (doMerge && _isTest) {
		_isMovePossible = true;

		return;
	}

	mergeTiles(a, b, tile);
	doMerge = false;
	refreshTiles();
}

/**
	Called before ending each turn to properly reset the state of each tile
	Marking them as not new
*/
void Grid::unnewTiles()
{
	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			m_tiles[i][j]->setNewlyCreated(false);
			m_tiles[i][j]->setNew(false);
		}
	}
}

/**
	Merge the tile at the provided position with the target one

	@param current_x x coordinate of the current tile
	@param current_y y coordinate of the current tile
	@param t The targeted tile we do the merge with
*/
void Grid::mergeTiles(int current_x, int current_y, Tile* t)
{
	// This doMerge variable is set to true in canMove() function
	if (doMerge) {		
		// The tile to merge
		removeTile(current_x, current_y);
		getTile(current_x, current_y)->refresh();

		// The tile hosting the merge
		t->up();
		setTile(t->getX(), t->getY(), t);
		getTile(t->getX(), t->getY())->refresh();
	}
}

/**
	Get the grid's shape
*/
RectangleShape* Grid::getShape()
{
	return &m_shape;
}

/**
	Get the size of the grid

	@return The grid's size
*/
int Grid::getSize()
{
	return m_size;
}

/**
	Update the grid by setting a tile to provided coordinates

	@param x The x position to set the tile at
	@param y The y position to set the tile at
	@param tile The tile to set
*/
void Grid::setTile(int x, int y, Tile* tile)
{
	m_tiles[x][y] = tile;
}

/**
	Get the tile at the provided coordinates

	@param x The x coordinate
	@param y The x coordinate
	@return The tile at the provided coordinates
*/
Tile* Grid::getTile(int x, int y)
{
	return m_tiles[x][y];
}

/**
	Create a ghost at the provided coordinates

	@param x The x coordinate to create a ghost at
	@param y The x coordinate to create a ghost at
*/
void Grid::removeTile(int x, int y)
{
	m_tiles[x][y] = Tile::createGhost(x, y, this);
}

/**
	Get the number of valued tiles on the grid

	@return The number of valued tiles
*/
int Grid::count()
{
	int count = 0;

	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			if (!(m_tiles[i][j]->isGhost())) {
				++count;
			}
		}
	}

	return count;
}

/*
	Test for each direction if a move can be done for at least one tile
	If yes that means the player can still perform an action for the next turn so the game is not over yet

	@return If a move is doable by the player
*/
bool Grid::isMovePossible()
{
	if (!_isTest) {
		return true;
	}

	// Iterates on each known direction
	for (std::vector<int>::iterator ite = directions.begin(); ite != directions.end(); ++ite) {
		// Save the value of the direction as the current direction
		m_dir = *ite;
		moveTiles();
		
		if (_isMovePossible) {
			resetTestVariables();

			return true;
		}
	}

	resetTestVariables();

	return false;
}

/**
	Used by the engine to turn on TEST context before starting a new turn
*/
void Grid::turnOnTest()
{
	_isTest = true;
}

/**
	Reset TEST context variables after finishing the test/searching process
*/
void Grid::resetTestVariables()
{
	// Reset variables default values
	m_dir = DIR_NONE;
	doMerge = false;
	_isTest = false;
	_isMovePossible = false;
}

/**
	Update each tile of the grid
*/
void Grid::update()
{
	// Update each tile of the grid
	for (int x = 0; x < m_size; x++) {
		for (int y = 0; y < m_size; y++) {
			m_tiles[x][y]->update();
		}
	}
}

/**
	Draw each tile of the grid

	@param w The window instance
*/
void Grid::draw(RenderWindow* w)
{
	// Draw the grid
	w->draw(m_shape);

	// Draw each tile of the grid
	for (int x = 0; x < m_size; x++) {
		for (int y = 0; y < m_size; y++) {
			m_tiles[x][y]->draw(w);
		}
	}
}

/**
	Describe the grid by printing its data as a console output
*/
void Grid::__toString()
{
	std::cout << std::endl << "-------------- DISPLAYING THE GRID" << std::endl << std::endl;

	for (int x = 0; x < m_size; x++) {
		for (int y = 0; y < m_size; y++) {
			m_tiles[x][y]->__toString();
		}
	}

	std::cout << "-------------- END OF DISPLAYING" << std::endl;
}