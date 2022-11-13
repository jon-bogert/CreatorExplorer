#include "Pch.h"
#include "Application.h"

#define DEF_DIR "C:/Users/Jon/";

int main(int argc, char* argv[])
{
	std::string path = (argc > 1) ? argv[1] : DEF_DIR;
	Application::Get().Setup(path);
	Application::Get().Update();
	return 0;
}