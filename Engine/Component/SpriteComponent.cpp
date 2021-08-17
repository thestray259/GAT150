#include "SpriteComponent.h"
#include "Graphics/Renderer.h"
#include "Object/Actor.h"

namespace nc
{
	void SpriteComponent::Update()
	{
		//
	}

	void SpriteComponent::Draw(Renderer* renderer)
	{
		renderer->Draw(texture, owner->transform); 
	}
}