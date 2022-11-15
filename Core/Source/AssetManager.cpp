#include "Pch.h"
#include "AssetManager.h"

extern std::string res;

AssetManager& AssetManager::Get()
{
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager()
{
    //FONT
    font.loadFromFile(res + "assets/font.ttf");

    //TEXTURES
    textures.insert({ "back", std::make_shared<sf::Texture>() });
    textures["back"]->loadFromFile(res + "assets/back.png");

    textures.insert({ "doc", std::make_shared<sf::Texture>() });
    textures["doc"]->loadFromFile(res + "assets/doc.png");

    textures.insert({ "drive", std::make_shared<sf::Texture>() });
    textures["drive"]->loadFromFile(res + "assets/drive.png");

    textures.insert({ "file", std::make_shared<sf::Texture>() });
    textures["file"]->loadFromFile(res + "assets/file.png");

    textures.insert({ "folder", std::make_shared<sf::Texture>() });
    textures["folder"]->loadFromFile(res + "assets/folder.png");

    textures.insert({ "music", std::make_shared<sf::Texture>() });
    textures["music"]->loadFromFile(res + "assets/music.png");

    textures.insert({ "picture", std::make_shared<sf::Texture>() });
    textures["picture"]->loadFromFile(res + "assets/picture.png");

    textures.insert({ "video", std::make_shared<sf::Texture>() });
    textures["video"]->loadFromFile(res + "assets/video.png");
}

sf::Font& AssetManager::GetFont()
{
    return Get().font;
}

sf::Texture& AssetManager::GetTexture(std::string key)
{
    return *Get().textures[key];
}
