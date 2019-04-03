#include "pch.h"
#include "Engine/Engine.h"
#include <iostream>
#include <chrono>
#include <ctime>    

bool Engine::isActionKeyPressed()
{
	return isMoveKeyPressed()
		|| Keyboard::isKeyPressed(Keyboard::Escape)
		|| Keyboard::isKeyPressed(Keyboard::S);
}

bool Engine::isMoveKeyPressed()
{
	return Keyboard::isKeyPressed(Keyboard::Left)
		|| Keyboard::isKeyPressed(Keyboard::Right)
		|| Keyboard::isKeyPressed(Keyboard::Up)
		|| Keyboard::isKeyPressed(Keyboard::Down);
}

void Engine::input()
{
	if (isActionKeyPressed()) {
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			m_window.close();
			// Handle start menu here

			// Block multiple events
			wasActionKeyPressed = true;
		}

		if (isMoveKeyPressed() && !wasActionKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				m_grid->moveLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				m_grid->moveRight();
			}

			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				m_grid->moveUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				m_grid->moveDown();
			}

			// Move the grid's tiles
			m_grid->moveTiles();

			// Turn is over so we generate a new tile randomly on the grid
			m_grid->newTile();

			// Turn on test context
			m_grid->turnOnTest();

			// Block multiple events
			wasActionKeyPressed = true;
		}

		// Screenshot
		if (Keyboard::isKeyPressed(Keyboard::S) && !wasActionKeyPressed) {
			screenshot();
		}
	}
	else {
		wasActionKeyPressed = false;
	}
}

void Engine::screenshot()
{
	sf::Vector2u windowSize = m_window.getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(m_window);
	sf::Image screenshot = texture.copyToImage();
	std::string filename = getFilename("screenshots", "png");
	screenshot.saveToFile(filename);

	// Block multiple events
	wasActionKeyPressed = true;
}

long Engine::uniqueID()
{
	return rand() % LONG_MAX;
}

std::string Engine::getFilename(const char* module, const char* extension)
{
	std::string filename = std::string(module);
	filename += "\\";
	filename += filename.substr(0, filename.size() - 2);
	filename += "_";
	filename += std::to_string(uniqueID());
	filename += "." + std::string(extension);

	return filename;
}
