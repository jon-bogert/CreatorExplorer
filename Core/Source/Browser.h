#pragma once
class Item;
class Browser
{
	std::shared_ptr<sf::RenderWindow> window;

	//Buffers
	std::vector<std::shared_ptr<Item>> itemBuffer;
	float spacing = 25.f;
	bool triggerDraw = false;

public:
	static Browser& Get();

private:
	Browser();

public:
	~Browser() {}

	int SearchItem(std::shared_ptr<Item> item);
	bool AddItem(std::shared_ptr<Item> item);
	bool RemoveItem(std::shared_ptr<Item> item);
	void ClearItems();
	std::shared_ptr<Item> GetItem(int index);
	size_t NumItems();

	void SetIsSelected(size_t index, bool onOff);
	std::string GetName(size_t index);
	bool GetIsFolder(size_t index);
	bool IsEmpty();
	
	void Draw();

	bool IsOpen();
	void CloseWindow();
	std::shared_ptr<sf::RenderWindow> GetWindow();

	void TriggerDraw();

};

