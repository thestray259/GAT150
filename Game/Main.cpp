#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <cassert>

int main(int, char**)
{
	nc::Engine engine; 
	engine.Startup(); 
	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600); 

	nc::Scene scene; 
	scene.engine = &engine; 

	nc::SetFilePath("../Resources");

	engine.Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav"); 
	engine.Get<nc::AudioSystem>()->AddAudio("music", "audio/gameMusic.mp3"); 
	nc::AudioChannel channel = engine.Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true); 

	std::shared_ptr<nc::Texture> texture = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine.Get<nc::Renderer>());
	std::shared_ptr<nc::Texture> particle01 = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine.Get<nc::Renderer>());
	std::shared_ptr<nc::Texture> particle02 = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("particle02.png", engine.Get<nc::Renderer>());

	for (size_t i = 0; i < 10; i++)
	{
		nc::Transform transform{ {nc::RandomRangeInt(0, 800), nc::RandomRangeInt(0, 600)}, static_cast<float>(nc::RandomRangeInt(0, 360)), 1.0f };
		std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture); 
		scene.AddActor(std::move(actor)); 
	}

	// get font from resource system
	int size = 16;
	std::shared_ptr<nc::Font> font = engine.Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/Techtro.ttf", &size); 

	// create font texture
	std::shared_ptr<nc::Texture> textTexture = std::make_shared<nc::Texture>(engine.Get<nc::Renderer>());

	// set font texture with font surface
	textTexture->Create(font->CreateSurface("hello world", nc::Color{ 1, 1, 1 }));

	// add font texture to resource system
	engine.Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

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

		if (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
		{
			quit = true; 
		}

		if (engine.Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed)
		{
			nc::Vector2 position = engine.Get<nc::InputSystem>()->GetMousePosition(); 
			engine.Get<nc::ParticleSystem>()->Create(position, 10, 2.0f, particle01, 50.0f, nc::DegToRad(45), 10.0f);
			engine.Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f));	
			channel.SetPitch(nc::RandomRange(0.2f, 2.0f)); 
		}

		//if (engine.time.time >= quitTime) quit = true; 
		//engine.time.timeScale = 0.1f; 

		// draw 
		engine.Get<nc::Renderer>()->BeginFrame(); 

		scene.Draw(engine.Get<nc::Renderer>());
		engine.Draw(engine.Get<nc::Renderer>());;

		nc::Transform t;
		t.position = { 30, 30 };
		engine.Get<nc::Renderer>()->Draw(textTexture, t);

		engine.Get<nc::Renderer>()->EndFrame(); 
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}
