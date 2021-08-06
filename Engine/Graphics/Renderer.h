#pragma once
#include "Framework/System.h"
#include "Texture.h"
#include "Math/Transform.h"
#include <SDL.h>
#include <string>

namespace nc
{
	class Renderer : public System
	{
	public: 
		void Startup() override;
		void Shutdown() override;
		void Update(float dt) override;

		void Create(const std::string& name, int width, int height); 
		void BeginFrame(); 
		void EndFrame(); 

		void Draw(std::shared_ptr<nc::Texture> texture, const Vector2& position, float angle = 0, const Vector2& scale = Vector2::one); 
		void Draw(std::shared_ptr<nc::Texture> texture, const Transform& transform); 

		friend class Texture; 

	private: 
		SDL_Renderer* renderer = nullptr; 
		SDL_Window* window = nullptr; 
	};
}