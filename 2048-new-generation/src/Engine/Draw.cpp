#include "pch.h"
#include "Engine/Engine.h"

void Engine::draw()
{
	// Rub out the last frame
	m_window.clear(Color::Black);
	
	// Draw the grid and its sub-components
	m_grid->draw(&m_window);

	// Show everything we have just drawn
	m_window.display();
}