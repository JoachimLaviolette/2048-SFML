#include "pch.h"
#include "Engine/Engine.h"
#include "Constants.h"

Engine::Engine()
{
	// Get the screen resolution and create an SFML window and the view
	Vector2f resolution;

	// Get the screen width & height
	resolution.x = VideoMode::getDesktopMode().width / 2;
	resolution.y = VideoMode::getDesktopMode().height / 2;

	// Setup render window's settings
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Disable key repeating
	m_window.setKeyRepeatEnabled(false);

	m_window.create(
		VideoMode(resolution.x, resolution.y),
		"2048 with SFML",
		Style::Fullscreen,
		settings
	);

	// Instantiate game entities
	m_grid = Grid::createGrid(AI_HARD);
	// A vector that helps us to know if the user can move in any direction during the current turn
	dirDataBuffer = std::vector<int>(4);
}

/**
	Start function called to launch the game
*/
void Engine::start()
{
	while (m_window.isOpen()) {
		input();
		update();
		draw();

		// No more move is possible so the game is over
		if (!m_grid->isMovePossible()) {
			m_window.close();
		}
	}

	// Window has been closed
}