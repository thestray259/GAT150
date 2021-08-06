#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	nc::Engine engine; 
	engine.Startup(); 
	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600); 

	nc::Scene scene; 
	scene.engine = &engine; 

	nc::SetFilePath("../Resources");

	std::shared_ptr<nc::Texture> texture = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine.Get<nc::Renderer>());

	for (size_t i = 0; i < 10; i++)
	{
		nc::Transform transform{ {nc::RandomRangeInt(0, 800), nc::RandomRangeInt(0, 600)}, static_cast<float>(nc::RandomRangeInt(0, 360)), 1.0f };
		std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture); 
		scene.AddActor(std::move(actor)); 
	}

	bool quit = false; 
	SDL_Event event; 
	float quitTime = engine.time.time + 3.0f; 

	while (!quit)
	{
		SDL_PollEvent(&event); 
		switch (event.type)
		{
		case SDL_QUIT: 
			quit = true; 
			break; 
		}

		// update
		engine.Update(); 
		scene.Update(engine.time.deltaTime); 
		quit = (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);

		if (engine.time.time >= quitTime) quit = true; 
		engine.time.timeScale = 0.1f; 

		// draw 
		engine.Get<nc::Renderer>()->BeginFrame(); 
		scene.Draw(engine.Get<nc::Renderer>());
		engine.Get<nc::Renderer>()->EndFrame(); 
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}
