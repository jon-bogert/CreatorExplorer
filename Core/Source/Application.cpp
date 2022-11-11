#include "Pch.h"
#include "Application.h"
#include "Browser.h"
#include "Item.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "Previewer.h"

using std::filesystem::directory_iterator;

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
        if (entry.is_directory() || entry.is_regular_file())
        {
            std::string fileName = entry.path().filename().generic_string();

            if (entry.path().has_extension())
                Browser::Get().AddItem(std::make_shared<Item>(fileName, entry.path().extension().generic_string()));
            else
                Browser::Get().AddItem(std::make_shared<Item>(fileName));
        }
    }
    Browser::Get().TriggerDraw();
    return true;
}

int Application::GetCurrItem() const
{
    return currItem;
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
        if (currItem < Browser::Get().NumItems())
        {
            Browser::Get().SetIsSelected(currItem, false);
            ++currItem;
            Browser::Get().SetIsSelected(currItem, true);
        }
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
        Browser::Get().TriggerDraw();
        Previewer::Get().LoadFile();
    }

    if (InputManager::Get().Select())
    {
        if (currItem <= -1)
            return;

        if (Browser::Get().GetIsFolder(currItem))
        {
            PathFrwd(Browser::Get().GetName(currItem));
        }
        else
        {
            system(("\"" + PathStr() + Browser::Get().GetName(currItem)+ "\"").c_str());
        }
    }

    if (InputManager::Get().Back())
    {
        PathBkwd();
    }

    if (InputManager::Get().Preview())
    {
        Previewer::Get().ToggleWindow();
    }
}
