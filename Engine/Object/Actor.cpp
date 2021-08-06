#include "Actor.h"
#include "Graphics/Renderer.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		transform.rotation += 180.0f * dt; 

		transform.Update(); 
		std::for_each(children.begin(), children.end(), [](auto& child) { child->transform.Update( child->parent->transform.matrix ); });
	}

	void Actor::Draw(Renderer* renderer)
	{
		renderer->Draw(texture, transform);
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this; 
		children.push_back(std::move(actor)); 
	}

	float Actor::GetRadius()
	{
		//return (std::max(texture->GetSize().x, texture->GetSize().y) * 0.5f); 
		return (texture) ? texture->GetSize().Length() * 0.5f : 0; 
	}
}
