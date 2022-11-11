#pragma once
class InputManager
{
	bool arrowUp = false;
	bool arrowDown = false;
	bool select = false;
	bool back = false;
	bool preview = false;

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

private:
	void Reset();
};

