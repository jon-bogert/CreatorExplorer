#pragma once
class Item;
class Browser
{
	std::shared_ptr<sf::RenderWindow> window;

	//Buffers
	std::vector<std::shared_ptr<Item>> itemBuffer;
	float spacing = 25.f;
	bool triggerDraw = false;

	sf::Vector2u resolution = { 500, 1080 };

	int itemsVisible{}; // set in constructor
	int scrollOffset = 0.f;

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

	int GetNumItems() const;

	float GetSpacing() const;

	int GetItemsVisible() const;

	int GetScrollOffset() const;
	void AddScrollOffset(const int add);
	void SetScrollOffset(const int setTo);
	
	void Draw();

	bool IsOpen();
	void CloseWindow();
	std::shared_ptr<sf::RenderWindow> GetWindow();

	void TriggerDraw();

private:
	void CheckScrollOffset();

};

