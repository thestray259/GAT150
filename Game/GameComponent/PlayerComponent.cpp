#include "PlayerComponent.h"
#include "Engine.h"

using namespace nc; 

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner); 
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner); 

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "Audio/hurt.wav"); 
}

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

	if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Pressed)
	{
		force.y -= jump;
	}

	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_S) == InputSystem::eKeyState::Held)
	{
		force.y += speed;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>(); 
	assert(physicsComponent); 
	physicsComponent->ApplyForce(force); 

	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>(); 
	assert(spriteAnimationComponent); 
	if (physicsComponent->velocity.x > 0.1) spriteAnimationComponent->StartSequence("walk_right"); 
	else if (physicsComponent->velocity.x < -0.1) spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("idle");
}

void PlayerComponent::OnCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data); 
	Actor* actor = reinterpret_cast<Actor*>(p); 

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.push_back(actor); 
	}

	if (istring_compare(actor->tag, "Enemy"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt"); 

		nc::Event event;
		event.name = "PlayerDead";
		event.data = std::string("yes I'm dead");
		owner->scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}

void PlayerComponent::OnCollisionExit(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.remove(actor);
	}
}

bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed); 
	JSON_READ(value, jump); 
	JSON_READ(value, fireRate); 
	JSON_READ(value, fireTimer); 

	return true;
}

