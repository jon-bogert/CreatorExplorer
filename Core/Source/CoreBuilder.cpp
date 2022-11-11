#include "Pch.h"
#include "Application.h"

int main()
{
	Application::Get().Setup("C:/Users/Jon");
	Application::Get().Update();
	return 0;
}