#pragma once

enum class ItemType { Folder, File, Text, Image, Audio, Video };

class Item
{
	bool isSelected = false;
	ItemType type = ItemType::Folder;

	sf::Sprite icon;
	sf::Text text;
	std::string name;
	std::string extention;
	float textSize = 12.f;
	float boxLength = 500.f;

	sf::RectangleShape border;

	sf::Vector2f textOffset = { 5, 5 };

public:
	Item(std::string name, std::string extention = "");

	sf::Text& GetText();
	void SetIsSelected(bool onOff);

	bool GetIsFolder();
	std::string GetName();
	ItemType GetType() const;

	void Draw(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f position);
};

