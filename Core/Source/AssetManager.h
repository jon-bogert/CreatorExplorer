#pragma once
class AssetManager
{
	sf::Font font;
	std::map<std::string, std::shared_ptr<sf::Texture>> textures;

public:
	static AssetManager& Get();

private:
	AssetManager();

public:
	~AssetManager() {}

	static sf::Font& GetFont();
	static sf::Texture& GetTexture(std::string key);
};

