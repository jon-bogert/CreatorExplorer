#include "Pch.h"
#include "Application.h"

#define USE_MAIN // Use when switching between console and windows application

#ifdef USE_WINMAIN // headers for WinMain
#include <windows.h>
#include <atlstr.h>
#endif

extern std::string res = {};

#define DEF_DIR "C:/"

#ifdef USE_WINMAIN
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR cmdline, int cmdshow)
{
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	std::string path = DEF_DIR;
	if (argc > 1)
		path = CW2A(argv[1]);
	if (argc > 2) res = CW2A(argv[2]);
	Application::Get().Setup(path);
	Application::Get().Update();
	return 0;
}
#endif

#ifdef USE_MAIN
int main(int argc, char* argv[])
{
	std::string path = DEF_DIR;
	if (argc > 1)
		path = argv[1];
	if (argc > 2) res = argv[2];
	Application::Get().Setup(path);
	Application::Get().Update();
	return 0;
}
#endif
