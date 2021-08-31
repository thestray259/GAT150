#pragma once
#include "GraphicsComponent.h"
#include <memory>

namespace nc
{
	class Texture; 

	class SpriteComponent : public GraphicsComponent
	{
	public: 
		std::unique_ptr<Object> Clone() const { return std::make_unique<SpriteComponent>(*this); }

		void Update() override; 
		void Draw(Renderer* renderer) override; 
		
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public: 
		SDL_Rect rect;

		std::shared_ptr<Texture> texture; 

	};
}