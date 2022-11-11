#pragma once
#include "Pch.h"

class Application
{
private:
	std::vector<std::string> path;

	int currItem = -1;
	bool isRunning = true;

public:
	static Application& Get();

private:
	Application();
public:
	~Application() {}

	void Setup(std::string startPath = "");
	void Update();

	std::string PathStr();
	bool PathFrwd(std::string into);
	bool PathBkwd();
	bool LoadDir();

	int GetCurrItem() const;

private:
	void ParsePath(std::string pathStr);
	void Navigation();
};

