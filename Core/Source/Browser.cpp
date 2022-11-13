#include "Pch.h"
#include "Browser.h"
#include "Item.h"
#include "Application.h"

Browser& Browser::Get()
{
	static Browser instance;
	return instance;
}

Browser::Browser()
{
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(resolution.x, resolution.y), "CreatorExplorer - Browser");
	itemsVisible = resolution.y / spacing;
}

int Browser::SearchItem(std::shared_ptr<Item> item)
{
	for (size_t i = 0; i < itemBuffer.size(); ++i)
	{
		if (item == itemBuffer[i])
			return static_cast<int>(i);
	}

	return -1;
}

bool Browser::AddItem(std::shared_ptr<Item> item)
{
	if (SearchItem(item) >= 0)
		return false;

	itemBuffer.push_back(item);
	return true;
}

bool Browser::RemoveItem(std::shared_ptr<Item> item)
{
	bool result{};
	for (auto i = itemBuffer.begin(); i != itemBuffer.end();)
	{
		if (*i == item)
		{
			i = itemBuffer.erase(i);
			result = true;
		}
		else ++i;
	}
	return result;
}

void Browser::ClearItems()
{
	itemBuffer.clear();
}

std::shared_ptr<Item> Browser::GetItem(int index)
{
	if (index == -1)
		return std::make_shared<Item>(Application::Get().GetLastName());
	return itemBuffer[index];
}

size_t Browser::NumItems()
{
	return itemBuffer.size();
}

void Browser::SetIsSelected(size_t index, bool onOff)
{
	if (index > NumItems())
	{
		std::cout << "Invalid item index." << std::endl;
		return;
	}
	itemBuffer[index]->SetIsSelected(onOff);
}

std::string Browser::GetName(size_t index)
{
	if (index >= NumItems())
	{
		std::cout << "Invalid item index." << std::endl;
		return std::string();
	}
	return itemBuffer[index]->GetName();
}

bool Browser::GetIsFolder(size_t index)
{
	if (index >= NumItems())
	{
		std::cout << "Invalid item index." << std::endl;
		return false;
	}
	return itemBuffer[index]->GetIsFolder();
}

bool Browser::IsEmpty()
{
	return itemBuffer.empty();
}

int Browser::GetNumItems() const
{
	return itemBuffer.size();
}

float Browser::GetSpacing() const
{
	return spacing;
}

int Browser::GetItemsVisible() const
{
	return itemsVisible;
}

int Browser::GetScrollOffset() const
{
	return scrollOffset;
}

void Browser::AddScrollOffset(const int add)
{
	scrollOffset += add;
	CheckScrollOffset();
}

void Browser::SetScrollOffset(const int setTo)
{
	scrollOffset = setTo;
	CheckScrollOffset();
}

void Browser::Draw()
{
	if (triggerDraw)
	{
		triggerDraw = false;
		window->clear({ 20, 20, 20, 255 });
		for (size_t i = 0; i < itemBuffer.size(); ++i)
		{
			itemBuffer[i]->Draw(window, { 0.f, spacing * (i - scrollOffset) });
		}
		window->display();
	}
}

bool Browser::IsOpen()
{
	return window->isOpen();
}

void Browser::CloseWindow()
{
	window->close();
}

std::shared_ptr<sf::RenderWindow> Browser::GetWindow()
{
	return window;
}

void Browser::TriggerDraw()
{
	triggerDraw = true;
}

void Browser::CheckScrollOffset()
{
	if (scrollOffset >= itemBuffer.size() - itemsVisible)
		scrollOffset = itemBuffer.size() - itemsVisible;
	if (scrollOffset < 0)
		scrollOffset = 0;
}
