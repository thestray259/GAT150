#include "Engine.h"

namespace nc
{
	void Engine::Startup()
	{
		systems.push_back(std::make_unique<EventSystem>());
		systems.push_back(std::make_unique<ResourceSystem>());
		systems.push_back(std::make_unique<InputSystem>());
		systems.push_back(std::make_unique<Renderer>());

		std::for_each(systems.begin(), systems.end(), [](auto& system) { system->Startup(); });
	}

	void Engine::Shutdown()
	{
		std::for_each(systems.begin(), systems.end(), [](auto& system) { system->Shutdown(); });
	}

	void Engine::Update()
	{
		time.Tick(); 
		std::for_each(systems.begin(), systems.end(), [this](auto& system) { system->Update(time.deltaTime); });
	}

	void Engine::Draw()
	{
		// 
	}
}
