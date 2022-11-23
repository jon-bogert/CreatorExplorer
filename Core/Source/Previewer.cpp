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
	rl::InitAudioDevice();
	rl::SetMasterVolume(1.f);

	loadingText.setFont(AssetManager::GetFont());
	loadingText.setCharacterSize(24);
	loadingText.setFillColor(sf::Color::White);
	loadingText.setString("Loading...");
	loadingText.setPosition({(defSize.x / 2.f) - (loadingText.getLocalBounds().width / 2.f), (defSize.y / 2.f) - (loadingText.getLocalBounds().height / 2.f) });

	fileNameText.setFont(AssetManager::GetFont());
	fileNameText.setCharacterSize(24);
	fileNameText.setFillColor(sf::Color::White);
	fileNameText.setString("FileName.ext");
	fileNameText.setPosition(fileNamePos);

	fileTypeText.setFont(AssetManager::GetFont());
	fileTypeText.setCharacterSize(18);
	fileTypeText.setFillColor(sf::Color::White);
	fileTypeText.setString("FileType");
	fileTypeText.setPosition(fileTypePos);

	float iconBuffer = (defSize.y / 2) - 64;
	icon.setPosition(75, iconBuffer);
	icon.setScale(0.5f, 0.5f);

	playheadBg.setSize(playheadDim);
	playheadBg.setPosition(playheadPos);
	playheadBg.setFillColor(playheadBgColor);
	playheadMain.setSize(playheadDim);
	playheadMain.setPosition(playheadPos);
	playheadMain.setFillColor(playheadMainColor);
}

Previewer::~Previewer()
{
	delete audioLong;
	delete audioShort;

	rl::CloseAudioDevice();
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
		window->draw(fileNameText);
		window->draw(fileTypeText);
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
	window->clear(bgColor);
	if (isAudioStreaming)
	{
		rl::UpdateMusicStream(*audioLong);

		if (InputManager::Get().PrevClick() && InPlayheadBounds())
		{
			float seekTo = ((InputManager::Get().MousePos().x - playheadPos.x) / playheadDim.x) * rl::GetMusicTimeLength(*audioLong);
			rl::SeekMusicStream(*audioLong, seekTo);
		}

		playheadMain.setScale({ rl::GetMusicTimePlayed(*audioLong) / rl::GetMusicTimeLength(*audioLong) ,1.f});
		window->draw(playheadBg);
		window->draw(playheadMain);
	}
	if (window)
	{
		window->draw(fileNameText);
		window->draw(fileTypeText);
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
		fileNameText.setString(item->GetName());

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
		rl::StopMusicStream(*audioLong);
		rl::UnloadMusicStream(*audioLong);
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
	{
		icon.setTexture(AssetManager::GetTexture("folder"));
		fileTypeText.setString("Folder");
	}
	else
	{
		icon.setTexture(AssetManager::GetTexture("file"));
		fileTypeText.setString("File");
	}
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
	fileTypeText.setString("Audio");

	if (audioLong)
	{
		rl::StopMusicStream(*audioLong);
		rl::UnloadMusicStream(*audioLong);
		delete audioLong;
		audioLong = nullptr;
	}
	if (audioShort)
	{
		rl::UnloadSound(*audioShort);
		delete audioShort;
		audioShort = nullptr;
	}

	std::string filePath = Application::Get().PathStr() + item->GetName();
	isAudioStreaming = true;

	audioLong = new rl::Music;
	*audioLong = rl::LoadMusicStream(filePath.c_str());
	if (rl::GetMusicTimeLength(*audioLong) <= streamLimit)
	{
		rl::UnloadMusicStream(*audioLong); delete audioLong; audioLong = nullptr;
		audioShort = new rl::Sound;
		*audioShort = rl::LoadSound(filePath.c_str());
		isAudioStreaming = false;
	}
	
	(isAudioStreaming) ? rl::PlayMusicStream(*audioLong) : rl::PlaySound(*audioShort);
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

bool Previewer::InPlayheadBounds()
{
	sf::Vector2f mPos = InputManager::Get().MousePos();
	return (mPos.x >= playheadPos.x && mPos.y >= playheadPos.y
		&& mPos.x <= playheadPos.x + playheadDim.x && mPos.y <= playheadPos.y + playheadDim.y);
}
