#include "pch.h"
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Entities/Tile.h"
#include <iostream>
#include <string>

using namespace sf;

Tile::Tile(int v, int x, int y, Grid* g)
{
	m_value = v;
	m_index = Vector2f(x, y);
	m_isGhost = true;
	m_isNew = false;
	m_isNewlyCreated = false;
	m_grid = g;
		
	setupShape();
	setupText();
	setupOrigins();
	setupPosition(x, y);
	setupShapePosition();
	setupTextPosition();
	setupTileColor();
}

Tile* Tile::createRandom(int x, int y, Grid* g)
{
	Tile* randomTile = new Tile(Grid::getRandomValue(), x, y, g);
	randomTile->setGhost(false);
	randomTile->setNewlyCreated(true);
	randomTile->refresh();

	return randomTile;
}

Tile* Tile::createGhost(int x, int y, Grid* g)
{
	return new Tile(GHOST_VAL, x, y, g);
}

void Tile::setupOrigins()
{
	// set shape origin
	m_shape.setOrigin(
		m_grid->getShape()->getOrigin()
	);

	// set text origin
	m_text.setOrigin(
		m_grid->getShape()->getOrigin()
	);
}

void Tile::setupShape()
{
	m_shape = RectangleShape(
		Vector2f(
			m_grid->getTileSize(),
			m_grid->getTileSize()
		)
	);
}

void Tile::setupText()
{
	updateText();
}

void Tile::updateText()
{
	m_text.setFont(*m_grid->getFont());
	m_text.setString(getTextString());
	m_text.setCharacterSize(getTextSize());
	m_text.setFillColor(Color::White);

	if (m_isNewlyCreated) {
		m_text.setFillColor(Color::Red);
	}
}

void Tile::setupPosition(int x, int y)
{
	float x_offset = m_grid->getTileSize() * x;
	float y_offset = m_grid->getTileSize() * y;

	// set tile position
	m_position = Vector2f(
		m_grid->getShape()->getPosition().x + x_offset,
		m_grid->getShape()->getPosition().y + y_offset
	);
}

void Tile::setupShapePosition()
{
	// set shape position
	m_shape.setPosition(m_position);
}

void Tile::setupTextPosition()
{
	FloatRect textRect = m_text.getLocalBounds();

	m_text.setPosition(m_position);

	m_text.move(
		(m_shape.getSize().x / 2.0f) - (textRect.width / 2.0f) - (textRect.left/2.0f),
		(m_shape.getSize().y / 2.0f) - (textRect.height / 2.0f) - (textRect.top / 2.0f)
	);	
}

std::string Tile::getTextString()
{
	if (m_value == GHOST_VAL) {
		return std::string(TEXT_STRING_GHOST);
	}

	return std::to_string(m_value);
}

unsigned int Tile::getTextSize()
{
	// 1024, 2048
	if (m_value > 512) {
		return TEXT_SIZE_4DIGIT;
	}

	// 128, 256, 512
	if (m_value > 64) { 
		return TEXT_SIZE_3DIGIT;
	}

	if (m_value == GHOST_VAL) {
		return TEXT_SIZE_GHOST;
	}

	// 2, 4, 16, 32, 64
	return TEXT_SIZE_STD;
}

void Tile::setupTileColor()
{
	switch (m_value) {
		case 2:
			m_shape.setFillColor(TILE_COLOR_2);
			break;
		case 4:
			m_shape.setFillColor(TILE_COLOR_4);
			break;
		case 8:
			m_shape.setFillColor(TILE_COLOR_8);
			break;
		case 16:
			m_shape.setFillColor(TILE_COLOR_16);
			break;
		case 32:
			m_shape.setFillColor(TILE_COLOR_32);
			break;
		case 64:
			m_shape.setFillColor(TILE_COLOR_64);
			break;
		case 128:
			m_shape.setFillColor(TILE_COLOR_128);
			break;
		case 256:
			m_shape.setFillColor(TILE_COLOR_256);
			break;
		case 512:
			m_shape.setFillColor(TILE_COLOR_512);
			break;
		case 1024:
			m_shape.setFillColor(TILE_COLOR_1024);
			break;
		case 2048:
			m_shape.setFillColor(TILE_COLOR_2048);
			break;
		default: // GHOST
			m_shape.setFillColor(TILE_COLOR_GHOST);
	}
}

void Tile::moveLeft()
{
	if ((int)getX() <= 0) {
		return;
	}

	setX(getX() - 1);
	setPosX(getPosX() - m_grid->getTileSize());

	m_grid->setTile(getX(), getY(), this);
	m_grid->removeTile(getX() + 1, getY());
}

void Tile::moveRight()
{
	if ((int)getX() >= (m_grid->getSize() - 1)) {
		return;
	}

	setX(getX() + 1);
	setPosX(getPosX() + m_grid->getTileSize());

	m_grid->setTile(getX(), getY(), this);
	m_grid->removeTile(getX() - 1, getY());
}

void Tile::moveUp()
{
	if ((int)getY() <= 0) {
		return;
	}

	setY(getY() - 1);
	setPosY(getPosY() - m_grid->getTileSize());

	m_grid->setTile(getX(), getY(), this);
	m_grid->removeTile(getX(), getY() + 1);
}

void Tile::moveDown()
{
	if ((int)getY() >= (m_grid->getSize() - 1)) {
		return;
	}

	setY(getY() + 1);
	setPosY(getPosY() + m_grid->getTileSize());

	m_grid->setTile(getX(), getY(), this);
	m_grid->removeTile(getX(), getY() - 1);
}

void Tile::refresh()
{
	update();
}

Vector2f Tile::getPosition()
{
	return m_position;
}

Vector2f Tile::getIndex()
{
	return m_index;
}

int Tile::getX()
{
	return (int)m_index.x;
}

int Tile::getY()
{
	return (int)m_index.y;
}

float Tile::getPosX()
{
	return m_position.x;
}

float Tile::getPosY()
{
	return m_position.y;
}

int Tile::getValue()
{
	return m_value;
}

void Tile::setIndex(Vector2f index)
{
	m_index = index;
}

void Tile::setIndex(int x, int y)
{
	m_index.x = (float)x;
	m_index.y = (float)y;
}

void Tile::setX(int x)
{
	m_index.x = (float)x;
}

void Tile::setY(int y)
{
	m_index.y = (float)y;
}

void Tile::setPosX(float posX)
{
	m_position.x = posX;
}

void Tile::setPosY(float posY)
{
	m_position.y = posY;
}

void Tile::setGhost(bool isGhost)
{
	m_isGhost = isGhost;

	if (!isGhost) {
		int randomValue = Grid::getRandomValue();
		m_value = randomValue;
	}
}

bool Tile::isGhost()
{
	return m_isGhost;
}

void Tile::setNew(bool isNew)
{
	m_isNew = isNew;
}

bool Tile::isNew()
{
	return m_isNew;
}

void Tile::setNewlyCreated(bool isNewlyCreated)
{
	m_isNewlyCreated = isNewlyCreated;
}

bool Tile::isNewlyCreated()
{
	return m_isNewlyCreated;
}

bool Tile::isMergeable(Tile* targetTile)
{
	return targetTile->getValue() == m_value 
		&& !(targetTile->isNew());
}

void Tile::up()
{
	// multiplicate the tile value by 2
	m_value *= 2;
	// marks the tile as resulting of a merge
	setNew(true);
}

void Tile::update()
{
	setupPosition(getX(), getY());
	setupShapePosition();
	setupText();
	setupTextPosition();
	setupTileColor();
}

void Tile::draw(RenderWindow* w)
{
	w->draw(m_shape);
	w->draw(m_text);
}

void Tile::__toString()
{
	std::cout << printf("Tile[%d][%d]...", (int)m_index.x, (int)m_index.y) << std::endl;
	std::cout << "Ghost ? ";
	if (m_isGhost) {
		std::cout << "YES" << std::endl;
	}
	else {
		std::cout << "NO" << std::endl;
	}
	std::cout << "Newly-created ? ";
	if (m_isNewlyCreated) {
		std::cout << "YES" << std::endl;
	}
	else {
		std::cout << "NO" << std::endl;
	}
	std::cout << "Resulting of a merge ? ";
	if (m_isNew) {
		std::cout << "YES" << std::endl;
	}
	else {
		std::cout << "NO" << std::endl;
	}
	std::cout << printf("Value = %d...", (int)m_value) << std::endl;
	std::cout << printf("Pos.x = %d...", (int)getPosX()) << std::endl;
	std::cout << printf("Pos.y = %d...", (int)getPosY()) << std::endl;
	std::cout << "Text string = " << std::string(m_text.getString()) << std::endl;
	std::cout << "Text size = " << m_text.getCharacterSize() << std::endl;
	std::cout << "Text font = " << m_text.getFont()->getInfo().family << std::endl << std::endl;
}