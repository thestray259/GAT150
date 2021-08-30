#include "PickupComponent.h"
#include "Engine.h"

using namespace nc;

void PickupComponent::Create()
{
	owner->scene->engine->Get<nc::EventSystem>()->Subscribe("collision_enter", std::bind(&PickupComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<nc::EventSystem>()->Subscribe("collision_exit", std::bind(&PickupComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "Audio/coin.wav");
}

void PickupComponent::Update()
{
}

void PickupComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player"))
	{
		contacts.push_back(actor);
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");
	}
}

void PickupComponent::OnCollisionExit(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player"))
	{
		contacts.remove(actor);
		destroy = true;
	}
}

bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, destroy); 

	return true;
}