#include "Pch.h"
#include "Application.h"
#include "Browser.h"
#include "Item.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "Previewer.h"

using std::filesystem::directory_iterator;
namespace fs = std::filesystem;

Application& Application::Get()
{
    static Application instance;
    return instance;
}

Application::Application()
{
}

void Application::Setup(std::string startPath)
{
    AssetManager::Get();
    Browser::Get();
    if (startPath == "")
        path.push_back("C");
    else
        ParsePath(startPath);

    LoadDir();
}

void Application::Update()
{
    while (isRunning)
    {
        InputManager::Get().Update();
        isRunning = Browser::Get().IsOpen();
       
        Navigation();
        Previewer::Get().Update();

        Browser::Get().Draw();
    }
}

std::string Application::PathStr()
{
    std::string out;
    out.append(path[0] + ":/");
    for (size_t i = 1; i < path.size(); ++i)
    {
        out.append(path[i] + "/");
    }
    return out;
}

bool Application::PathFrwd(std::string into)
{
    path.push_back(into);
    LoadDir();
    return true;
}

bool Application::PathBkwd()
{
	if (path.size() <= 1)
		return false;

	path.pop_back();
	LoadDir();

    Previewer::Get().LoadFile();
	return true;
}

bool Application::LoadDir()
{
    currItem = -1;
    Browser::Get().ClearItems();
    for (const auto& entry : directory_iterator(PathStr()))
    {
        std::string fileName = entry.path().filename().u8string();
        bool canAdd = true;
        //if (entry.path().has_extension())
        if (entry.is_regular_file())
            Browser::Get().AddItem(std::make_shared<Item>(fileName, entry.path().extension().u8string()));
        else
        {
            try
            {
                directory_iterator(PathStr() + fileName);
            }
            catch (std::filesystem::filesystem_error err)
            {
                canAdd = false;
            }

            if (canAdd)
            {
                Browser::Get().AddItem(std::make_shared<Item>(fileName));
            }
        }
    }
    Browser::Get().TriggerDraw();
    return true;
}

int Application::GetCurrItem() const
{
    return currItem;
}

std::string Application::GetLastName()
{
    return path[path.size() - 1];
}

void Application::ParsePath(std::string pathStr)
{
    if ((pathStr.size() >= 3 && pathStr[1] != ':' && !(pathStr[2] == '/' || pathStr[2] == '\\')) || pathStr.size() < 3)
    {
        std::cout << "Incompatible string passed into Application.ParsePath()." << std::endl;
        if (path.size() == 0)
            path.push_back("C");
        return;
    }

    //Path has Drive Letter
    path.clear();

    std::string step;
    step.push_back(pathStr[0]);
    path.push_back(step);
    step.clear();
    for (size_t i = 3; i < pathStr.size(); ++i)
    {
        if (pathStr[i] == '/' || pathStr[i] == '\\')
        {
            path.push_back(step);
            step.clear();
        }
        else
            step.push_back(pathStr[i]);
    }
    if (step.size() != 0) // no '/' at end of provided string
        path.push_back(step);

}

void Application::Navigation()
{
    if (InputManager::Get().ArrowDown())
    {
        if (currItem <= -1 && !Browser::Get().IsEmpty())
        {
            currItem = 0;
            Browser::Get().SetIsSelected(currItem, true);
            return;
        }
        if (currItem < Browser::Get().NumItems() - 1)
        {
            Browser::Get().SetIsSelected(currItem, false);
            ++currItem;
            Browser::Get().SetIsSelected(currItem, true);
        }
        if (currItem > Browser::Get().GetItemsVisible() + Browser::Get().GetScrollOffset() - 1)
            Browser::Get().AddScrollOffset(1);

        Browser::Get().TriggerDraw();
        Previewer::Get().LoadFile();
    }

    if (InputManager::Get().ArrowUp())
    {
        if (currItem <= -1 && !Browser::Get().IsEmpty())
        {
            currItem = 0;
            Browser::Get().SetIsSelected(currItem, true);
            return;
        }
        if (currItem > 0)
        {
            Browser::Get().SetIsSelected(currItem, false);
            --currItem;
            Browser::Get().SetIsSelected(currItem, true);
        }
        if (currItem < Browser::Get().GetScrollOffset())
            Browser::Get().AddScrollOffset(-1);

        Browser::Get().TriggerDraw();
        Previewer::Get().LoadFile();
    }

    if (InputManager::Get().Select())
    {
        SelectItem();
    }

    if (InputManager::Get().Back())
    {
        PathBkwd();
        Browser::Get().SetScrollOffset(0);
    }

    if (InputManager::Get().Preview())
    {
        Previewer::Get().ToggleWindow();
    }

    int scrollDelta = -InputManager::Get().ScrollDelta();
    if ((scrollDelta > 0 && Browser::Get().GetItemsVisible() + Browser::Get().GetScrollOffset() < Browser::Get().GetNumItems()) // Down
        ||(scrollDelta < 0 && Browser::Get().GetScrollOffset() > 0)) // Up
    {
        Browser::Get().AddScrollOffset(scrollDelta);
        Browser::Get().TriggerDraw();
    }

    if (InputManager::Get().MouseClick())
    {
        sf::Vector2f mousePos = InputManager::Get().MousePos();
        int newCurrItem = static_cast<int>(mousePos.y / Browser::Get().GetSpacing()) + Browser::Get().GetScrollOffset();
        if (InputManager::Get().MouseDoubleClick() && newCurrItem == currItem)
        {
            SelectItem();
        }
        else
        {
            if (currItem >= 0)
                Browser::Get().SetIsSelected(currItem, false);
            currItem = newCurrItem;

            if (currItem < Browser::Get().GetNumItems())
                Browser::Get().SetIsSelected(currItem, true);
            else currItem = -1;

            Browser::Get().TriggerDraw();
            Previewer::Get().LoadFile();
        }
    }
}

void Application::SelectItem()
{
    if (currItem <= -1)
        return;

    if (Browser::Get().GetIsFolder(currItem))
    {
        PathFrwd(Browser::Get().GetName(currItem));
    }
    else
    {
        system(("\"" + PathStr() + Browser::Get().GetName(currItem) + "\"").c_str());
    }
    Browser::Get().SetScrollOffset(0);
}
