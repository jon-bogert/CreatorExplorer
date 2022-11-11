#include "Pch.h"
#include "InputManager.h"
#include "Browser.h"
#include "Previewer.h"

InputManager& InputManager::Get()
{
    static InputManager instance;
    return instance;
}

void InputManager::Update()
{
	sf::Event windowEvent;
	std::shared_ptr<sf::RenderWindow> window = Browser::Get().GetWindow();
	if (Previewer::Get().GetIsActive() && Previewer::Get().GetWindow()->hasFocus())
		window = Previewer::Get().GetWindow();

	Reset();

	while (window->pollEvent(windowEvent))
	{
		if (windowEvent.type == sf::Event::Closed)
		{
			window->close();
		}
		if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Up)
			arrowUp = true;
		if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Down)
			arrowDown = true;
		if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Enter)
			select = true;
		if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Backspace)
			back = true;
		if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Space)
			preview = true;
	}
}

bool InputManager::ArrowUp() const
{
    return arrowUp;
}

bool InputManager::ArrowDown() const
{
    return arrowDown;
}

bool InputManager::Select() const
{
    return select;
}

bool InputManager::Back() const
{
	return back;
}

bool InputManager::Preview() const
{
    return preview;
}

void InputManager::Reset()
{
	arrowUp = false;
	arrowDown = false;
	select = false;
	back = false;
	preview = false;
}
