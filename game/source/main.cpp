#if defined(unix)
	#include <X11/Xlib.h>
#endif

#include <Engine/Application.hpp>

#include "LoadingScene.hpp"
#include "GameScene.hpp"
#include "VictoryScene.hpp"
#include "DefeatScene.hpp"

int main()
{
	// This is required for multithreading with OpenGL in unix.
	#if defined(unix)
		XInitThreads();
	#endif

	// Create the game.
	Engine::Application game(1280, 900, "Total Resistance");

	// Register scenes.
	game.RegisterScene<LoadingScene>("MainLoadingScene");
	game.RegisterScene<GameScene>("MainGameScene");
	game.RegisterScene<VictoryScene>("VictoryScene");
	game.RegisterScene<DefeatScene>("DefeatScene");

	// Push the splash scene to make it active.
	game.PushScene("MainLoadingScene");

	// Run the game.
	game.Run();

	return 0; // Success!
}
