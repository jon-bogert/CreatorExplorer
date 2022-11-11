#pragma once
class AssetManager
{
	sf::Font font;
	std::map<std::string, sf::Texture> textures;

public:
	static AssetManager& Get();

private:
	AssetManager();

public:
	~AssetManager() {}

	static sf::Font& GetFont();
};

