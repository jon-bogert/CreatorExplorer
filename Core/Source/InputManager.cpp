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

		//Triggers
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
		if (windowEvent.type == sf::Event::MouseButtonPressed && windowEvent.mouseButton.button == sf::Mouse::Button::Left)
		{
			mouseClick = true;
			if (clickTimer.getElapsedTime().asSeconds() < doubleClickLimit)
				mouseDoubleClick = true;
			clickTimer.restart();
		}

		//Axis
		if (windowEvent.type == sf::Event::MouseWheelScrolled)
			scrollDelta = windowEvent.mouseWheelScroll.delta;

		//2D-Axis
		if (windowEvent.type == sf::Event::MouseMoved)
			mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
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

bool InputManager::MouseClick() const
{
	return mouseClick;
}

bool InputManager::MouseDoubleClick() const
{
	return mouseDoubleClick;
}

int InputManager::ScrollDelta() const
{
	return scrollDelta;
}

sf::Vector2f InputManager::MousePos() const
{
	return mousePos;
}

void InputManager::Reset()
{
	arrowUp = false;
	arrowDown = false;
	select = false;
	back = false;
	preview = false;
	mouseClick = false;
	mouseDoubleClick = false;

	scrollDelta = 0;
}
