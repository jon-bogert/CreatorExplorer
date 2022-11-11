#include "Pch.h"
#include "AssetManager.h"

AssetManager& AssetManager::Get()
{
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager()
{
    font.loadFromFile("assets/font.ttf");
}

sf::Font& AssetManager::GetFont()
{
    return Get().font;
}
