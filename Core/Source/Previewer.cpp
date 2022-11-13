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
	loadingText.setFont(AssetManager::GetFont());
	loadingText.setCharacterSize(24);
	loadingText.setFillColor(sf::Color::White);
	loadingText.setString("Loading...");
	loadingText.setPosition({(defSize.x / 2.f) - (loadingText.getLocalBounds().width / 2.f), (defSize.y / 2.f) - (loadingText.getLocalBounds().height / 2.f) });
}

Previewer::~Previewer()
{
	delete audioLong;
	delete audioShort;
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
		default:
			DefaultUpdate();
		}
	}
}

void Previewer::DefaultUpdate()
{
	if (window)
	{
		window->clear(sf::Color::Black);
		window->display();
	}
}

void Previewer::ImageUpdate()
{
	if (window)
	{
		window->clear(sf::Color::Black);
		window->draw(*sprite);
		window->display();
	}
}

void Previewer::AudioUpdate()
{
	if (isAudioStreaming) UpdateMusicStream(*audioLong);
	if (window)
	{
		window->clear(sf::Color::Black);
		window->display();
	}
}

void Previewer::LoadFile()
{
	if (isActive)
	{
		Reset();
		int currItem = Application::Get().GetCurrItem();
		if (currItem <= -1)
		{
			LoadRoot();
			return;
		}
		
		item = Browser::Get().GetItem(currItem);

		switch (item->GetType())
		{
		case ItemType::Image:
			LoadImage();
			break;
		case ItemType::Audio:
			LoadAudio();
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
	InitAudioDevice();
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(defSize.x, defSize.y), "CreatorExplorer - Previewer");
	LoadFile();
}

void Previewer::Deactivate()
{
	Reset();
	CloseAudioDevice();
	window = nullptr;
}

void Previewer::Reset()
{
	//Image
	item = nullptr;

	texture = nullptr;
	sprite = nullptr;

	if (window->getSize().x != defSize.x || window->getSize().y != defSize.y)
		window = std::make_shared<sf::RenderWindow>(sf::VideoMode(defSize.x, defSize.y), "CreatorExplorer - Previewer");

	//Audio
	if (audioLong)
	{
		StopMusicStream(*audioLong);
		delete audioLong;
		audioLong = nullptr;
	}

	//Loading Display
	window->clear(bgColor);
	window->draw(loadingText);
	window->display();
}

void Previewer::LoadRoot()
{
}

void Previewer::LoadDefault()
{
}

void Previewer::LoadImage()
{
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

	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), "CreatorExplorer - Previewer");

	sprite = std::make_shared<sf::Sprite>();
	sprite->setTexture(*texture);
	sprite->setScale(scale, scale);
}

void Previewer::LoadAudio()
{
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
