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
	while (!quit)
	{
		SDL_PollEvent(&event); 
		switch (event.type)
		{
		case SDL_QUIT: 
			quit = true; 
			break; 
		}

		engine.Update(0); 
		scene.Update(0); 
		quit = (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);

		engine.Get<nc::Renderer>()->BeginFrame(); 

		scene.Draw(engine.Get<nc::Renderer>());
		//nc::Vector2 position{ 300, 400 }; 
		//engine.Get<nc::Renderer>()->Draw(texture, position);

		engine.Get<nc::Renderer>()->EndFrame(); 

		//for (size_t i = 0; i < 50; i++)
		//{
		//	SDL_Rect src{ 32, 64, 32, 64 }; 
		//	SDL_Rect dest{ nc::RandomRangeInt(0, screen.x), nc::RandomRangeInt(0, screen.y), 16, 24}; 
		//	SDL_RenderCopy(renderer, texture, &src, &dest); 
		//}

	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}
