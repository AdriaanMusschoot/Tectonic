#include "Amugen.h"
#include <filesystem>
#include "Windows.h"
#include <iostream>

#include "SceneManager.h"
#include "LoadFunctions.h"

namespace fs = std::filesystem;

int main(int, char*[]) 
{
	//thx mat for the console fix
#ifdef WIN32
	if (AllocConsole()) 
	{
		FILE* pEmpty;
		freopen_s(&pEmpty, "CONOUT$", "w", stdout);
		freopen_s(&pEmpty, "CONOUT$", "w", stderr);
	}
#endif

	fs::path data_location = "./Resources/";
	if(not fs::exists(data_location))
		data_location = "../Resources/";

	amu::Amugen engine(data_location, 600, 400);

	auto& sm{ amu::SceneManager::GetInstance() };

	sm.CreateScene("Main Level", tct::LoadMainScene);

	sm.SetCurrentScene("Main Level");

	engine.Run();

    return 0;
}
