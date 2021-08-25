#include "PlayerComponent.h"
#include "Engine.h"

using namespace nc; 

void PlayerComponent::Update()
{
	Vector2 force = Vector2::zero; 
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Held) 
	{
		force.x -= speed; 
	}

	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Held)
	{
		force.x += speed;
	}

	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_W) == InputSystem::eKeyState::Held)
	{
		force.y -= speed;
	}

	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_S) == InputSystem::eKeyState::Held)
	{
		force.y += speed;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>(); 
	assert(physicsComponent); 

	physicsComponent->ApplyForce(force); 
}

bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed); 

	return true;
}

