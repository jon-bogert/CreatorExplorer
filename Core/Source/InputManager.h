#pragma once
class InputManager
{
	bool arrowUp = false;
	bool arrowDown = false;
	bool select = false;
	bool back = false;
	bool preview = false;
	bool mouseClick = false;
	bool mouseDoubleClick = false;
	bool prevClick = false;

	int scrollDelta = 0;

	sf::Vector2f mousePos = { 0, 0 };

	sf::Clock clickTimer{};
	float doubleClickLimit = 0.5f;

public:
	static InputManager& Get();

private:
	InputManager() {}

public:
	~InputManager() {}

	void Update();

	bool ArrowUp() const;
	bool ArrowDown() const;
	bool Select() const;
	bool Back() const;
	bool Preview() const;
	bool MouseClick() const;
	bool MouseDoubleClick() const;
	bool PrevClick() const;

	int ScrollDelta() const;

	sf::Vector2f MousePos() const;

private:
	void Reset();
};

