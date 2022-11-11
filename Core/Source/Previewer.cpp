#include "Pch.h"
#include "Item.h"
#include "Previewer.h"
#include "InputManager.h"
#include "Browser.h"
#include "Application.h"

Previewer& Previewer::Get()
{
	static Previewer instance;
	return instance;
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

		window->clear(sf::Color::Black);
		if (sprite) window->draw(*sprite);
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
		std::shared_ptr<Item> item = Browser::Get().GetItem(currItem);

		switch (item->GetType())
		{
		case ItemType::Image:
			LoadImage(item);
			break;
		default:
			LoadDefault(item);
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
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(defSize.x, defSize.y), "CreatorExplorer - Previewer");
	LoadFile();
}

void Previewer::Deactivate()
{
	Reset();
	window = nullptr;
}

void Previewer::Reset()
{
	item = nullptr;

	texture = nullptr;
	sprite = nullptr;

	if (window->getSize().x != defSize.x || window->getSize().y != defSize.y)
		window = std::make_shared<sf::RenderWindow>(sf::VideoMode(defSize.x, defSize.y), "CreatorExplorer - Previewer");
}

void Previewer::LoadRoot()
{
}

void Previewer::LoadDefault(std::shared_ptr<Item> item)
{
}

void Previewer::LoadImage(std::shared_ptr<Item> item)
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
