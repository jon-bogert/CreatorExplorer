#include "Pch.h"
#include "Item.h"
#include "Previewer.h"
#include "InputManager.h"
#include "Browser.h"
#include "Application.h"
#include "AssetManager.h"


Previewer& Previewer::Get()
{
	static Previewer instance;
	return instance;
}

Previewer::Previewer()
{
	InitAudioDevice();
	SetMasterVolume(1.f);

	loadingText.setFont(AssetManager::GetFont());
	loadingText.setCharacterSize(24);
	loadingText.setFillColor(sf::Color::White);
	loadingText.setString("Loading...");
	loadingText.setPosition({(defSize.x / 2.f) - (loadingText.getLocalBounds().width / 2.f), (defSize.y / 2.f) - (loadingText.getLocalBounds().height / 2.f) });

	float iconBuffer = (defSize.y / 2) - 64;
	icon.setPosition(75, iconBuffer);
	icon.setScale(0.5f, 0.5f);
}

Previewer::~Previewer()
{
	delete audioLong;
	delete audioShort;

	CloseAudioDevice();
}

bool Previewer::ToggleWindow()
{
	if (!isActive)
	{
		isActive = true;
		Activate();
	}
	else
	{
		isActive = false;
		Deactivate();
	}
	return isActive;
}

void Previewer::Update()
{
	if (isActive)
	{
		switch (item->GetType())
		{
		case ItemType::Image:
			ImageUpdate();
			break;
		case ItemType::Audio:
			AudioUpdate();
			break;
		case ItemType::Text:
			TextUpdate();
			break;
		default:
			DefaultUpdate();
		}
		if (!window->isOpen())
		{
			isActive = false;
			Deactivate();
		}
	}
}

void Previewer::DefaultUpdate()
{
	if (window)
	{
		window->clear(bgColor);
		window->draw(icon);
		window->display();
	}
}

void Previewer::ImageUpdate()
{
	if (window)
	{
		window->clear(bgColor);
		window->draw(*sprite);
		window->display();
	}
}

void Previewer::AudioUpdate()
{
	if (isAudioStreaming) UpdateMusicStream(*audioLong);
	if (window)
	{
		window->clear(bgColor);
		window->draw(icon);
		window->display();
	}
}

void Previewer::TextUpdate()
{
	window->clear(bgColor);
	window->draw(textContents);
	window->display();
}

void Previewer::LoadFile()
{
	if (isActive)
	{
		//window->requestFocus();
		Reset();
		int currItem = Application::Get().GetCurrItem();
		
		item = Browser::Get().GetItem(currItem);

		switch (item->GetType())
		{
		case ItemType::Image:
			LoadImage();
			break;
		case ItemType::Audio:
			LoadAudio();
			break;
		case ItemType::Text:
			LoadText();
			break;
		default:
			LoadDefault();
		}
	}
}

bool Previewer::GetIsActive()
{
	return isActive;
}

std::shared_ptr<sf::RenderWindow> Previewer::GetWindow()
{
	return window;
}

void Previewer::Activate()
{
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(defSize.x, defSize.y), "CreatorExplorer - Previewer", sf::Style::Close);
	LoadFile();
}

void Previewer::Deactivate()
{
	Reset();
	window = nullptr;
}

void Previewer::Reset()
{
	//Image
	item = nullptr;

	texture = nullptr;
	sprite = nullptr;

	if (window->getSize().x != defSize.x || window->getSize().y != defSize.y)
		window = std::make_shared<sf::RenderWindow>(sf::VideoMode(defSize.x, defSize.y), "CreatorExplorer - Previewer", sf::Style::Close);

	//Audio
	if (audioLong)
	{
		StopMusicStream(*audioLong);
		UnloadMusicStream(*audioLong);
		delete audioLong;
		audioLong = nullptr;
	}

	//Loading Display
	window->clear(bgColor);
	window->draw(icon);
	window->draw(loadingText);
	window->display();
}

void Previewer::LoadRoot()
{
	icon.setTexture(AssetManager::GetTexture("folder"));
}

void Previewer::LoadDefault()
{
	if (item->GetType() == ItemType::Folder)
		icon.setTexture(AssetManager::GetTexture("folder"));
	else
		icon.setTexture(AssetManager::GetTexture("file"));
}

void Previewer::LoadImage()
{
	icon.setTexture(AssetManager::GetTexture("picture"));

	texture = std::make_shared<sf::Texture>();
	texture->loadFromFile(Application::Get().PathStr() + item->GetName());

	float width = texture->getSize().x;
	float height = texture->getSize().y;
	float scale = 1.f;
	if (height > maxSize.y)
	{
		scale = maxSize.y / texture->getSize().y;
		width = texture->getSize().x * scale;
		height = texture->getSize().y * scale;
	}
	if (width > maxSize.x)
	{
		scale = maxSize.x / texture->getSize().x;
		width = texture->getSize().x * scale;
		height = texture->getSize().y * scale;
	}

	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), "CreatorExplorer - Previewer", sf::Style::Close);

	sprite = std::make_shared<sf::Sprite>();
	sprite->setTexture(*texture);
	sprite->setScale(scale, scale);
}

void Previewer::LoadAudio()
{
	icon.setTexture(AssetManager::GetTexture("music"));

	if (audioLong)
	{
		StopMusicStream(*audioLong);
		UnloadMusicStream(*audioLong);
		delete audioLong;
		audioLong = nullptr;
	}
	if (audioShort)
	{
		UnloadSound(*audioShort);
		delete audioShort;
		audioShort = nullptr;
	}

	std::string filePath = Application::Get().PathStr() + item->GetName();
	isAudioStreaming = true;

	audioLong = new Music;
	*audioLong = LoadMusicStream(filePath.c_str());
	if (GetMusicTimeLength(*audioLong) <= streamLimit)
	{
		UnloadMusicStream(*audioLong); delete audioLong; audioLong = nullptr;
		audioShort = new Sound;
		*audioShort = LoadSound(filePath.c_str());
		isAudioStreaming = false;
	}
	
	(isAudioStreaming) ? PlayMusicStream(*audioLong) : PlaySound(*audioShort);
}

void Previewer::LoadText()
{
	icon.setTexture(AssetManager::GetTexture("doc"));

	std::string textStr;
	std::ifstream file;
	file.open(Application::Get().PathStr() + item->GetName());

	std::string lineBuffer;
	while (std::getline(file, lineBuffer))
	{
		textStr.append(lineBuffer + "\n");
	}

	textContents.setFont(AssetManager::GetFont());
	textContents.setCharacterSize(12);
	textContents.setFillColor({ 218, 218, 218, 255 });
	textContents.setString(textStr);
}
