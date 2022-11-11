#pragma once
class Item;
class Previewer
{
	std::shared_ptr<sf::RenderWindow> window = nullptr;
	bool isActive = false;

	std::shared_ptr<Item> item = nullptr;

	std::shared_ptr<sf::Texture> texture = nullptr;
	std::shared_ptr<sf::Sprite> sprite = nullptr;

	sf::Vector2f defSize = { 400.f, 225.f };
	sf::Vector2f maxSize = { 1920.f, 1080.f };

public:
	static Previewer& Get();

private:
	Previewer() {}

public:
	~Previewer() {}

	bool ToggleWindow();
	void Update();

	void LoadFile();
	
	bool GetIsActive();

	std::shared_ptr<sf::RenderWindow> GetWindow();

private:
	void Activate();
	void Deactivate();
	void Reset();

	void LoadRoot();
	void LoadDefault(std::shared_ptr<Item> item);
	void LoadImage(std::shared_ptr<Item> item);
};

