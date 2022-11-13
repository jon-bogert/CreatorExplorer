#include "Pch.h"
#include "AssetManager.h"

AssetManager& AssetManager::Get()
{
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager()
{
    //FONT
    font.loadFromFile("assets/font.ttf");

    //TEXTURES
    textures.insert({ "back", std::make_shared<sf::Texture>() });
    textures["back"]->loadFromFile("assets/back.png");

    textures.insert({ "doc", std::make_shared<sf::Texture>() });
    textures["doc"]->loadFromFile("assets/doc.png");

    textures.insert({ "drive", std::make_shared<sf::Texture>() });
    textures["drive"]->loadFromFile("assets/drive.png");

    textures.insert({ "file", std::make_shared<sf::Texture>() });
    textures["file"]->loadFromFile("assets/file.png");

    textures.insert({ "folder", std::make_shared<sf::Texture>() });
    textures["folder"]->loadFromFile("assets/folder.png");

    textures.insert({ "music", std::make_shared<sf::Texture>() });
    textures["music"]->loadFromFile("assets/music.png");

    textures.insert({ "picture", std::make_shared<sf::Texture>() });
    textures["picture"]->loadFromFile("assets/picture.png");

    textures.insert({ "video", std::make_shared<sf::Texture>() });
    textures["video"]->loadFromFile("assets/video.png");
}

sf::Font& AssetManager::GetFont()
{
    return Get().font;
}

sf::Texture& AssetManager::GetTexture(std::string key)
{
    return *Get().textures[key];
}
