#ifndef TILE_H
#define TILE_H

#include "Entities/Grid.h"

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class Grid;

class Tile
{
private:
	Tile(int v, int x, int y, Grid* g);
	int m_value;
	Vector2f m_position;
	Vector2f m_index;
	RectangleShape m_shape;
	Grid* m_grid;
	Text m_text;
	
	bool m_isGhost;
	bool m_isNew; // if the tile is created for the new turn
	bool m_isNewlyCreated; // if the tile is resulting of a merge (FOR THE CURRENT TURN !!)
	
	void setupShape();
	void setupText();
	void updateText();
	void setupOrigins();
	void setupPosition(int x, int y);
	void setupShapePosition();
	void setupTextPosition();
	void setupTileColor();
	unsigned int getTextSize();

public:
	static Tile* createRandom(int x, int y, Grid* g);
	static Tile* createGhost(int x, int y, Grid* g);
	
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void refresh();

	Vector2f getPosition();
	Vector2f getIndex();
	int getX();
	int getY();
	float getPosX();
	float getPosY();
	int getValue();
	std::string getTextString();
	bool isGhost();
	bool isNew();
	bool isNewlyCreated();
	bool isMergeable(Tile* targetTile);

	void setIndex(Vector2f index);
	void setIndex(int x, int y);
	void setX(int x);
	void setY(int y);
	void setPosX(float posX);
	void setPosY(float posY);
	void setGhost(bool isGhost);
	void setNew(bool isNew);
	void setNewlyCreated(bool isNewlyCreated);
	void up(); // set value and text

	void update();
	void draw(RenderWindow* w);

	void __toString();
};

#endif
