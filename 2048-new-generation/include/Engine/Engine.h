#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include "Entities/Grid.h"

using namespace sf;

class Engine
{
private:
	RenderWindow m_window;
	Grid* m_grid;
	std::vector<int> dirDataBuffer;
	bool wasActionKeyPressed = false;

	bool isMoveKeyPressed();
	bool isActionKeyPressed();

	void screenshot();
	long uniqueID();
	std::string getFilename(const char* module, const char* extension);

	void input();
	void update();
	void draw();

public:
	Engine();

	void start();
};

#endif